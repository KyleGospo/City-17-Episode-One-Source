// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// matrix.h -- contains the etMatrix class.
//
// $Revision: #1 $
// $Change: 6627 $

#ifndef ET_MATRIX_H
#define ET_MATRIX_H

#include "haptics_vector.h"
#include "haptics_orientation.h"

// Class:
//     etMatrix
//
// Description:
//     A data structure representing a 4x4 Matrix.
//     The matrix is stored internally in Row-Column order
//     Methods are provided for scaling, translating and 
//     rotating the matrix.
//
// Exceptions:
//     None
//
// Notes:
//     Row & Column addressing for the array operators is done by
//     returning an intermediate object that also provides an array
//     operator.  Use matrix[row][column] for access to elements of
//     the matrix.
//
//     Inverse assumes that this is an affine matrix, which will
//     be the case if the matrix has only been modified by it's 
//     methods.
//
//     The matrix coords a are stored in Column,Row order to match 
//     the ordering of OpenGL matrices.
//
//      getTranslation, getScale and getRotation are used to
//      decompose a matrix into a single rotation, translation,
//      and scaling. The components are applied in this order:
//          original matrix = (translation)(scale)(rotation)
//          
//      i.e. apply rotation first, then scale, then translation.
//
//      The results of these get functions will only be defined if
//      the matrix has been created by any number or sequence of 
//      rotations, translations and uniform scalings 
//      (i.e. scale(1,1,2) is not allowed, while scale(2,2,2) is allowed)
//      
//
// See Also Classes:
//     etVector, etOrientation
//
// See Also Functions:
//     None

class etMatrix
{
    public:
        // Default constructor.
        inline etMatrix()
        {
            setIdentity();
        }

        etMatrix(double* array16);

        // RowAccess is an intermediate class used to allow two index addressing.
        class RowAccess;

        // RowAccessConst is an intermediate class used to allow two index addressing.
        class RowAccessConst;

        // Use default destructor, copy, assignment.

        // Sets the matrix to the identity matrix.
        void setIdentity();

        // Post multiplies the matrix by a_rotation.
        void rotate(const etOrientation& a_rotation);

        // Post multiplies the matrix by a_translation.
        void translate(const etVector& a_translation);

        // Post multiplies the matrix by a_scale.
        void scale(const etVector& a_scale);

        // Sets the matrix to the rotation matrix formed by a_rotation.
        void setRotate(const etOrientation& a_rotation);

        // Sets the matrix to the translation matrix formed by a_translation.
        void setTranslate(const etVector& a_translation);

        // Sets the matrix to the scale matrix formed by a_scale.
        void setScale(const etVector& a_scale);

        // Assignment operator
        etMatrix& operator=(const etMatrix& other);

        // Returns the product of this and other.
        etMatrix operator*(const etMatrix& other) const;

        // Multiplies this by other.
        etMatrix& operator*=(const etMatrix& other);

        // Returns the product of this and a_vec.
        etVector operator*(const etVector& a_vec) const;

        // Returns Row[a_index].  Use matrixName[row][column] to
        // return matrix elements.
        RowAccess operator[](int a_index);

        // Returns Row[a_index].  Use matrixName[row][column] to
        // return matrix elements.
        const RowAccessConst operator[](int a_index) const;

        // Returns the array of matrix values.
        inline const double* getArray() const { return m_matrix; }

        // Matrix multiplication without an extra array copy.
        static void multiply(etMatrix& a_result,
                                 const etMatrix& a_matrixA,
                                 const etMatrix& a_matrixB);

        // Returns this Matrix's inverse.
        etMatrix inverse();
 
        // Decompose the Matrix into components and return the rotation.
        // See notes.
        etOrientation getRotate() const;

        // Decompose the Matrix into components and return the translation.
        // See notes.
        etVector getTranslate() const;

        // Decompose the Matrix into components and return the scale.
        // See notes.
        etVector getScale() const;

    private:
        // Internal
        //
        // Return the determinant of the top left 3x3 sub-matrix within this matrix.        
        double determinant3x3();

        // Internal
        //
        // Return the determinant of a 2x2 sub-matrix within this matrix.
        // Specified by row1, row2, col1, col2.
        inline double determinant2x2(int a_row1, int a_row2, int a_col1, int a_col2);

        double m_matrix[16];
};

// RowAccess is an intermediate class used to allow two index addressing.   
class etMatrix::RowAccess
{
    public:
        // Column operator.
        double& operator[](int i);
    
    private:
        friend class etMatrix;

        // Constructor only accessible from etMatrix.
        RowAccess(double* a_rowStart);

        // The start position of the row
        double* m_rowStart;
};

// RowAccessConst is an intermediate class used to allow two index addressing.
class etMatrix::RowAccessConst
{
    public:
        // Column operator.
        const double& operator[](int i) const;

    private: 
        friend class etMatrix;

        // Constructor only accessible from etMatrix.
        RowAccessConst(const double* a_rowStart);

        // The start position of the row
        const double* m_rowStart;
};

// Return a 2x2 determinant.
inline double etMatrix::determinant2x2(int a_row1, int a_row2, int a_col1, int a_col2)
{
    // Performance optimization:
    int l_row1 = a_row1 * 4;
    int l_row2 = a_row2 * 4;
    return (m_matrix[l_row1 + a_col1] * m_matrix[l_row2 + a_col2] -
            m_matrix[l_row2 + a_col1] * m_matrix[l_row1 + a_col2]);

    // Original code:
    //      return (*this)[a_row1][a_col1] * (*this)[a_row2][a_col2] -
    //              (*this)[a_row2][a_col1] * (*this)[a_row1][a_col2];
}

// Comparison.
bool operator==(const etMatrix& a, const etMatrix& b);

// Comparison.
inline bool operator!=(const etMatrix& a, const etMatrix& b)
{
    return !(a == b);
}

#endif
