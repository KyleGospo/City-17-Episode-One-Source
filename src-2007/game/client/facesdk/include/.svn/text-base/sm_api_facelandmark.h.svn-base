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
#ifndef SM_API_FACELANDMARK_H
#define SM_API_FACELANDMARK_H

/*! @file
    Defines types and routines in the API relating to workling with face landmarks.
    
    @see
        - @ref sm_api_flm_standard */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief A smFaceCoord is a 3D cartesian coordinate in the head-reference frame. */
typedef smPos3f smFaceCoord;

/*! @brief Texture coordinates for faces.

    Face Texture Coordinates have their origin at the midpoint between the eyes (origin of the head-reference frame).
    The coordinates are normalized and are unrelated to the size of the texture image.
    To transform from a smFaceTexCoord to a pixel location in the texture image.
    
    @see
    - @ref smFaceTexCoordToPixel */
typedef struct smFaceTexCoord
{
    float u;    /*!< Position along x-axis, unitless. */
    float v;    /*!< Position along y-axis, unitless. */
} smFaceTexCoord;

/*! @brief Defines the id and location of a landmark on the face.

    @see
    - @ref sm_api_flm_standard
    - smHTFaceDataCallback */
typedef struct smFaceLandmark
{
    int id;               /*!< Valid id values are >= 0. @see @ref sm_api_flm_standard */
    smFaceCoord fc;       /*!< Position of the face landmark in the head-pose reference frame (face coordinates) */
    smFaceTexCoord ftc;   /*!< Position of the face landmark on the 2D face-texture plane (face texture coordinates) */
    smCoord3f wc;         /*!< Position of the face landmark in the world coordinate frame (world coordinates).*/
    smPixel pc;           /*!< Position of the face landmark in the 2D image pixel ccoordinate frame (pixel coordinates). @note Only currently used by smFaceSearch() */
} smFaceLandmark;

#ifdef __cplusplus
}
#endif
#endif
