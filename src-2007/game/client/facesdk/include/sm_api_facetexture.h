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
#ifndef SM_API_FACETEXTURE_H
#define SM_API_FACETEXTURE_H

/*! @file
    Defines types and routines in the API relating to working with face textures. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Possible types of textures the tracker can produce */
typedef enum smFaceTextureType
{
    SM_ORTHOGRAPHIC_PROJECTION = 0     /*!< The texture is generated from an orthographic projection of the front of the face. */
} smFaceTextureType;

/*! @brief Defines a texture image of a face, a boundary region (mask) as well as scale and offset parameters to convert
    between face-coordinates and pixels in the texture image. */
typedef struct smFaceTexture
{
    smFaceTextureType type;             /*!< Type of the texture. */
    smImageInfo image_info;             /*!< Texture image info. You can create an image from this info using smImageCreateFromInfo() */
    int num_mask_landmarks;             /*!< Number of landmarks used to define the mask region (outline) of the face texture. */
    smFaceLandmark *mask_landmarks;     /*!< List of landmarks used to define the mask region (outline) of the face texture. This memory is allocated by the API, do not free it. */
    smPos2f origin;                     /*!< Face origin pixel coordinate. @see smFaceTexCoordToPixel */
    smCoord2f scale;                    /*!< Scale factor in convert between pixels and face texture coordinates. @see smFaceTexCoordToPixel */
} smFaceTexture;

#ifdef __cplusplus
}
#endif
#endif
