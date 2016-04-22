// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// vector.cpp -- contains the etVector class implementation.
//
// $Revision: #1 $
// $Change: 6627 $

#include "cbase.h"

#include "haptics_common.h"

#include "haptics_vector.h"
//#include <math.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

double etVector::dot(const etVector& other) const
{
    double l_x = x() * other.x();
    double l_y = y() * other.y();
    double l_z = z() * other.z();
    return l_x + l_y + l_z;
}

etVector etVector::cross(const etVector& other) const
{
    double l_x = y() * other.z() - z() * other.y();
    double l_y = z() * other.x() - x() * other.z();
    double l_z = x() * other.y() - y() * other.x();
    return etVector(l_x, l_y, l_z);
}

double etVector::length() const
{
    return sqrt(dot(*this));
}

etVector etVector::scale(double a_factor) const
{
    double l_x = x() * a_factor;
    double l_y = y() * a_factor;
    double l_z = z() * a_factor;
    return etVector(l_x, l_y, l_z);
}

etVector etVector::operator+(const etVector& other) const
{
    double l_x = x() + other.x();
    double l_y = y() + other.y();
    double l_z = z() + other.z();
    return etVector(l_x, l_y, l_z);
}

etVector& etVector::operator+=(const etVector& other)
{
    m_x += other.x();
    m_y += other.y();
    m_z += other.z();
    return *this;
}

etVector etVector::operator-(const etVector& other) const
{
    double l_x = x() - other.x();
    double l_y = y() - other.y();
    double l_z = z() - other.z();
    return etVector(l_x, l_y, l_z);
}

etVector etVector::operator-() const
{
    return etVector(-x(), -y(), -z());
}

etVector etVector::operator/(const etVector& other) const
{
    double l_x = x() / other.x();
    double l_y = y() / other.y();
    double l_z = z() / other.z();
    return etVector(l_x, l_y, l_z);
}

etVector& etVector::operator-=(const etVector& other)
{
    m_x -= other.x();
    m_y -= other.y();
    m_z -= other.z();
    return *this;
}
