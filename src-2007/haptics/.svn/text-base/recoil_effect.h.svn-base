// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef RECOIL_EFFECT_H
#define RECOIL_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"
#include "haptics_effect.h"

// Class:
//     RecoilEffect
//
// Description:
//    Force effect for a gun recoil.
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

class RecoilEffect : public HapticEffect
{

public:

    // Constructor
    RecoilEffect()
    {
        m_duration = 10;
		m_rampTime = 10;
        m_timer = 0;

        setDirection(etVector(0,0,10));
    }

    // Constructor
    RecoilEffect(double _x, double _y, double _z, int a_duration, float a_rampTime)
    {
        m_duration = a_duration;
		m_rampTime = a_rampTime;
        m_timer = 0;

    //  setDirection(etVector(0,0,_z));        
    //    setDirection(etVector(_x,_y,_z));   
		m_direction = etVector(_x,_y,_z);
    }

    void setDirection(etVector a_direction)
    {
        double l_length = a_direction.length();
        if (l_length) 
            m_direction = a_direction.scale(1.0 / l_length);

        m_maxForce = l_length;

        setScale();
    }

    void setDuration(int a_duration)
    {
        m_duration = a_duration;
        setScale();
    }
   
    void start(){m_timer += m_duration;}

	double computeForceRatio()
	{
		int l_half = (int)((double)(m_timer) * (1.0 - m_rampTime));
		double l_ratio=0.0;

		// First half of force simulation (from 0% -> 100%)
		if(m_timer >= l_half)
		{
			l_ratio = ((double)m_duration - (double)m_timer)/((double)m_timer - (double)l_half);
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
 #if 0
    //   if (!m_timer) return etVector();

        double l_force;
        if (m_timer < m_duration * 0.5)
            l_force = m_timer;
        else
            l_force = m_duration - m_timer;

      //  l_force *= m_scale;

        m_timer--;

		Msg("Recoil detected: Mag = %+0.2f, X = %+0.2f, Y = %+0.2f, Z = %+0.2f, Dur = %+0.2d, Ramp = %+0.2f\n",
		m_direction.length(),
		m_direction.x(),
		m_direction.y(),
		m_direction.z(),
		m_duration, 
		m_rampTime);

        return m_direction.scale(l_force);
#endif
		//start the recoil force
		etVector l_force(0,0,0);
		etMatrix l_recoilPitchMat, l_recoilYawMat, l_recoilRollMat;

		// should move these into the class...
		l_recoilPitchMat.setRotate(etOrientation(m_direction.x()/(double)m_duration, (double)1, (double)0, (double)0));
		l_recoilYawMat.setRotate(etOrientation(m_direction.y()/(double)m_duration, (double)0, (double)1, (double)0));
		l_recoilRollMat.setRotate(etOrientation(m_direction.z()/(double)m_duration, (double)0, (double)0, (double)1));

		// rotates z unit vector about y-axis
		m_direction = l_recoilPitchMat*m_direction;
		m_direction = l_recoilYawMat*m_direction;
		m_direction = l_recoilRollMat*m_direction;

		double l_ratio = computeForceRatio();

		l_force = m_direction.scale(l_ratio);

		m_timer--;

		return(l_force);
    }

    virtual bool isStillActive()
    {
        return m_timer > 0;
    }

private: 

    // Calculate the force scale
   void setScale()
   {
       if (m_duration != 0)
            m_scale = m_maxForce / m_duration;
       else 
           m_scale = 0;
   }
   
   etVector m_direction;
   int m_duration;
   float m_rampTime;

   double m_maxForce;
   double m_scale;   
   int m_timer;
};
#endif