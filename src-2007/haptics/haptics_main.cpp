// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//////////////////////////////////////////////////////////////////////
// haptics.cpp: implementation of the haptics class.
//
// NOTE:  This is applicaiton specific code for HDAL based haptics.
//
//
// haptics class provides the interface between the application and
// the haptic device via HDAL.
//
// The haptic device is treated as the world coordinate system.
//
// - Each servo tick the callback ContactCB is called.
//
// - ContactCB queries end-effector characteristics (pos, transform, button).
//   It then calls the calculateForce object associated with the tObject sending
//   the resultant calculation to the haptic device.
//
// - Client thread access to the haptics class should be proceeded by a
//   calling hdlHaptics::syncFromServo().  The call syncs the client thread
//   copy of the end-effector characteristics from the servo thread using
//   GetStateCB.
//
//////////////////////////////////////////////////////////////////////

#include "cbase.h"

#include "haptics_main.h"
#include "in_haptics.h"

#include <math.h>
#include <assert.h>
#include <direct.h>

//BA CES07
#include "haptics_effect.h"
#include "recoil_effect.h"
#include "damage_effect.h"
#include "footsteps_effect.h"
#include "carry_effect.h"
#include "vehicle_effect.h"
#include "acceleration_effect.h"
// for ui detection
#include <vgui_controls/Controls.h>
#include "vgui/isurface.h"
#ifdef PSIMOD
#include "psimod/psimod_player_shared.h"
#endif
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Constants used to tune haptic feel
// These are "master" defaults, which will be in effect if the
//   HDAL device type is unknown.  See initParams() for known
//   device type defaults.
static const char* g_deviceType;
static double DEFAULT_SCALE = 2.0;
static double DEFAULT_STIFFNESS = 2.0;
static double EFFECT_STIFFNESS = 0.50;

const double SCALE_FACTOR = 1.0;

// max dimension for haptic workspace.
static double HAPTIC_WORKSPACE = 100.0;

// dimension variable use to scale device movement
static double GAMESPACE_DIMENSION = 100.0;

// Duration of triggered effects, in ms
int MAX_EFFECT_DURATION = 500;

// The device's positional extents (until the extent reading function works)
// read (minx, miny, minz, maxx, maxy, maxz)
static double POSITION_EXTENTS[NUM_COORDS] = {0.18,0.10,0.07,0.18,0.19,0.09};

hdlHaptics* hdlHaptics::s_instance = 0;

static HDLDeviceHandle g_hDevice = HDL_INVALID_HANDLE;
static bool g_isServoDone = false;
static HANDLE g_hServoThreadHandle;

//BA CES07
CarryEffect* CarryEffect::s_instance = 0;

//#define LOG

#ifdef LOG
FILE* fd = 0;
#endif

ConVar hap_var_vehicle_force_clear("hap_var_vehicle_force_clear", "0", FCVAR_REPLICATED);

// * Movement model variables *

ConVar hap_debug_control("hap_debug_control", "0", FCVAR_ARCHIVE);

ConVar hap_control_type_inner("hap_control_type_inner", "0", 0,
	"Specifies the control style for the inner control area. 0 = Constant, 1 = Linear, 2 = Discrete(Not working yet), 3 = Exponential");
ConVar hap_control_type_outer("hap_control_type_outer", "1", 0,
	"Specifies the control style for the outer control area. 0 = Constant, 1 = Linear, 2 = Discrete(Not working yet), 3 = Exponential");

// Has +/- sign convention
ConVar hap_cb_left_start("hap_cb_left_start", "-0.035", FCVAR_ARCHIVE,
	"X-Axis (negative) boundary point (absolute) between inner and outer control areas");
ConVar hap_cb_bottom_start("hap_cb_bottom_start", "-0.035", FCVAR_ARCHIVE,
	"Y-Axis (negative) boundary point (absolute) between inner and outer control areas");
ConVar hap_cb_far_start("hap_cb_far_start", "-0.06", FCVAR_ARCHIVE,
	"Z-Axis (negative) boundary point (absolute) between inner and outer control areas");
ConVar hap_cb_right_start("hap_cb_right_start", "0.035", FCVAR_ARCHIVE,
	"X-Axis (positive) boundary point (absolute) between inner and outer control areas");
ConVar hap_cb_top_start("hap_cb_top_start", "0.035", FCVAR_ARCHIVE,
	"Y-Axis (positive) boundary point (absolute) between inner and outer control areas");
ConVar hap_cb_near_start("hap_cb_near_start", "0.02", FCVAR_ARCHIVE,
	"Z-Axis (positive) boundary point (absolute) between inner and outer control areas");

// Has +/- sign convention
ConVar hap_cb_left_end("hap_cb_left_end", "-0.05", FCVAR_ARCHIVE,
	"X-Axis (negative) boundary point at the outer control areas (device extents)");
ConVar hap_cb_bottom_end("hap_cb_bottom_end", "-0.05", FCVAR_ARCHIVE,
	"Y-Axis (negative) boundary point at the outer control areas (device extents)");
ConVar hap_cb_far_end("hap_cb_far_end", "-0.05", FCVAR_ARCHIVE,
	"Z-Axis (negative) boundary point at the outer control areas (device extents)");
ConVar hap_cb_right_end("hap_cb_right_end", "0.05", FCVAR_ARCHIVE,
	"X-Axis (positive) boundary point at the outer control areas (device extents)");
ConVar hap_cb_top_end("hap_cb_top_end", "0.05", FCVAR_ARCHIVE,
	"Y-Axis (positive) boundary point at the outer control areas (device extents)");
ConVar hap_cb_near_end("hap_cb_near_end", "0.05", FCVAR_ARCHIVE,
	"Z-Axis (positive) boundary point at the outer control areas (device extents)");

ConVar hap_cb_springk("hap_cb_springk", "300.0", FCVAR_ARCHIVE,
	"This factor is used to define the spring constant (Linear model)");

ConVar hap_cb_scale("hap_control_scale", "1.0");

// Vehicle specific haptic zones
//BA CES07
ConVar hap_var_isInVehicle("hap_var_isInVehicle", "0", FCVAR_REPLICATED);

// Has +/- sign convention
ConVar hap_cbvehicle_left_start("hap_cbvehicle_left_start", "-0.003", FCVAR_ARCHIVE,
	"X-Axis (negative) boundary point (absolute) between inner and outer control areas");
ConVar hap_cbvehicle_bottom_start("hap_cbvehicle_bottom_start", "-0.1", FCVAR_ARCHIVE,
	"Y-Axis (negative) boundary point (absolute) between inner and outer control areas");
ConVar hap_cbvehicle_far_start("hap_cbvehicle_far_start", "-0.035", FCVAR_ARCHIVE,
	"Z-Axis (negative) boundary point (absolute) between inner and outer control areas");
ConVar hap_cbvehicle_right_start("hap_cbvehicle_right_start", "0.003", FCVAR_ARCHIVE,
	"X-Axis (positive) boundary point (absolute) between inner and outer control areas");
ConVar hap_cbvehicle_top_start("hap_cbvehicle_top_start", "0.1", FCVAR_ARCHIVE,
	"Y-Axis (positive) boundary point (absolute) between inner and outer control areas");
ConVar hap_cbvehicle_near_start("hap_cbvehicle_near_start", "0.035", FCVAR_ARCHIVE,
	"Z-Axis (positive) boundary point (absolute) between inner and outer control areas");

ConVar hap_cbvehicle_springk("hap_cbvehicle_springk", "250.0", FCVAR_ARCHIVE,
	"This factor is used to define the spring constant (Linear model)");
ConVar hap_cbvehicle_scale("hap_controlvehicle_scale", "1.0");

ConVar hap_toolweight("hap_toolweight", "1.0", FCVAR_ARCHIVE);

ConVar hap_panel_xpos("hap_panel_xpos", "0");
ConVar hap_panel_ypos("hap_panel_ypos", "0");
ConVar hap_panel_zpos("hap_panel_zpos", "0");

// Variable Updaters
class HapticsVariables
{
public:
	HapticsVariables()
		:	m_scaleFactor(SCALE_FACTOR),
			//m_scaleDamage(0.5),
			//m_scaleCarry(0.5),
			m_controlType(Constant),	
			m_controlLengthInner_x_min(0.01),
			m_controlLengthInner_y_min(0.01),
			m_controlLengthInner_z_min(0.01),
			m_controlLengthInner_x_max(0.01),
			m_controlLengthInner_y_max(0.01),
			m_controlLengthInner_z_max(0.01),
			//Temp BA
			m_controlBoxMinX(-0.01),
			m_controlBoxMinY(-0.01),
			m_controlBoxMinZ(-0.01),
			m_controlBoxMaxX(0.01),
			m_controlBoxMaxY(0.01),
			m_controlBoxMaxZ(0.01),
			m_controlBoxStiffness(0.01),
			//
			m_controlForce_A(0.0),
			m_controlForce_B(0.0),
			m_controlForce_C(0.6),
			m_controlDampeningInner(10),
			m_controlDampeningOuter(0),
			m_carryWeight(0) {};

	double m_scaleFactor;
	//double m_damageScale;
	int m_controlType;
	double m_controlLengthInner_x_min;
	double m_controlLengthInner_y_min;
	double m_controlLengthInner_z_min;
	double m_controlLengthInner_x_max;
	double m_controlLengthInner_y_max;
	double m_controlLengthInner_z_max;
	//Temp BA
	double m_controlBoxMinX;
	double m_controlBoxMinY;
	double m_controlBoxMinZ;
	double m_controlBoxMaxX;
	double m_controlBoxMaxY;
	double m_controlBoxMaxZ;
	double m_controlBoxStiffness;
	//
	double m_controlForce_A;
	double m_controlForce_B;
	double m_controlForce_C;
	double m_controlDampeningInner;
	double m_controlDampeningOuter;
	double m_carryWeight;

	etVector m_vehicleForce;
};

static HapticsVariables hapticsvars;

void hdlHaptics::setScaleFactor(double a_scale) {hapticsvars.m_scaleFactor = a_scale;}
//void hdlHaptics::setDamageFactor(double a_factor) {hapticsvars.m_scaleDamage = a_factor;}

void hdlHaptics::setControlType(int a_type) {hapticsvars.m_controlType = a_type;}
void hdlHaptics::setControlCube(double a_size)
{hapticsvars.m_controlLengthInner_x_min = a_size; hapticsvars.m_controlLengthInner_y_min = a_size; hapticsvars.m_controlLengthInner_z_min = a_size;
 hapticsvars.m_controlLengthInner_x_max = a_size; hapticsvars.m_controlLengthInner_y_max = a_size; hapticsvars.m_controlLengthInner_z_max = a_size;}
void hdlHaptics::setControlLengthInnerX(double a_lengthMin, double a_lengthMax)
	{hapticsvars.m_controlLengthInner_x_min = a_lengthMin; hapticsvars.m_controlLengthInner_x_max = a_lengthMax;}
void hdlHaptics::setControlLengthInnerY(double a_lengthMin, double a_lengthMax)
	{hapticsvars.m_controlLengthInner_y_min = a_lengthMin; hapticsvars.m_controlLengthInner_x_max = a_lengthMax;}
void hdlHaptics::setControlLengthInnerZ(double a_lengthMin, double a_lengthMax)
	{hapticsvars.m_controlLengthInner_z_min = a_lengthMin; hapticsvars.m_controlLengthInner_x_max = a_lengthMax;}

	//TEMP BA
void hdlHaptics::setControlBoxX(double a_lengthMin, double a_lengthMax)
	{hapticsvars.m_controlBoxMinX = a_lengthMin; hapticsvars.m_controlBoxMaxX = a_lengthMax;}
void hdlHaptics::setControlBoxY(double a_lengthMin, double a_lengthMax)
	{hapticsvars.m_controlBoxMinY = a_lengthMin; hapticsvars.m_controlBoxMaxY = a_lengthMax;}
void hdlHaptics::setControlBoxZ(double a_lengthMin, double a_lengthMax)
	{hapticsvars.m_controlBoxMinZ = a_lengthMin; hapticsvars.m_controlBoxMaxZ = a_lengthMax;}
void hdlHaptics::setControlBoxStiffness(double a_force)
	{hapticsvars.m_controlBoxStiffness = a_force;}

	
void hdlHaptics::setControlCoefficientA(double a_force) {hapticsvars.m_controlForce_A = a_force;}
void hdlHaptics::setControlCoefficientB(double a_force) {hapticsvars.m_controlForce_B = a_force;}
void hdlHaptics::setControlCoefficientC(double a_force) {hapticsvars.m_controlForce_C = a_force;}
void hdlHaptics::setControlDampeningInner(double a_dampening) {hapticsvars.m_controlDampeningInner = a_dampening;}
void hdlHaptics::setControlDampeningOuter(double a_dampening) {hapticsvars.m_controlDampeningOuter = a_dampening;}

// Main callback that runs the dynamics simulation within the haptics loop.
// HD API specific code.
ConVar hap_debug_contact("hap_debug_contact","0",FCVAR_ARCHIVE);
HDLServoOpExitCode ContactCB(void *data) // req
{
    hdlHaptics* l_pHaptics = static_cast< hdlHaptics* >( data );
    if ( !l_pHaptics->m_isServoRunning ) return HDL_SERVOOP_CONTINUE;
 
    double position[3];
    int buttons;

    hdlToolPosition(position);
    //hdlToolButton((bool *)&buttons);
	hdlToolButtons(&buttons);

	hap_panel_xpos.SetValue((float)position[0]);
	hap_panel_ypos.SetValue((float)position[1]);
	hap_panel_zpos.SetValue((float)position[2]);

    l_pHaptics->m_buttonStateServo = buttons;

	l_pHaptics->m_terminusPositionServo = 
		etVector(position[0], // * hapticsvars.m_scaleFactor, 
				 position[1], // * hapticsvars.m_scaleFactor, 
				 position[2]); // * hapticsvars.m_scaleFactor);

    
    // send force to device( if not in cursor mode )
	if(!InUIMode())
	{
   		// query force from forceField of tObject        
		etVector l_forceVec = l_pHaptics->calculateForces();
		double l_force[3];
		for(int i = 0;i!=3;i++)
		{
			l_force[i] = 0;
		}
		//NAN check
		if(l_forceVec.x()==l_forceVec.x())
			l_force[0] = l_forceVec.x();
		//NAN check
		if(l_forceVec.y()==l_forceVec.y())
			l_force[1] = l_forceVec.y();
		//NAN check
		if(l_forceVec.z()==l_forceVec.z())
			l_force[2] = l_forceVec.z();

		if(hap_debug_contact.GetInt()!=0)
		{
			Msg("[ %f %f %f ]\n",l_force[0],l_force[1],l_force[2]);
		}
		hdlSetToolForce( l_force );
	}else{
		//zero
		double zero[3] ={0,0,0};
		hdlSetToolForce(zero );
	}
    return HDL_SERVOOP_CONTINUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void hdlHaptics::create() // req
{
    assert(!s_instance);
	s_instance = new hdlHaptics();
}

void hdlHaptics::destroy() // req
{
    assert(s_instance);
    delete s_instance;
}

hdlHaptics* hdlHaptics::instance() // req
{
    return s_instance;
}

void hdlHaptics::start() // req
{ 
    m_isServoRunning = true; 
}

void hdlHaptics::stop() // req
{ 
    m_isServoRunning = false; 
}

// **********
// Constructor
// **********
hdlHaptics::hdlHaptics() // req, but needs modification
    :	m_hContactCB(HDL_INVALID_HANDLE),
		m_terminusPositionServo(0,0,0),
		m_terminusPositionClient(0,0,0),
		m_buttonStateServo(0),
		//m_buttonStateClient(0),    
		m_floorHeightServo(0),
		m_floorHeightClient(0),
		m_triggerEffectServo(false),    
		m_triggerEffectClient(false),

		m_oldTime(0),
		m_bExpUpdate(0),
		m_nExpCycles(0),

		//BA CES07
		m_tempEffect(0),
		tempTest(0.0),

		m_doRecoilEffect(false),
		m_doPlayerShotEffect(false),
		m_doPlayerFootstepEffect(false),
		m_doPlayerCarryEffect(false),
		m_doVehicleForceEffect(false),
		m_doAvatarForceEffect(false)
{
    // Initialize scale
    m_scale = DEFAULT_SCALE;
	m_avatarEffect = NULL;
    // Initialize basic scene graph (scene and root)
    initialize();
}


// **********
// Destructor
// **********
hdlHaptics::~hdlHaptics() // req
{
#ifdef LOG
    fclose(fd);
#endif
    hdlDestroyServoOp( m_hContactCB );
    hdlUninitDevice( g_hDevice );
    g_hDevice = HDL_INVALID_HANDLE;

	s_instance = 0;
}

// **********
// Does first order initialization of GHOST scene graph.
// Scene and Root nodes are set up.
// **********
bool hdlHaptics::initialize() // req
{
    // Initialize device
    const HDLDeviceID deviceIDFiller = 0;
    g_hDevice = hdlInitDevice(deviceIDFiller);

    hdlMakeCurrent(g_hDevice);
    hdlDeviceWorkspace(m_deviceWorkspaceDim);
	assert(m_deviceWorkspaceDim);

	m_hContactCB = hdlCreateServoOp(ContactCB, this, bNonBlocking);

    m_isServoRunning = true;

    initParams();

	// Set gamespace scale
	m_gamespaceScale = GAMESPACE_DIMENSION / HAPTIC_WORKSPACE;

    return (true);
}

// Callback to sync data from haptics thread to client (graphics) thread.
HDLServoOpExitCode GetStateCB(void *pUsrData) // req
{
    hdlHaptics* l_pHaptics = static_cast<hdlHaptics*>(pUsrData);
    l_pHaptics->doSyncOp();

    return HDL_SERVOOP_EXIT;
}

void hdlHaptics::doSyncOp() // req.. I think
{
    m_terminusPositionClient = m_terminusPositionServo;
    m_terminusTransformClient = m_terminusTransformServo;
	m_buttonHistoryClient.push_front(m_buttonStateServo);
//  m_buttonStateClient = m_buttonStateServo;

    // pass client vars to servo
    m_floorHeightServo = m_floorHeightClient;
    m_triggerEffectServo = m_triggerEffectClient;
    m_triggerEffectClient = false; //turn off trigger in client

	//BA CES07
	if(m_tempEffect)
	{
		m_effectList.push_back(m_tempEffect);
		m_tempEffect = 0;
	}
  }

// Sets the state returned by haptics class
// in a thread safe manner by scheduling
// a copy in the servo thread.
void hdlHaptics::syncFromServo() // req
{
    hdlCreateServoOp(GetStateCB, this, bBlocking);
}

void hdlHaptics::getAbsolutePosition(double &x, double &y, double &z)
{
    etVector l_position = m_terminusPositionClient;
    
    x = l_position.x();
    y = l_position.y();
    z = l_position.z();
}

void hdlHaptics::getAbsolutePositionServo(double &x, double &y, double &z)
{
    etVector l_position = m_terminusPositionServo;
    
    x = l_position.x();
    y = l_position.y();
    z = l_position.z();
}

void hdlHaptics::getRelativePosition(double &x, double &y, double &z)
{
    etVector l_position = m_terminusPositionClient;
    
	if(l_position.x() < 0)
		x = l_position.x() / abs(hap_cb_left_end.GetFloat());
	else
		x = l_position.x() / abs(hap_cb_right_end.GetFloat());
    if(l_position.y() < 0)
		y = l_position.y() / abs(hap_cb_bottom_end.GetFloat());
	else
		y = l_position.y() / abs(hap_cb_top_end.GetFloat());
	if(l_position.z() < 0)
	    z = l_position.z() / abs(hap_cb_far_end.GetFloat());
	else
	    z = l_position.z() / abs(hap_cb_near_end.GetFloat());
}

void hdlHaptics::getRelativePositionServo(double &x, double &y, double &z)
{
    etVector l_position = m_terminusPositionServo;
    
    if(l_position.x() < 0)
		x = l_position.x() / POSITION_EXTENTS[MINX];
	else
		x = l_position.x() / POSITION_EXTENTS[MAXX];
    if(l_position.y() < 0)
		y = l_position.y() / POSITION_EXTENTS[MINY];
	else
		y = l_position.y() / POSITION_EXTENTS[MAXY];
	if(l_position.z() < 0)
	    z = l_position.z() / POSITION_EXTENTS[MINZ];
	else
	    z = l_position.z() / POSITION_EXTENTS[MAXZ];
}

// Returns the state of the device (stylus) switch -- true if depressed, false otherwise
// Returns false when history has been cleared.
bool hdlHaptics::getDeviceSwitchState(int &iButton)
{
	
	if(m_buttonHistoryClient.empty())
		return false;

	iButton = m_buttonHistoryClient.back();
	m_buttonHistoryClient.pop_back();
    
	//iButton = m_buttonStateClient;
	return true;
}

// Force calculation

// fRatio - 0.0 to 0.5, represents the percent ramp up (the other half is the ramp down)
double hdlHaptics::computeForceRatio(double fRatio) // linear model
{
	int l_half = (int)((double)(MAX_EFFECT_DURATION) * (1.0 - fRatio));
	double l_ratio=0.0;

	// First half of force simulation (from 0% -> 100%)
	if(m_effectDuration >= l_half)
	{
		l_ratio = ((double)MAX_EFFECT_DURATION - (double)m_effectDuration)/((double)MAX_EFFECT_DURATION - (double)l_half);
	}
	// Second half of force simulation (from 100% -> 0%)
	else
	{
		l_ratio = (double)m_effectDuration / (double)l_half;
	}

	return l_ratio;
}

// fRatio - 0.0 to 0.5, represents the percent ramp up (the other half is the ramp down)
double hdlHaptics::computeForceRatioExp(double fRatio, double fShape) // linear model
{
	double l_firsthalf = (double)(m_effectDuration) / (double)(MAX_EFFECT_DURATION);
	double l_norm = exp(fShape);
	double l_ratio=0.0;

	// First half of force simulation (from 0% -> 100%)
	if(l_firsthalf <= fRatio)
	{
		l_ratio = exp(-fShape / (l_firsthalf/fRatio)) / l_norm;
	}
	// Second half of force simulation (from 100% -> 0%)
	else
	{
		l_ratio = exp(-fShape / ((1.0-l_firsthalf)/(1.0 - fRatio))) / l_norm;
	}

	return l_ratio;
}
etVector hdlHaptics::getLastForceSent()
{
	return m_lastForces;
}
etVector hdlHaptics::computeRecoil( )
{
	//
	//DO FORCES
	//start the recoil force
	//
	etVector l_force(0,0,0);
	etMatrix l_recoilPitchMat, l_recoilYawMat, l_recoilRollMat;

	// should move these into the class...
	l_recoilPitchMat.setRotate(etOrientation(m_effectVelocity.x()/(double)MAX_EFFECT_DURATION, (double)1, (double)0, (double)0));
	l_recoilYawMat.setRotate(etOrientation(m_effectVelocity.y()/(double)MAX_EFFECT_DURATION, (double)0, (double)1, (double)0));
	l_recoilRollMat.setRotate(etOrientation(m_effectVelocity.z()/(double)MAX_EFFECT_DURATION, (double)0, (double)0, (double)1));

	// rotates z unit vector about y-axis
	m_recoilDirection = l_recoilPitchMat*m_recoilDirection;
	m_recoilDirection = l_recoilYawMat*m_recoilDirection;
	m_recoilDirection = l_recoilRollMat*m_recoilDirection;

	//gun weight
	//l_force = a_staticWt;

	double l_ratio = computeForceRatio(m_effectRampUp);

	l_force = m_recoilDirection.scale(l_ratio*10.0); //PISTOL_RECOIL_MAG); // 10

	return(l_force);
}

const bool bLowerThreshold = false;
const bool bUpperThreshold = true;

etVector scaleAxisAbs(double x, double y, double z, bool bThresholdType)
{
	double l_x = 0.0;
	double l_y = 0.0;
	double l_z = 0.0;

	// x
	if(bThresholdType == bLowerThreshold)
	{
		// Less than minimum
		if ( x < hap_cb_left_start.GetFloat() )
		{
			l_x = ( hap_cb_left_start.GetFloat() - x );
			//l_x = ( x - hap_cb_left_start.GetFloat() ) / ( 1.0 - abs(hap_cb_left_start.GetFloat()) ); // should be neg
		}
		// Greater than maximum
		else if ( x > hap_cb_right_start.GetFloat() )
		{
			l_x = ( x - hap_cb_right_start.GetFloat() );
			//l_x = ( x - hap_cb_right_start.GetFloat() ) / ( 1.0 - abs(hap_cb_right_start.GetFloat()) ); // should be pos
		}
		else
			l_x = 0.0;
	}

	// y
	if(bThresholdType == bLowerThreshold)
	{
		// Less than minimum
		if ( y < hap_cb_bottom_start.GetFloat() )
		{
			l_y = ( hap_cb_bottom_start.GetFloat() - y );
			//l_y = ( y - hap_cb_bottom_start.GetFloat() ) / ( 1.0 - abs(hap_cb_bottom_start.GetFloat()) ); // should be neg
		}
		// Greater than maximum
		else if ( y > hap_cb_top_start.GetFloat() )
		{
			l_y = ( y - hap_cb_top_start.GetFloat() );
			//l_y = ( y - hap_cb_top_start.GetFloat() ) / ( 1.0 - abs(hap_cb_top_start.GetFloat()) ); // should be pos
		}
		else
			l_y = 0.0;
	}

	// z
	if(bThresholdType == bLowerThreshold)
	{
		// Less than minimum
		if ( z < hap_cb_far_start.GetFloat() )
		{
			l_z = ( hap_cb_far_start.GetFloat() - z );
			//l_z = ( z - hap_cb_far_start.GetFloat() ) / ( 1.0 - abs(hap_cb_far_start.GetFloat()) ); // should be neg
		}
		// Greater than maximum
		else if ( z > hap_cb_near_start.GetFloat() )
		{
			l_z = ( z - hap_cb_near_start.GetFloat() );
			//l_z = ( z - hap_cb_near_start.GetFloat() ) / ( 1.0 - abs(hap_cb_near_start.GetFloat()) ); // should be pos
		}
		else
			l_z = 0.0;
	}

	return etVector(l_x, l_y, l_z);
}

etVector hdlHaptics::calculateMovementModel()
{
	// Tool position
    //etVector l_posWC = m_terminusPositionServo;
	double l_posXAbs, l_posYAbs, l_posZAbs;
	getAbsolutePositionServo(l_posXAbs, l_posYAbs, l_posZAbs);
	double l_posX, l_posY, l_posZ;
	getRelativePositionServo(l_posX, l_posY, l_posZ);

#ifdef PSIMOD
	double l_forceX,l_forceY,l_forceZ;
	l_forceX=0;
	l_forceY=0;
	l_forceZ=0;
	C_HandPlayer *pPlayer = C_HandPlayer::GetLocalHandPlayer();
	//get relative to client settings
	if(l_posXAbs < 0)
		l_posX = l_posXAbs / abs(hap_cb_left_end.GetFloat());
	else
		l_posX = l_posXAbs / abs(hap_cb_right_end.GetFloat());
    if(l_posYAbs < 0)
		l_posY = l_posYAbs / abs(hap_cb_bottom_end.GetFloat());
	else
		l_posY = l_posYAbs / abs(hap_cb_top_end.GetFloat());
	if(l_posZAbs < 0)
	    l_posZ = l_posZAbs / abs(hap_cb_far_end.GetFloat());
	else
	    l_posZ = l_posZAbs / abs(hap_cb_near_end.GetFloat());

	if(pPlayer)
	{
		pPlayer->CalculateBoundingForce(l_posX, l_posY, l_posZ, l_forceX, l_forceY, l_forceZ);
	}
	return etVector(l_forceX,l_forceY,l_forceZ);
#else
	etVector l_relativePositionOuter;
	etVector l_relativePositionInner;

	l_relativePositionOuter = scaleAxisAbs(l_posXAbs, l_posYAbs, l_posZAbs, bLowerThreshold);

	etVector l_controlForceInner;
	etVector l_controlForceOuter;
	double l_forceX, l_forceY, l_forceZ;

	switch(hap_control_type_inner.GetInt())
	{
		case Constant:
			l_controlForceInner = etVector(0,0,0);
			break;
		case Linear:
			l_controlForceInner = l_relativePositionInner.scale(hap_cb_springk.GetFloat());
			break;
		default:
			l_controlForceInner = etVector(0,0,0);
			break;
	}

	switch(hap_control_type_outer.GetInt())
	{
		case Constant:
			l_controlForceOuter = etVector(0,0,0);
			break;
		case Linear:
			l_controlForceOuter = l_relativePositionOuter.scale(hap_cb_springk.GetFloat());
			break;
		default:
			l_controlForceOuter = etVector(0,0,0);
			break;
	}

	// Centering force
	if(l_posX > 0)
		l_forceX = (l_posXAbs / hap_cb_right_start.GetFloat() >= 1) ? -l_controlForceOuter.x() : 0.0;
	else if(l_posX < 0)
		l_forceX = (l_posXAbs / hap_cb_left_start.GetFloat() >= 1) ? l_controlForceOuter.x() :  0.0;
	else
		l_forceX = 0;

	if(l_posY > 0)
		l_forceY = (l_posYAbs / hap_cb_top_start.GetFloat() >= 1) ? -l_controlForceOuter.y() : 0.0;
	else if(l_posY < 0)
		l_forceY = (l_posYAbs / hap_cb_bottom_start.GetFloat() >= 1) ? l_controlForceOuter.y() :  0.0;
	else
		l_forceY = 0;

	if(l_posZ > 0)
		l_forceZ = (l_posZAbs / hap_cb_near_start.GetFloat() >= 1) ? -l_controlForceOuter.z() : 0.0;
	else if(l_posZ < 0)
		l_forceZ = (l_posZAbs / hap_cb_far_start.GetFloat() >= 1) ? l_controlForceOuter.z() :  0.0;
	else
		l_forceZ = 0;

	return etVector(l_forceX, l_forceY, l_forceZ);
#endif
}

etVector hdlHaptics::calculateVehicleModel()
{
	// Tool position
	double l_posX, l_posY, l_posZ;
	getAbsolutePositionServo(l_posX, l_posY, l_posZ);
	double l_forceX, l_forceY, l_forceZ;

	//Pos/Neg X box forces
	if(l_posX < hap_cbvehicle_left_start.GetFloat())
		l_forceX = (hap_cbvehicle_left_start.GetFloat()-l_posX)*hap_cbvehicle_springk.GetFloat();
	else if(l_posX > hap_cbvehicle_right_start.GetFloat())
		l_forceX = (hap_cbvehicle_right_start.GetFloat()-l_posX)*hap_cbvehicle_springk.GetFloat();
	else
		l_forceX = 0;

	//Pos/Neg Y box forces
	if(l_posY < hap_cbvehicle_bottom_start.GetFloat())
		l_forceY = (hap_cbvehicle_bottom_start.GetFloat()-l_posY)*hap_cbvehicle_springk.GetFloat();
	else if(l_posY > hap_cbvehicle_top_start.GetFloat())
		l_forceY = (hap_cbvehicle_top_start.GetFloat()-l_posY)*hap_cbvehicle_springk.GetFloat();
	else
		l_forceY = 0;

	//Pos/Neg Z box forces
	if(l_posZ < hap_cbvehicle_far_start.GetFloat())
		l_forceZ = (hap_cbvehicle_far_start.GetFloat()-l_posZ)*hap_cbvehicle_springk.GetFloat();
	else if(l_posZ > hap_cbvehicle_near_start.GetFloat())
		l_forceZ = (hap_cbvehicle_near_start.GetFloat()-l_posZ)*hap_cbvehicle_springk.GetFloat();
	else
		l_forceZ = 0;
	
	return etVector(l_forceX,l_forceY,l_forceZ);
}

extern ConVar hap_vehicle_constant;
extern ConVar hap_vehicle_shape;

double hdlHaptics::computeExpModel(double start, double end)
{
	double scale;

	float fShapeFunc = hap_vehicle_shape.GetFloat();
	
	// linear model first
	if(start > end)
		scale = ((start - end) * computeForceRatioExp(0.0, fShapeFunc)) + end;
	else if(start < end)
		scale = ((end - start) * computeForceRatioExp(1.0, fShapeFunc)) + start;
	else
		scale = start; // 1.0

	return scale;
}

//BA CES07
void hdlHaptics::terminateEffect(EffectType a_effectID)
{
	if(a_effectID == Carry)
	{
		CarryEffect::instance()->dropObject();
	}
}

static VehicleEffect *prevVehicleEffect = 0;
static etVector tempEffect(0,0,0);

// handles sending effects to device and effect list.
void hdlHaptics::triggerEffect(EffectType a_effectID, float a_effectAmplitude,
		float a_x, float a_y, float a_z, int a_effectDuration, float a_effectRampUp, 
		float b_x, float b_y, float b_z)
{
		m_triggerEffectClient = true;
		
		//force vector components
		double x;
		double y;
		double z;
		switch(a_effectID)
		{
		case Recoil:
			{
				x = 0;
				y = 0;
				z = (double)a_effectAmplitude;
				RecoilEffect* l_effect = new RecoilEffect(x,y,z,a_effectDuration,a_effectRampUp);
				l_effect->start();
				addEffect(l_effect);
			}
			break;
		case Damage:
			{
				x = (double)(a_x*a_effectAmplitude);
				y = (double)(a_y*a_effectAmplitude);
				z = (double)(a_z*a_effectAmplitude);
				DamageEffect* l_effect = new DamageEffect(x,y,z,a_effectDuration,a_effectRampUp);
				l_effect->start();
				addEffect(l_effect);
			}
			break;
		case Footsteps:
			{
				x = 0;
				y = (double)(a_y*a_effectAmplitude);
				z = 0;
				FootstepsEffect* l_effect = new FootstepsEffect(x,y,z,a_effectDuration,a_effectRampUp);
				l_effect->start();
				addEffect(l_effect);
			}
			break;
		case Carry:
			{
				CarryEffect* l_effect = CarryEffect::create();
				l_effect->pickUpObject();
				l_effect->setObjectMass(a_effectAmplitude);
				addEffect(l_effect);
			}
			break;
		case Vehicle:
			x = (double)(a_x*a_effectAmplitude);
			y = (double)(a_y*a_effectAmplitude);
			z = (double)(a_z*a_effectAmplitude);

			// WARNING: POTENTIALLY HACKISH!!!
			tempEffect = etVector(x, y, z);
			// End HACKISH
			break;
		case Avatar:
			{

				m_mutex.Lock();
				x = (double)(a_x*a_effectAmplitude);
				y = (double)(a_y*a_effectAmplitude);
				z = (double)(a_z*a_effectAmplitude);
				etVector old = etVector();
				if(m_avatarEffect)
				{
					old = m_avatarEffect->getForce();
					// remove old from effect list.
					delete m_avatarEffect;
					m_avatarEffect = NULL;
				}
				m_avatarEffect = new AvatarEffect(x,y,z,old.x(),old.y(),old.z(),10);
				m_mutex.Unlock();
				
			}
			break;
		default:
			Warning("Effect sent to haptics with unknown id of %i\n",a_effectID);
			m_triggerEffectClient=false;
			break;
		}
}

//BA CES07
etVector hdlHaptics::calculateForces() // req
{
	// Stiffness
    double l_stiffness = 1.0 * DEFAULT_STIFFNESS;
	etVector l_force(0,0,0); 

	
	if(!InUIMode())
	{
		// Force effects list
		//etVector l_effectForce;
		for(int i = 0; i < (int)m_effectList.size(); i++)
		{
			//if(m_effectList[i]->isStillActive())
				l_force += m_effectList[i]->getForce();
		}
		
		m_mutex.Lock();
		if(m_avatarEffect && m_avatarEffect->isValid())
		{
			etVector l_avatar = m_avatarEffect->getForce();
			if(l_avatar.isValid())
				l_force += l_avatar;
			else{
				delete m_avatarEffect;
				m_avatarEffect=NULL;
			}
		}
		m_mutex.Unlock();
		
		// WARNING: POTENTIALLY HACKISH! (for vehicle, this line only)
		l_force += tempEffect;

		cleanupEffectList();

		// Control box (movement model)
		if(C_BasePlayer::GetLocalPlayer()&&C_BasePlayer::GetLocalPlayer()->IsInAVehicle())
			l_force +=	calculateVehicleModel().scale(hap_cbvehicle_scale.GetFloat());
		else
			l_force += calculateMovementModel().scale(hap_cb_scale.GetFloat());

	}
	
	m_lastForces = l_force.scale(l_stiffness);
	// return
	return m_lastForces;
}

void hdlHaptics::addEffect(HapticEffect* a_effect)
{
    // If the old tmep effect hasn't been moved then remove it.
    if (m_tempEffect) delete m_tempEffect;

    // wait for next sync before adding to list.
    m_tempEffect = a_effect;
}

// Remove expired effects.
void hdlHaptics::cleanupEffectList()
{
    EffectListType::iterator l_iter = m_effectList.begin();
    while(l_iter != m_effectList.end())
    {
        if (!(*l_iter)->isStillActive())
        {
            delete (*l_iter);
            l_iter = m_effectList.erase(l_iter);
        } 
        else
            l_iter++;
    }
}

void hdlHaptics::initParams()
{
    g_deviceType = hdlDeviceModel();
	m_deviceName = g_deviceType;

    // Defaults for each known device type
    if ( strcmp( g_deviceType, "PHANTOM_DESKTOP" ) == 0 )
    {
        DEFAULT_SCALE = 2.0;
        DEFAULT_STIFFNESS = 1.0;
        EFFECT_STIFFNESS = 0.95;
        MAX_EFFECT_DURATION = 500;
        //PISTOL_WEIGHT = 1.0;
        //PISTOL_RECOIL_MAG = 10.0;
        //PISTOL_RECOIL_DURATION = 200;
        //IS_FALCON = 0;

		HAPTIC_WORKSPACE = 150.0;
		GAMESPACE_DIMENSION = 30.0;

		POSITION_EXTENTS[0] = 0.22;
		POSITION_EXTENTS[1] = 0.10;
		POSITION_EXTENTS[2] = 0.09;
		POSITION_EXTENTS[3] = 0.22;
		POSITION_EXTENTS[4] = 0.17;
		POSITION_EXTENTS[5] = 0.13;
    }

    if ( strcmp( g_deviceType, "PHANTOM_OMNI" ) == 0 )
    {
        DEFAULT_SCALE = 2.0;
        DEFAULT_STIFFNESS = 1.0;
        EFFECT_STIFFNESS = 0.95;
        MAX_EFFECT_DURATION = 500;
        //PISTOL_WEIGHT = 1.0;
        //PISTOL_RECOIL_MAG = 10.0;
        //PISTOL_RECOIL_DURATION = 200;
        //IS_FALCON = 0;

		HAPTIC_WORKSPACE = 150.0;
		GAMESPACE_DIMENSION = 30.0;

		POSITION_EXTENTS[0] = 0.18;
		POSITION_EXTENTS[1] = 0.10;
		POSITION_EXTENTS[2] = 0.07;
		POSITION_EXTENTS[3] = 0.18;
		POSITION_EXTENTS[4] = 0.19;
		POSITION_EXTENTS[5] = 0.09;
	}

    if ( strcmp( g_deviceType, "FALCON_ALPHA" ) == 0 )
    {
        DEFAULT_SCALE = 2.0;
        DEFAULT_STIFFNESS = 2.0;
        EFFECT_STIFFNESS = 0.95;
        MAX_EFFECT_DURATION = 500;
        //PISTOL_WEIGHT = 3.0;
        //PISTOL_RECOIL_MAG = 30.0;
        //PISTOL_RECOIL_DURATION = 200;
        //IS_FALCON = 1;

		HAPTIC_WORKSPACE = 150.0;
		GAMESPACE_DIMENSION = 30.0;

		// Haven't tested values for these, please update
		POSITION_EXTENTS[0] = 0.27;
		POSITION_EXTENTS[1] = 0.27;
		POSITION_EXTENTS[2] = 0.18;
		POSITION_EXTENTS[3] = 0.29;
		POSITION_EXTENTS[4] = 0.26;
		POSITION_EXTENTS[5] = 0.25;
    }
}

