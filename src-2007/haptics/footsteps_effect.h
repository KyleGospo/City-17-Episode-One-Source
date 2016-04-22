// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef FOOTSTEPS_EFFECT_H
#define FOOTSTEPS_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"
#include "haptics_effect.h"

// Class:
//     FootstepsEffect
//
// Description:
//    Force effect for walking and jumping.
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

class FootstepsEffect : public HapticEffect
{

public:

    // Constructor
    FootstepsEffect()
    {
        m_duration = 10;
		m_rampTime = 10;
        m_timer = 0;

        m_direction = etVector(0,0,0);
    }

    // Constructor
    FootstepsEffect(double _x, double _y, double _z, int a_duration, float a_rampTime)
    {
        m_duration = a_duration;
		m_rampTime = a_rampTime;
        m_timer = 0;
  
		m_direction = etVector(_x,_y,_z);
    }
   
    void start(){m_timer += m_duration;}

	double computeForceRatio()
	{
		int l_half = (int)((double)(m_duration) * (1.0 - m_rampTime));
		double l_ratio=0.0;

		// First half of force simulation (from 0% -> 100%)
		if(m_timer >= l_half)
		{
			l_ratio = ((double)m_duration - (double)m_timer)/((double)m_duration - (double)l_half);
		}
		// Second half of force simulation (from 100% -> 0%)
		else
		{
			l_ratio = (double)m_timer / (double)l_half;
		}

		return l_ratio;
	}

    etVector getForce()
    {
       if (!m_timer) return etVector();

        double l_force = computeForceRatio();

        m_timer--;
        return m_direction.scale(l_force);
    }

    virtual bool isStillActive()
    {
        return m_timer > 0;
    }

private: 
   etVector m_direction;
   int m_duration;
   float m_rampTime;

   double m_maxForce;
   double m_scale;   
   int m_timer;
};
#endif