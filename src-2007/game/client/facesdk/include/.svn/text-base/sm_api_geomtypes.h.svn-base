/*
	Copyright (C) 2008 Seeing Machines Ltd. All rights reserved.

	This file is part of the FaceTrackingAPI, also referred to as "faceAPI".

	This file may be distributed under the terms of the Seeing Machines 
	FaceTrackingAPI Non-Commercial License Agreement.

	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

	Further information about faceAPI licensing is available at:
	http://www.seeingmachines.com/faceapi/licensing.html
*/
#ifndef SM_API_GEOMTYPES_H
#define SM_API_GEOMTYPES_H

/*! @file
    Defines types for basic geometry. All units are S.I. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Defines a coordinate in 2 dimensions using floating point values. */
typedef struct smCoord2f
{
    float x; /*!< Coordinate X value */
    float y; /*!< Coordinate Y value */
} smCoord2f;

/*! @brief Defines a coordinate in 3 dimensions using floating point values. */
typedef struct smCoord3f
{
    float x; /*!< Coordinate X value */
    float y; /*!< Coordinate Y value */
    float z; /*!< Coordinate Z value */
} smCoord3f;

/*! @brief Defines a position in 2 dimensions using floating point values. */
typedef smCoord2f smPos2f;

/*! @brief Defines a position in 3 dimensions using floating point values. */
typedef smCoord3f smPos3f;

/*! @brief Defines a coordinate in 2 dimensions using double-precision floating point values. */
typedef struct smCoord2d
{
    double x; /*!< Coordinate X value */
    double y; /*!< Coordinate Y value */
} smCoord2d;

/*! @brief Defines a coordinate in 3 dimensions using double-precision floating point values. */
typedef struct smCoord3d
{
    double x; /*!< Coordinate X value */
    double y; /*!< Coordinate Y value */
    double z; /*!< Coordinate Z value */
} smCoord3d;

/*! @brief Defines a position in 2 dimensions using double-precision floating point values. */
typedef smCoord2d smPos2d;

/*! @brief Defines a position in 3 dimensions using double-precision floating point values. */
typedef smCoord3d smPos3d;

/*! @brief Euler angle representation of orientation. All values are in radians.

    The euler angles are represented in the X-Y-Z convention.
    @see
    - http://en.wikipedia.org/wiki/Euler_angles */
typedef struct smRotEuler
{
    float x_rads; /*!< Rotation angle around X-axis, in radians */
    float y_rads; /*!< Rotation angle around Y-axis, in radians */
    float z_rads; /*!< Rotation angle around Z-axis, in radians */
} smRotEuler;

/*! @brief Defines a coordinate in 2 dimensions using integer values. */
typedef struct smCoord2i
{
    int x;  /*!< Coordinate X value */
    int y;  /*!< Coordinate X value */
} smCoord2i;

/*! @brief Defines a size in 2 dimensions using integer values. */
typedef struct smSize2i
{
    int w;  /*!< Width */
    int h;  /*!< Height */
} smSize2i;

#ifdef __cplusplus
}
#endif
#endif

