// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef AVATAR_EFFECT_H
#define AVATAR_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"
#include "haptics_effect.h"

// Class:
//     AvatarEffect
//
// Description:
//    Force effect for avatar acceleration forces.
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

class AvatarEffect : public HapticEffect
{

public:

    // Constructor
    AvatarEffect()
    {
        m_direction[0] = m_direction[1] = etVector(0,0,0);
    }

    // Constructor
    AvatarEffect(double new_x, double new_y, double new_z, double old_x, double old_y, double old_z, int smoothtime )
    {
		m_direction[0] = etVector(old_x,old_y,old_z);
		m_direction[1] = etVector(new_x,new_y,new_z);
		m_smooth_duration = smoothtime;
		m_timer = smoothtime-1;
    }
   
    etVector getForce()
    {
		// if our timer isnt done..
		if(m_timer>0)
		{
			//lerp by smoothtime
			m_timer--;
			if(!m_direction[0].isValid()||!m_direction[1].isValid())
			{
				return etVector();
			}
			return etVector( m_direction[0] + etVector(m_direction[1]-m_direction[0]).scale(getInterpValue()) );
		}
		//otherwise just return the final direction.
		return m_direction[1];
    }

	float getInterpValue()
	{
		// shouldnt be called unless timer is above zero (adds one due to prior subtraction)
		return 1.0f - (float)(m_timer+1)/(float)m_smooth_duration;
	}

    virtual bool isStillActive()
    {
		//active untill deleted externaly.
        return true;//m_timer > 0;
    }
	virtual bool isValid()
	{
		//return m_timer > 0;
		return (m_direction[0].isValid()&&m_direction[1].isValid()&&m_direction[0].length()!=m_direction[1].length());
	}

private: 
   etVector m_direction[2];
   int m_timer;
   int m_smooth_duration;
};
#endif