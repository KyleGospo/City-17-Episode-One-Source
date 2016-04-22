// -*-c++-*-
//
// Copyright 2001 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// matrix.cpp -- contains the etMatrix implementation.
//
// $Revision: #1 $
// $Change: 6627 $
//

#include "cbase.h"

#include "haptics_common.h"
#include "haptics_matrix.h"
#include "haptics_vector.h"
#include "haptics_orientation.h"
#include <stdio.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const double PRECISION_LIMIT = (1.0e-15);

etMatrix::etMatrix(double *array16)
{
    for (int i = 0; i < 16; m_matrix[i] = array16[i++]);
}

void etMatrix::setIdentity()
{
    m_matrix[0] = 1; m_matrix[1] = 0; m_matrix[2] = 0; m_matrix[3] = 0;
    m_matrix[4] = 0; m_matrix[5] = 1; m_matrix[6] = 0; m_matrix[7] = 0;
    m_matrix[8] = 0; m_matrix[9] = 0; m_matrix[10] = 1; m_matrix[11] = 0;
    m_matrix[12] = 0; m_matrix[13] = 0; m_matrix[14] = 0; m_matrix[15] = 1;
}

void etMatrix::setRotate(const etOrientation& a_rotation)
{
    etVector axis = (a_rotation.axis().normalize());

    const double l_x = axis.x();
    const double l_y = axis.y();
    const double l_z = axis.z();

    const double l_cosA = ::cos(a_rotation.angle());
    const double l_sinA = ::sin(a_rotation.angle());

    m_matrix[0] = l_x * l_x + l_cosA * (1 - l_x * l_x);
    m_matrix[1] = l_x * l_y * (1 - l_cosA) + l_z * l_sinA;
    m_matrix[2] = l_z * l_x * (1 - l_cosA) - l_y * l_sinA;
    m_matrix[3] = 0;
    m_matrix[4] = l_x * l_y * (1 - l_cosA )- l_z * l_sinA;
    m_matrix[5] = l_y * l_y + l_cosA * (1 - l_y * l_y);
    m_matrix[6] = l_y * l_z * (1 - l_cosA) + l_x * l_sinA;
    m_matrix[7] = 0;
    m_matrix[8] = l_z * l_x * (1 - l_cosA) + l_y * l_sinA;
    m_matrix[9] = l_y * l_z * (1 - l_cosA) - l_x * l_sinA;
    m_matrix[10] = l_z * l_z + l_cosA * (1 - l_z * l_z);
    m_matrix[11] = 0;
    m_matrix[12] = 0;
    m_matrix[13] = 0;
    m_matrix[14] = 0;
    m_matrix[15] = 1;
}

void etMatrix::setTranslate(const etVector& a_translation)
{
    setIdentity();
    m_matrix[12] = a_translation.x();
    m_matrix[13] = a_translation.y();
    m_matrix[14] = a_translation.z();
}

void etMatrix::setScale(const etVector& a_scale)
{
    setIdentity();
    m_matrix[0] = a_scale.x();
    m_matrix[5] = a_scale.y();
    m_matrix[10] = a_scale.z();
}

void etMatrix::rotate(const etOrientation& a_rotation)
{
    etMatrix l_temp;
    l_temp.setRotate(a_rotation);

    *this *= l_temp;
}

void etMatrix::translate(const etVector& a_translation)
{
    for (int i = 0; i < 4; i++)
    {
        m_matrix[12 + i] =
            m_matrix[i] * a_translation.x() +
            m_matrix[i+4] * a_translation.y() +
            m_matrix[i+8] * a_translation.z() +
            m_matrix[i+12];
    }
}

void etMatrix::scale(const etVector& a_scale)
{
    for (int i = 0; i < 4; i += 1)
    {
        m_matrix[i] *= a_scale.x();
        m_matrix[i+4] *= a_scale.y();
        m_matrix[i+8] *= a_scale.z();
    }
}

void etMatrix::multiply(etMatrix& a_result,
                        const etMatrix& a_matrixA,
                        const etMatrix& a_matrixB)
{
    assert(&a_result != &a_matrixA);
    assert(&a_result != &a_matrixB);

    // Note, column major ordering, so we return At * Bt

    // Performance optimization:
    for(int l_row = 0; l_row < 16; l_row += 4)
    {
        for(int l_col = 0; l_col < 4; l_col++)
        {
            a_result.m_matrix[l_row + l_col] =
                a_matrixA.m_matrix[l_col] * a_matrixB.m_matrix[l_row] +
                a_matrixA.m_matrix[4 + l_col] * a_matrixB.m_matrix[l_row + 1] +
                a_matrixA.m_matrix[8 + l_col] * a_matrixB.m_matrix[l_row + 2] +
                a_matrixA.m_matrix[12 + l_col] * a_matrixB.m_matrix[l_row + 3];
        }
    }

    // Original code:
    //      for(int l_row = 0; l_row < 4; l_row ++)
    //      {
    //          for(int l_col = 0; l_col < 4; l_col++)
    //          {
    //              a_result[l_row][l_col] =
    //                  a_matrixA[0][l_col] * a_matrixB[l_row][0] +
    //                  a_matrixA[1][l_col] * a_matrixB[l_row][1] +
    //                  a_matrixA[2][l_col] * a_matrixB[l_row][2] +
    //                  a_matrixA[3][l_col] * a_matrixB[l_row][3];
    //          }
    //      }
}

// Assignment operator
etMatrix& etMatrix::operator=(const etMatrix& other)
{
    for (int i=0;i<16;m_matrix[i]=other.m_matrix[i++]);
    return *this;
}

etMatrix etMatrix::operator*(const etMatrix& other) const
{
    etMatrix result;

    multiply(result, *this, other);
    return result;
}

etMatrix& etMatrix::operator*=(const etMatrix& other)
{
    *this = *this * other;

    return *this;
}

etVector etMatrix::operator*(const etVector& a_vec) const
{
    double x = m_matrix[0] * a_vec.x()
        + m_matrix[4] * a_vec.y()
        + m_matrix[8] * a_vec.z()
        + m_matrix[12];

    double y = m_matrix[1] * a_vec.x()
        + m_matrix[5] * a_vec.y()
        + m_matrix[9] * a_vec.z()
        + m_matrix[13];

    double z = m_matrix[2] * a_vec.x()
        + m_matrix[6] * a_vec.y()
        + m_matrix[10] * a_vec.z()
        + m_matrix[14];

    return etVector(x, y, z);
}

bool operator==(const etMatrix& a, const etMatrix& b)
{
    // Performance optimization:
    const double* l_aArray = a.getArray();
    const double* l_bArray = b.getArray();
    for (int i = 0; i < 16; i++)
    {
        if (l_aArray[i] != l_bArray[i])
        {
            return false;
        }
    }

    // Original code:
    //      for(int row = 0; row < 4; row++)
    //      {
    //          for(int col = 0; col < 4; col++)
    //          {
    //              if (a[row][col] != b[row][col])
    //              {
    //                  return false;
    //              }
    //          }
    //      }

    return true;
}

etMatrix::RowAccess etMatrix::operator[](int a_index)
{
    assert(0 <= a_index && a_index < 4);

    return RowAccess(&(m_matrix[a_index * 4]));
}

const etMatrix::RowAccessConst etMatrix::operator[] (int a_index) const
{
    assert(0 <= a_index && a_index < 4);

    return RowAccessConst(&(m_matrix[a_index * 4]));
}

// etRowAccess functions.

etMatrix::RowAccess::RowAccess(double* a_rowStart)
    : m_rowStart(a_rowStart)
{
    // Do nothing.
}

etMatrix::RowAccessConst::RowAccessConst(const double* a_rowStart)
    : m_rowStart(a_rowStart)
{
    // Do nothing.
}

double& etMatrix::RowAccess::operator[](int a_index)
{
    assert(0 <= a_index && a_index < 4);

    return m_rowStart[a_index];
}

const double& etMatrix::RowAccessConst::operator[](int a_index) const
{
    assert(0 <= a_index && a_index < 4);

    return m_rowStart[a_index];
}

// Return this matrix's inverse, if it is an affine matrix
etMatrix etMatrix::inverse()
{
    etMatrix l_result;

    double l_det = determinant3x3();

    assert(fabs(l_det) > PRECISION_LIMIT);

    // Performance optimization:
    // Calculate inverse(A) = adj(A) / det(A)
    double invdet = 1.0 / l_det;
    l_result.m_matrix[0] =  determinant2x2(1,2,1,2) * invdet;
    l_result.m_matrix[4] = -determinant2x2(1,2,0,2) * invdet;
    l_result.m_matrix[8] =  determinant2x2(1,2,0,1) * invdet;
    l_result.m_matrix[1] = -determinant2x2(0,2,1,2) * invdet;
    l_result.m_matrix[5] =  determinant2x2(0,2,0,2) * invdet;
    l_result.m_matrix[9] = -determinant2x2(0,2,0,1) * invdet;
    l_result.m_matrix[2] =  determinant2x2(0,1,1,2) * invdet;
    l_result.m_matrix[6] = -determinant2x2(0,1,0,2) * invdet;
    l_result.m_matrix[10] =  determinant2x2(0,1,0,1) * invdet;

    // Calculate -C * inverse(A) 
    l_result.m_matrix[12] = - (m_matrix[12] * l_result.m_matrix[0] +
                               m_matrix[13] * l_result.m_matrix[4] +
                               m_matrix[14] * l_result.m_matrix[8]);
    l_result.m_matrix[13] = - (m_matrix[12] * l_result.m_matrix[1] +
                               m_matrix[13] * l_result.m_matrix[5] +
                               m_matrix[14] * l_result.m_matrix[9]);
    l_result.m_matrix[14] = - (m_matrix[12] * l_result.m_matrix[2] +
                               m_matrix[13] * l_result.m_matrix[6] +
                               m_matrix[14] * l_result.m_matrix[10]);

    // Fill in the last column 
    l_result.m_matrix[3] = 0.0;
    l_result.m_matrix[7] = 0.0;
    l_result.m_matrix[11] = 0.0;
    l_result.m_matrix[15] = 1.0;

    // Original code:
    //      /* Calculate inverse(A) = adj(A) / det(A) */
    //      double invdet = 1.0 / l_det;
    //      l_result[0][0] =  determinant2x2(1,2,1,2) * invdet;
    //      l_result[1][0] = -determinant2x2(1,2,0,2) * invdet;
    //      l_result[2][0] =  determinant2x2(1,2,0,1) * invdet;
    //      l_result[0][1] = -determinant2x2(0,2,1,2) * invdet;
    //      l_result[1][1] =  determinant2x2(0,2,0,2) * invdet;
    //      l_result[2][1] = -determinant2x2(0,2,0,1) * invdet;
    //      l_result[0][2] =  determinant2x2(0,1,1,2) * invdet;
    //      l_result[1][2] = -determinant2x2(0,1,0,2) * invdet;
    //      l_result[2][2] =  determinant2x2(0,1,0,1) * invdet;
    //
    //      /* Calculate -C * inverse(A) */
    //      l_result[3][0] = - ( (*this)[3][0] * l_result[0][0] +
    //                           (*this)[3][1] * l_result[1][0] +
    //                           (*this)[3][2] * l_result[2][0] );
    //      l_result[3][1] = - ( (*this)[3][0] * l_result[0][1] +
    //                           (*this)[3][1] * l_result[1][1] +
    //                           (*this)[3][2] * l_result[2][1] );
    //      l_result[3][2] = - ( (*this)[3][0] * l_result[0][2] +
    //                           (*this)[3][1] * l_result[1][2] +
    //                           (*this)[3][2] * l_result[2][2] );
    //
    //      /* Fill in the last column */
    //      l_result[0][3] = 0.0;
    //      l_result[1][3] = 0.0;
    //      l_result[2][3] = 0.0;
    //      l_result[3][3] = 1.0;

    return l_result;
}

double etMatrix::determinant3x3()
{
    // Performance optimization:
    return (m_matrix[0] * determinant2x2(1,2,1,2) -
            m_matrix[1] * determinant2x2(1,2,0,2) +
            m_matrix[2] * determinant2x2(1,2,0,1));

    // Original code:
    //      return ((*this)[0][0] * determinant2x2(1,2,1,2) -
    //              (*this)[0][1] * determinant2x2(1,2,0,2) +
    //              (*this)[0][2] * determinant2x2(1,2,0,1));
}

etOrientation etMatrix::getRotate() const
{
    //assert that col3 is 0 0 0 1
    assert(m_matrix[3]==0 && m_matrix[7]==0 &&
           m_matrix[11]==0 && m_matrix[15]==1);

    etVector l_scale = getScale();

    double l_theta = acos((m_matrix[0]/l_scale.x() +
                           m_matrix[5]/l_scale.y() +
                           m_matrix[10]/l_scale.z() - 1) / 2);

    if(l_theta == 0)
    {
        return etOrientation(0, 1, 0, 0);
    }

    double l_denom = 2*sin(l_theta);

    double l_x = (m_matrix[6]/l_scale.y() - m_matrix[9]/l_scale.z())/l_denom;
    double l_y = (m_matrix[8]/l_scale.z() - m_matrix[2]/l_scale.x())/l_denom;
    double l_z = (m_matrix[1]/l_scale.x() - m_matrix[4]/l_scale.y())/l_denom;

    return etOrientation(l_theta,l_x,l_y,l_z);
}

etVector etMatrix::getTranslate() const
{
    //assert that col3 is 0 0 0 1
    assert(m_matrix[3]==0 && m_matrix[7]==0 &&
           m_matrix[11]==0 && m_matrix[15]==1);

    return etVector(m_matrix[12], m_matrix[13], m_matrix[14]);
}

etVector etMatrix::getScale() const
{
    //assert that col3 is 0 0 0 1
    assert(m_matrix[3]==0 && m_matrix[7]==0 &&
           m_matrix[11]==0 && m_matrix[15]==1);

    double l_row0Mag = etVector(m_matrix[0],
                                   m_matrix[1],
                                   m_matrix[2]).length();
    double l_row1Mag = etVector(m_matrix[4],
                                   m_matrix[5],
                                   m_matrix[6]).length();
    double l_row2Mag = etVector(m_matrix[8],
                                   m_matrix[9],
                                   m_matrix[10]).length();



        //THIS IS TOO STRICT, NEED TO ADD SOME ROUNDOFF SLACK
        //assert that the row magnitudes are the same
            //assert(l_row0Mag==l_row1Mag && l_row1Mag==l_row2Mag);


        //try this instead:

            //double l_row0row1Diff = fabs(l_row0Mag-l_row1Mag);
            //double l_row1row2Diff = fabs(l_row1Mag-l_row2Mag);

        /*   assert(l_row0row1Diff<2*PRECISION_LIMIT 
                && l_row1row2Diff<2*PRECISION_LIMIT);
        */


    return etVector(l_row0Mag, l_row1Mag, l_row2Mag);
}
