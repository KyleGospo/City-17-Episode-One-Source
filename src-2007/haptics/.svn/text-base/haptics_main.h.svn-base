// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//////////////////////////////////////////////////////////////////////
// haptics.h: interface for the haptics class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HAPTICS_MAIN_H
#define HAPTICS_MAIN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef _WIN32
#ifndef WIN32
#define WIN32 1
#endif // WIN32

#endif // _WIN32

#include <hdl/hdl.h>
#include <hdlu/hdlu.h>

#include "haptics_vector.h"
#include "haptics_matrix.h"

//BA CES07
#include <vector>
//For button history
#include <list>
#include "haptics_effect.h"

#include "convar.h"
#include "tier0/threadtools.h"

//static ConVar hap_var_effectstep_force ( "hap_var_effectstep_force", "0", FCVAR_REPLICATED );

enum EffectType	{Recoil = 1,
				Damage = 2,
				Footsteps = 3,
				Carry = 4,
				Vehicle = 5,
				Avatar = 6};

enum MovementAlgorithm	{Constant,
						Linear,
						Discrete,
						Exponential,
						Quadratic};

enum
{ MINX = 0, MINY, MINZ, MAXX, MAXY, MAXZ, NUM_COORDS };

#define		HAPTICS_EFFECTS_NONE		0
#define		HAPTICS_EFFECTS_CARRY		1
#define		HAPTICS_EFFECTS_RECOIL		2
#define		HAPTICS_EFFECTS_DAMAGE		4
#define		HAPTICS_EFFECTS_STEPS		8
#define		HAPTICS_EFFECTS_VEHICLE		16

// A few useful constants
const bool bBlocking = true;
const bool bNonBlocking = false;

// Haptics --------------------------------------------------------------------
// hdlHaptics is our interface to the hdl libraries. this class is refered to
// as the servo class as it handles all the motors and input sampling at a rate
// that is much faster then the game loop so that no input is missed and our
// effects do not get affected by the games process loop rate.
//
// the client(haptics) class should be the one that is actually sending the
// servo class effects and the servo class does all the processing tied to
// them (including output forces and removing the effects that are over.)
//
// Effects can be started here but for most controll over forces it is best to
// keep starting/queueing effects on the client(haptics) class.
//
//-----------------------------------------------------------------------------
class hdlHaptics  
{
    friend HDLServoOpExitCode ContactCB(void *data); // req
    friend HDLServoOpExitCode GetStateCB(void *pUsrData); // req

public:

	CThreadMutex m_mutex;
  
	// Called outside hdlHaptics (put this outside the class?)
	static hdlHaptics* instance(); // req
    static void create(); // req
    static void destroy(); // req
    void start(); // req
    void stop(); // req

	void initParams();

    // Destructor
    //virtual ~hdlHaptics();
    ~hdlHaptics();

    // Sync data returned by hapics object to the servo thread in 
    // a thread safe manner.  Call only once per application frame
    // (usually at the beginning of the graphics draw).
    void syncFromServo();

    // Returns the position of the haptic device terminus in millimeters.
    // If device has not been initialized, a position of 0,0,0 is returned
    // Note that 0,0,0 is at the center of the devices workspace
    // (i.e., its reset position)
    bool getDeviceTerminusPositionWC(double &px, double &py, double &pz);

    // Returns the position in absolute coords. via hdlToolPos function
    void getAbsolutePosition(double &px, double &py, double &pz);
    void getAbsolutePositionServo(double &px, double &py, double &pz);

	// Returns the position in relative coords., a percent of each axes extents
    void getRelativePosition(double &px, double &py, double &pz);
    void getRelativePositionServo(double &px, double &py, double &pz);

	// sets iButton to a recorded history of the device's buttons
    // if there is no history it returns false.
	bool getDeviceSwitchState(int &iButton );

	// gets last force vector sent
	etVector getLastForceSent();
    //
    // Game specific functions
    //
    void setFloorHeight(double a_height){m_floorHeightClient = a_height;}

	void setScaleFactor(double a_scale);
	void setControlType(int a_type);
	void setControlCube(double a_size);
	void setControlLengthInnerX(double a_lengthMin, double a_lengthMax);
	void setControlLengthInnerY(double a_lengthMin, double a_lengthMax);
	void setControlLengthInnerZ(double a_lengthMin, double a_lengthMax);
	void setControlCoefficientA(double a_force);
	void setControlCoefficientB(double a_force);
	void setControlCoefficientC(double a_force);
	void setControlDampeningInner(double a_dampening);
	void setControlDampeningOuter(double a_dampening);
	void setDamageFactor(double a_factor);
	//Temp BA
	void setControlBoxX(double a_lengthMin, double a_lengthMax);
	void setControlBoxY(double a_lengthMin, double a_lengthMax);
	void setControlBoxZ(double a_lengthMin, double a_lengthMax);
	void setControlBoxStiffness(double a_force);

	//BA CES07
	void terminateEffect(EffectType a_effectID);

	void triggerEffect(EffectType a_effectID, float a_effectAmplitude,
		float a_x, float a_y, float a_z, int a_effectDuration, float a_effectRampUp,
		float b_x = 0.0, float b_y = 0.0, float b_z = 0.0);

    // Servo loop only
	double computeForceRatio(double fRatio);
  	double computeForceRatioExp(double fRatio, double fShape);
	double computeExpModel(double start, double end);

	etVector computeRecoil();
	etVector calculateMovementModel();
	etVector calculateVehicleModel();

	etVector calculateForces();

    void doSyncOp();
    double m_deviceWorkspaceDim[6];
	const char *m_deviceName;
	
	//BA CES07
	void addEffect(HapticEffect* a_effect);


private:

    // Constructor
    hdlHaptics(); // req

    static hdlHaptics* s_instance; // req

    // Does first order haptic scene initialization.
    // Note that the haptic device is not  reset
    // or added to the scene.  addDevice() must be
    // called to do this.
    bool initialize(); // req

    double m_scale;                                         // Scale for haptic device
	
    // servo thead end-effector state
    etVector m_terminusPositionServo;
    etMatrix m_terminusTransformServo;
    int m_buttonStateServo;
    //double m_deviceWorkspaceDim[6];

    // client thread end-effector state
    etVector m_terminusPositionClient;
    etMatrix m_terminusTransformClient;
	//
	std::list<int>m_buttonHistoryClient;
    //int m_buttonStateClient;
    HDLOpHandle m_hContactCB;

    // Servo game vars
    double m_floorHeightServo;
    bool m_triggerEffectServo;
	etVector m_effectVelocity;
    bool m_isServoRunning;
	int m_recoilCounter;
	etVector m_recoilDirection;

	// Vehicle specific servo game vars
	int m_oldTime;
	int m_bExpUpdate;
	int m_nExpCycles;
	etVector m_oldForce;
	etVector m_vExpForceStart;
	etVector m_vExpForceEnd;

	float m_effectAmplitude;
 	int m_effectMaxDuration;
	int m_effectDuration;
	etVector m_effectVector;
  	float m_effectRampUp;

	//BA CES07
	HapticEffect* m_tempEffect;
    typedef  std::vector<HapticEffect*> EffectListType;
    EffectListType m_effectList;
	void cleanupEffectList();
	float tempTest;

//    int m_effectDuration;
//  etVector m_effectVector;
//	float m_effectAmplitude;
//	int m_effectMaxDuration;
//	float m_effectRampUp;

	double m_carryWeight;
	etVector m_vehicleForce;
	etVector m_avatarForce;
	HapticEffect* m_avatarEffect;

	// Effect variables
	bool m_doRecoilEffect;
	bool m_doPlayerShotEffect;
	bool m_doPlayerFootstepEffect;
	bool m_doPlayerCarryEffect;
	bool m_doVehicleForceEffect;
	bool m_doAvatarForceEffect;

    // Client game vars
    double m_floorHeightClient;
    bool m_triggerEffectClient;

    double m_gamespaceScale;
	etVector m_lastForces;
};

#endif // HAPTICS_MAIN_H
