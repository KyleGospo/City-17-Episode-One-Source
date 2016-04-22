// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// quaternion.h -- contains the etQuaternion class.
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef ET_QUATERNION_H
#define ET_QUATERNION_H

#include "haptics_vector.h"
#include "haptics_orientation.h"
#include "haptics_matrix.h"

// Class:
//     etQuaternion
//
// Description:
//     A a quaternion, is stored as a scalar and a vector.
//     The constructor takes a rotation and produces a 
//     normalised quaternion.
// 
//     All functions assume that the Quaternion is normalised. 
//     Conversions to etOrientation and etMatrix are provided.
//
//     Note that Quaternion multiplication is not commutative
//     ie A*B != B*A
//
// Exceptions:
//     None
//
// Notes:
//
// See Also Classes:
//     etOrientation
//     etMatrix
//
// See Also Functions:
//     None

class etQuaternion
{
    public:
        // Construct from etOrientation.
        explicit etQuaternion(const etOrientation& a_rot);

        // Use default copy constructor, destructor, and assignment operator.

        // Concatenate this and other.
        // Note that Quaternion multiplication is not commutative
        // ie A*B != B*A
        etQuaternion operator*(const etQuaternion& other) const;

        // Concatenate this and other, and store the result in this
        // Note that Quaternion multiplication is not commutative
        // ie the result of A *= B doesn't equal the result of B *= A
        etQuaternion& operator*=(const etQuaternion& other);

        // Returns an etOrientation equivalent to this quaternion.
        etOrientation toOrientation() const;

        // Returns a rotation Matrix equivalent to this quaternion.
        etMatrix toMatrix() const;

        // Return scalar component.
        inline double s() const { return m_s; }

        // Return vector component.
        inline etVector v() const { return m_v; }

    private:
        // Hide the default Constructor.
        etQuaternion();

        // The scalar component
        double m_s;

        // The vector component
        etVector m_v;
};

// Comparison
inline bool operator==(const etQuaternion& a, const etQuaternion& b)
{
    return (a.s() == b.s()) && (a.v() == b.v());
}

// Comparison
inline bool operator!=(const etQuaternion& a, const etQuaternion& b)
{
    return !(a == b);
}

#endif
