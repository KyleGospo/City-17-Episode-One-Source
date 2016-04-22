// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef HAPTIC_EFFECT_H
#define HAPTIC_EFFECT_H

// Class forward declaraions
#include "haptics_vector.h"

// Class:
//     HapticEffect
//
// Description:
//    Base class for effects
//
// Exceptions:
//     None
//
// Notes:

//
// See Also Classes:
//     etVector
//
// See Also Functions:
//     None

class HapticEffect
{
public:
    virtual etVector getForce() = 0;
    virtual bool isStillActive() = 0;
	virtual bool isValid(){return true;};
};
#endif