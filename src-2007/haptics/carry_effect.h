// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// $Revision: #2 $
// $Change: 6634 $

#ifndef CARRY_EFFECT_H
#define CARRY_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"
#include "haptics_effect.h"
#include "haptics_main.h"

//#include "windows.h"
#include "cbase.h"

static CFastTimer gtToggleTimer;

ConVar hap_carry_anchor_scale("hap_carry_anchor_scale", "1.0");
ConVar hap_carry_gravity_scale("hap_carry_gravity_scale", "1.0");
ConVar hap_carry_anchor_time("hap_carry_anchor_time", "1.0");
ConVar hap_carry_anchor_mass("hap_carry_anchor_mass", "1.0");

// Class:
//     CarryEffect
//
// Description:
//    Force effect for taking enemy damage.
//
// Exceptions:
//     None
//
// Notes:

//
// See Also Classes:
//     etVector, etOrientation
//
// See Also Functions:
//     None

#if 0
namespace 
{
double getSystemTime()
{
    static double s_wavelength = 0.0;
    static double s_wavelength_x_high = 0.0;
    static bool s_isFirstTime = true;

    if (s_isFirstTime)
    {
        LARGE_INTEGER l_frequency = { 0 };
        ::QueryPerformanceFrequency(&l_frequency);
        s_wavelength = 1.0 / double(l_frequency.LowPart);
        s_wavelength_x_high = s_wavelength * double(MAXDWORD);
        s_isFirstTime = false;
    }

    LARGE_INTEGER l_ticks = { 0 };
    ::QueryPerformanceCounter(&l_ticks);

    return s_wavelength_x_high * double(l_ticks.HighPart) +
        s_wavelength * double(l_ticks.LowPart);
}
};
#endif

class CarryEffect : public HapticEffect
{

public:

	static CarryEffect* instance()
	{
		return s_instance;
	}
    static CarryEffect* create()
	{
		assert(!s_instance);
		s_instance = new CarryEffect();
		return s_instance;
	}
	static void destroy()
	{
		assert(s_instance);
		delete s_instance;
		s_instance = NULL;
	}

    // Constructor
    CarryEffect()
	{
		m_objectMass = 0;
		//m_lastTime = getSystemTime();
		m_currTime = 0;
		m_lastTime = 0;
		//gtToggleTimer.Start();
		m_anchorPos = getDevicePos();
		m_anchorVelocity = etVector(0,0,0);
	}


	void setObjectMass(double a_mass) {m_objectMass = a_mass;}

    void pickUpObject(){m_isHoldingObject = true;}
    void dropObject(){m_isHoldingObject = false;}

	etVector getDevicePos()
	{
		double l_posXAbs, l_posYAbs, l_posZAbs;
		hdlHaptics::instance()->getAbsolutePositionServo(l_posXAbs, l_posYAbs, l_posZAbs);

		return etVector(l_posXAbs,l_posYAbs,l_posZAbs);
	}

	etVector anchorSpringForce()
	{
		//double currTime = getSystemTime();
		//m_currTime = getSystemTime();
#if 0
		if(gtToggleTimer.GetDuration().GetMilliseconds() >= 1)
		{
			gtToggleTimer.End();
			m_currTime = gtToggleTimer.GetDuration().GetMilliseconds();
			gtToggleTimer.Start();
		}
#endif

		double elapsedTime = m_currTime - m_lastTime;
Msg("Elapsed Time: %f\n", elapsedTime);
		m_lastTime = m_currTime;

		etVector l_anchorForce(0,0,0);

		//get the device position
		etVector l_currDevPos = getDevicePos();
Msg("Device: X = %f,Y = %f,Z = %f\n", l_currDevPos.x(),l_currDevPos.y(),l_currDevPos.z());

		//velocity of the tool
		m_objectVelocity = m_anchorVelocity;

		etVector forceDir = m_anchorPos - l_currDevPos;
		l_anchorForce = forceDir.scale(hap_carry_anchor_mass.GetFloat());
Msg("Mass: %f\n", l_anchorForce.length());

		etVector gravity(0,-m_objectMass,0);
		l_anchorForce += gravity.scale(hap_carry_gravity_scale.GetFloat());

		// acceleration of anchor based on force applied to user
		// moves anchor towards tool position
		etVector anchorAcceleration = (l_anchorForce.length() > 0) ? 
			l_anchorForce.scale(-1/m_objectMass) : etVector(0,0,0);

		m_anchorVelocity += anchorAcceleration.scale(elapsedTime*hap_carry_anchor_time.GetFloat());
		m_anchorPos += m_anchorVelocity.scale(elapsedTime*hap_carry_anchor_time.GetFloat());

		return l_anchorForce;
	}

    etVector getForce()
    {
		if (!m_objectMass) return etVector();

		etVector l_force(0,0,0);
		l_force = etVector(0,-m_objectMass,0).scale(hap_carry_gravity_scale.GetFloat());

		//l_force = anchorSpringForce().scale(hap_carry_anchor_scale.GetFloat());

		//Msg("Holding Object = %+0.2d, weight = %+0.2f,\n",m_isHoldingObject,m_objectMass);

		return l_force;
    }

    virtual bool isStillActive()
    {
        return m_isHoldingObject;
    }

	static CarryEffect* s_instance;

private: 

	double m_objectMass;
	bool m_isHoldingObject;

	//anchor spring force
	double m_lastTime;
	double m_currTime;
	etVector m_anchorPos;
	etVector m_objectPos;
	etVector m_anchorVelocity;
	etVector m_objectVelocity;


	double m_maxForce;
};
#endif


