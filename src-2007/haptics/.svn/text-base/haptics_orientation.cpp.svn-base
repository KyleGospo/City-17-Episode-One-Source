// -*-c++-*-
//
// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// orientation.cpp -- contains the etOrientation class implementation.
//
// $Revision: #1 $
// $Change: 6627 $
//

#include "cbase.h"

#include "haptics_orientation.h"
#include "haptics_quaternion.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const double etOrientation::PI = 3.1415926535897932384626433832795;

etOrientation etOrientation::operator+(const etOrientation& other) const
{
    etQuaternion l_this(*this);
    etQuaternion l_other(other);
    etQuaternion l_result = l_this * l_other;
    return l_result.toOrientation();
}
