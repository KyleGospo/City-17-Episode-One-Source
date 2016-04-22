// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef DAMAGE_EFFECT_H
#define DAMAGE_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"
#include "haptics_effect.h"

// Class:
//     DamageEffect
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

class DamageEffect : public HapticEffect
{

public:

    // Constructor
    DamageEffect()
    {
        m_duration = 10;
		m_rampTime = 10;
        m_timer = 0;

        m_direction = etVector(0,0,0);
    }

    // Constructor
    DamageEffect(double _x, double _y, double _z, int a_duration, float a_rampTime)
    {
        m_duration = a_duration;
		m_rampTime = a_rampTime;
        m_timer = 0;
  
		m_direction = etVector(_x,_y,_z);
    }
   
    void start(){m_timer += m_duration;}

    etVector getForce()
    {
       if (!m_timer) return etVector();

        double l_force;
        if (m_timer < m_duration * 0.5)
            l_force = m_timer;
        else
            l_force = m_duration - m_timer;

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