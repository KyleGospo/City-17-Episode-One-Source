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
#ifndef SM_API_COORDUTILS_H
#define SM_API_COORDUTILS_H

/*! @file
    Defines utility functions for mapping and projecting points between various coordinate frames.
    
    @see
    - @ref sm_api_coord_frames_standard */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Map a 3D face coordinate into 3D world coordinates.
    
    @param head_pose_data Defines the position and orientation of the head relative to the world coordinate frame.
    @param face_coord The 3d face coordinate to be mapped. Defined relative to the face coordinate frame.
    @param world_coord The mapped @a face_coord, defined relative to the world coordinate frame.    

    @return @ref smReturnCode "SM_API_OK" if the mapping computation was successful.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL.
    
    @see
    - @ref sm_api_coord_frames_standard
    - smEngineHeadPoseData
    - smFaceCoord
    - smCoord3f */
SM_API(smReturnCode) smCoordFaceToWorld(const smEngineHeadPoseData* head_pose_data, const smFaceCoord* face_coord, smCoord3f* world_coord );

/*! @brief Project a 3D world coordinate into 2D pixel coordinates.
    
    @param lens_params Defines the camera lens projection, from world to pixel coordinates. 
                       Parameters must define a valid projection (e.g. non-zero focal length, principal point).
    @param world_coord The 3d world coordinate to be projected, relative to the world coordinate frame.
    @param pixel_coord The 2d pixel projection of the 3d world coordinate, defined relative to the pixel coordinate frame.

    @return @ref smReturnCode "SM_API_OK" if the mapping computation was successful.   
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL.
    
    @see
    - @ref sm_api_coord_frames_standard
    - smEngineHeadPoseData    
    - smCoord3f
    - smPixel*/
SM_API(smReturnCode) smCoordWorldToPixel(const smCameraLensParams* lens_params, const smCoord3f* world_coord, smPixel* pixel_coord );

/*! @brief Project a 3D face coordinate into 2D pixel coordinates.
    
    This function is provided for convenience. It is equivalent to calling smCoordFaceToWorld() and smCoordWorldToPixel()
    in succession.

    @param lens_params Defines the camera lens projection, from world to pixel coordinates.  
                       Parameters must define a valid projection (e.g. non-zero focal length, principal point).
    @param head_pose_data Defines the position and orientation of the head relative to the world coordinate frame.
    @param face_coord The 3d face coordinate to be mapped, defined relative to the face coordinate frame.
    @param pixel_coord The 2d pixel projection of the 3d world coordinate, defined relative to the pixel coordinate frame.

    @return @ref smReturnCode "SM_API_OK" if the mapping computation was successful.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL or lens_params
        is not valid for projection.
    
    @see
    - @ref sm_api_coord_frames_standard
    - smEngineHeadPoseData
    - smCameraLensParams
    - smFaceCoord    
    - smPixel*/
SM_API(smReturnCode) smCoordFaceToPixel(const smEngineHeadPoseData* head_pose_data, const smCameraLensParams* lens_params, const smFaceCoord* face_coord, smPixel* pixel_coord );

/*! @brief Computes the pixel location in @a texture.image of the face texture coordinate @a ftc using the @a texture.origin and @a texture.scale

    This function is @a reentrant.
    @param texture Must point to an existing smFaceTexture structure.
    @param ftc face texture coordinate associated with this texture.
    @param pixel Must point to an existing smPixel. The coordinates of this pixel are set.
    @return SM_API_OK if conversion suceeded.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL.
    
    @see
    - @ref sm_api_coord_frames_standard
    - @ref smFaceTexture */
SM_API(smReturnCode) smFaceTexCoordToPixel(const smFaceTexture* texture, const smFaceTexCoord* ftc, smPixel *pixel);


/*! Computes the pixel location of the Nth mask boundary landmark in @a texture.image.

    This function is @a reentrant.
    @param texture smFaceTexture structure.
    @param mask_landmark_index value between [0,texture.num_mask_landmarks-1]
    @param pixel Must point to an existing smPixel. The coordinates of this pixel are set.
    @return SM_API_OK if conversion suceeded.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL.

    @see
    - @ref sm_api_coord_frames_standard
    - @ref smFaceTexture */
SM_API(smReturnCode) smFaceTexMaskIndexToPixel(const smFaceTexture* texture, int mask_landmark_index, smPixel *pixel);


#ifdef __cplusplus
}
#endif
#endif
