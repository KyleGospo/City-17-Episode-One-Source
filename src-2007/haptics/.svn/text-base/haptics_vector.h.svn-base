// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// vector.h -- contains the etVector class.
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef ET_VECTOR_H
#define ET_VECTOR_H

// Class:
//     etVector
//
// Description:
//     A vector containing x,y,z components.
//     Most methods do not modify the contents of the vector, 
//     rather they return a modified vector.
//     Only the assignment and mixed operators (=, -=, +=)
//     modify the value of this vector.
//
// Exceptions:
//     None
//
// Notes:
//     None
//
// See Also Classes:
//     None
//
// See Also Functions:
//     None

class etVector
{
    public:
	    // Constructor, takes X,Y,Z values
        inline etVector(double x = 0, double y = 0, double z = 0)
            : m_x(x), m_y(y), m_z(z) {}

        // Use default copy constructor and assignment operator.

        // Destructor
        ~etVector() {}

        // Copy constuctor
        inline etVector(const etVector& other)
            : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}

        // Assignment operator
        inline etVector& operator=(const etVector& other)
        {
            m_x = other.m_x;
            m_y = other.m_y;
            m_z = other.m_z;
            return *this;
        }
        
	    // Returns the dot product of this and other
        double dot(const etVector& other) const;

	    // Returns the cross product of this and other
        etVector cross(const etVector& other) const;

	    // Returns this + other
	    etVector operator+(const etVector& other) const;

	    // Adds other to this
	    etVector& operator+=(const etVector& other);

	    // Returns this - other
	    etVector operator-(const etVector& other) const;

        // Negates all coordinates
        etVector operator-() const;

        // Returns a component-wise division of this by other
        etVector operator/(const etVector& other) const;
	
	    // Subtracts other from this
	    etVector& operator-=(const etVector& other);

	    // Returns the length of the vector
        double length() const;

	    // Return the normalized value of this vector
        inline etVector normalize() const { return scale(1/length()); }

	    // Scale vector by a_factor
        etVector scale(double a_factor) const;
    
	    // Return x component
        inline double x() const { return m_x; }
	
	    // Return y component
        inline double y() const { return m_y; }

	    // Return z component
        inline double z() const { return m_z; }
		inline bool isValid(){
			return (m_x==m_x&&m_y==m_y&&m_z==m_z);
		}

    private:
        double m_x;
        double m_y;
        double m_z;
};

// Comparison
inline bool operator==(const etVector& a, const etVector& b)
{
    return (a.x() == b.x()) && (a.y() == b.y()) && (a.z() == b.z());
}

// Comparison
inline bool operator!=(const etVector& a, const etVector& b)
{
    return !(a == b);
}

#endif
