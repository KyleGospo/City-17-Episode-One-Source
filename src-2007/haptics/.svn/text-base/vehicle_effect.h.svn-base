// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef VEHICLE_EFFECT_H
#define VEHICLE_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"
#include "haptics_effect.h"

// Class:
//     VehicleEffect
//
// Description:
//    Force effect for vehicle driving forces.
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

class VehicleEffect : public HapticEffect
{

public:

    // Constructor
    VehicleEffect()
    {
        m_direction = etVector(0,0,0);
		m_cancelForce = false;
	}

    // Constructor
    VehicleEffect(double _x, double _y, double _z)
    {
		m_timer = 1;
		m_direction = etVector(_x,_y,_z);
    }
   
    etVector getForce()
    {
		m_timer = 0;
        return m_direction;
    }

    virtual bool isStillActive()
    {
		return !m_cancelForce;
		//return m_timer;
	}

	void cancelForce()
	{
		m_cancelForce = true;
	}

private: 
   etVector m_direction;
   int m_timer;

	bool m_cancelForce;
};
#endif