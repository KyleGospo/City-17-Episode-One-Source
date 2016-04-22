// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// orientation.h -- contains the etOrientation class.
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef ET_ORIENTATION_H
#define ET_ORIENTATION_H

// Includes
#include "haptics_vector.h"

// Class:
//     etOrientation
//
// Description:
//     An orientation defined by an Axis and an Angle
//     Angle is in radians, and the Axis is not normalised.     
//     Components cannot be set individually.
//
//     Note that the result of concatenating two Orientations is 
//     order dependent, ie A+B != B+A
//
// Exceptions:
//     None
//
// Notes:
//
// See Also Classes:
//     None
//
// See Also Functions:
//     None

class etOrientation
{
    public:
        static const double PI;

        // Constructor, specifies angle, and x,y,z of axis
        inline etOrientation(double a = 0, double x = 1,
                             double y = 0, double z = 0)
            : m_angle(a), m_axis(x, y, z)
        {
            m_axis = m_axis.normalize();
        }

        // Destructor
        inline ~etOrientation() {}

        // Copy constructor
        inline etOrientation(const etOrientation& other)
            : m_angle(other.m_angle), m_axis(other.m_axis) {}

        // Assignment operator
        inline etOrientation& operator=(const etOrientation& other)
        {
            m_angle = other.m_angle;
            m_axis = other.m_axis;
            return *this;
        }

        // Return the angle in radians
        inline double angle() const { return m_angle; }

        // Return the angle in degrees
        inline double angleDegrees() const { return (m_angle / PI) * 180.0; }

        // Return the axis
        inline etVector axis() const { return m_axis; }

        // Return the x component of the axis
        inline double x() const { return m_axis.x(); }

        // Return the y component of the axis
        inline double y() const { return m_axis.y(); }

        // Return the z component of the axis
        inline double z() const { return m_axis.z(); }

        // Returnss the inverse of this orientation.
        inline etOrientation operator-() const
        {
            return etOrientation(-angle(), x(), y(), z());
        }

        // Subtract other from this
        // This is order dependent ,see operator+
        inline etOrientation operator-(const etOrientation& other) const
        {
            return *this + (-other);
        }

        // Returns this result of concatenating this and other
        // Note that the result of concatenating two rotations is 
        // order dependent, ie A+B != B+A        
        etOrientation operator+(const etOrientation& other) const;

        // Scale the angle
        inline etOrientation scale(double a_factor) const
        {
            return etOrientation(angle() * a_factor, x(), y(), z());
        }

    private:
        double m_angle;
        etVector m_axis;
};

// Comparison
inline bool operator==(const etOrientation& a, const etOrientation& b)
{
    return (a.angle() == b.angle()) && (a.axis() == b.axis());
}

// Comparison
inline bool operator!=(const etOrientation& a, const etOrientation& b)
{
    return !(a == b);
}

#endif
