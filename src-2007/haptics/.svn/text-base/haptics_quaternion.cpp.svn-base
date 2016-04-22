// -*-c++-*-
//
// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// quaternion.cpp -- contains the etQuaternion class implementation.
//
// $Revision: #1 $
// $Change: 6627 $
//

#include "cbase.h"

#include "haptics_common.h"
#include "haptics_quaternion.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

etQuaternion::etQuaternion(const etOrientation& a_rot)
{
    // Normalises a_rot.axis, which will result in a normalised Quaternion.
    m_s = cos((a_rot.angle()) /2);
    m_v = a_rot.axis().normalize().scale(sin((a_rot.angle()) / 2));
}

etQuaternion etQuaternion::operator*(const etQuaternion& other) const
{
    etQuaternion temp = *this;
    return temp *= other;
}

etQuaternion& etQuaternion::operator*=(const etQuaternion& other)
{
    double l_s = s()*other.s() - v().dot(other.v());
    etVector l_v =
	other.v().scale(s()) +
	v().scale(other.s()) +
        v().cross(other.v());

    m_s = l_s;
    m_v = l_v;

    return *this;
}

etOrientation etQuaternion::toOrientation() const
{
    double scale = v().length();
    double angle = 2 * ::acos( m_s );
    if (scale)
    {
        return etOrientation(angle,
			     m_v.x()/scale,
			     m_v.y()/scale,
			     m_v.z()/scale);
    }
    else
    {
        return etOrientation(0,1,0,0);
    }
}

etMatrix etQuaternion::toMatrix() const
{
    etMatrix l_result;

    l_result[0][0] = 1 - 2*v().y()*v().y() - 2*v().z()*v().z();
    l_result[1][0] = 2*v().x()*v().y() - 2*s()*v().z();
    l_result[2][0] = 2*v().x()*v().z() + 2*s()*v().y();
    l_result[3][0] = 0.0;

    l_result[0][1] = 2*v().x()*v().y() + 2*s()*v().z();
    l_result[1][1] = 1 - 2*v().x()*v().x() - 2*v().z()*v().z();
    l_result[2][1] = 2*v().y()*v().z() - 2*s()*v().x();
    l_result[3][1] = 0.0;

    l_result[0][2] = 2*v().x()*v().z() - 2*s()*v().y();
    l_result[1][2] = 2*v().y()*v().z() + 2*s()*v().x();
    l_result[2][2] = 1 - 2*v().x()*v().x() - 2*v().y()*v().y();
    l_result[3][2] = 0.0;

    l_result[0][3] = 0.0;
    l_result[1][3] = 0.0;
    l_result[2][3] = 0.0;
    l_result[3][3] = 1.0;

    return l_result;
}
