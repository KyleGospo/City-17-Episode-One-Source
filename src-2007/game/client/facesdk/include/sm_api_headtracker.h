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
#ifndef SM_API_HEADTRACKER_H
#define SM_API_HEADTRACKER_H

/*! @file
    Defines types and routines in the API relating to tracking head-pose. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Defines the 3D pose of the head.
    @see
    - smHTHeadPoseCallback */
typedef struct smEngineHeadPoseData
{
    smPos3f head_pos;        /*!< Position of the head relative to the camera. */
    smPos3f left_eye_pos;    /*!< Position of the left eyeball center relative to the camera. */
    smPos3f right_eye_pos;   /*!< Position of the right eyeball center relative to the camera. */
    smRotEuler head_rot;     /*!< Rotation of the head around the X,Y and Z axes respectively in euler angles. @see http://en.wikipedia.org/wiki/Euler_angles */
    float confidence;        /*!< Confidence of the head-pose measurement [0..1]. A value of 0 indicates that the measurements cannot be trusted and may have undefined values. */
} smEngineHeadPoseData;

/*! @brief Contains facial landmarks and texture.

    This data may be produced by face algorithms. The contents of the structure are dynamic,
    depending on how many face landmarks are able to be located and if a 
    facial texture image can be generated. 

    @see
    - sm_api_flm_standard
    - smHTFaceDataCallback */
typedef struct smEngineFaceData
{
    int num_landmarks;          /*!< Number of landmarks being tracked */
    smFaceLandmark *landmarks;  /*!< Pointer to allocated array of size num_landmarks*sizeof(smFaceLandmark). Do not attempt to free this memory. */
    smFaceTexture *texture;     /*!< Texture for the face region. May be 0 if no texture is produced. Do not attempt to free this memory. */
    smCoord3f origin_wc;         /*!< The origin of the head, in world coordinates. @see sm_api_coord_frames_standard */
    smPixel origin_pc;           /*!< The origin of the head, in pixel coordinates. @note Only currently used by smFaceSearch(). @see sm_api_coord_frames_standard */
} smEngineFaceData;

/*! @brief Signature of function for obtaining the head-pose solution
    
    Define your own function matching this interface and register it with the engine to receive head-pose information.
    @param user_data Value passed into @a user_data argument in smHTRegisterHeadPoseCallback(). 
    @param head_pose The 3D head-pose solution.
    @see
    - smHTRegisterHeadPoseCallback() */
typedef void (STDCALL *smHTHeadPoseCallback)(void *user_data, smEngineHeadPoseData head_pose);

/*! @brief Registers a callback function for receiving smEngineHeadPoseData data from an smEngineHandle.
    
    Only one callback of this type can be registered with a particular smEngineHandle at any given time.
    @param engine_handle The engine to register the callback with.
    @param user_data Can be 0. This value is passed back to your callback routine. 
    It is typically used to pass the 'this' pointer of an object to enable the callback 
    to call a member function in the object.
	@param callback_fn Address of callback function, or 0 to unregister the callback.
	@return SM_API_OK if callback was registered successfully. */
SM_API(smReturnCode) smHTRegisterHeadPoseCallback(smEngineHandle engine_handle, void *user_data, smHTHeadPoseCallback callback_fn);

/*! @brief Obtains a current estimate of the head-pose from the engine.

    If the engine is not tracking the confidence will be 0.
    This function is designed to be called just before the value is used. The time of call
    of the function effects the output result. Internally the function performs interpolation
    using the time-of-call vs the last tracked sample from the engine.

    @param engine A valid engine of type SM_API_ENGINE_TYPE_HEAD_TRACKER_V1, or SM_API_ENGINE_TYPE_HEAD_TRACKER_V2.
    @param head_pose Must point to an existing smEngineHeadPoseData struct. The contents are set to the current estimate of the head-pose.
    @pre engine state >= SM_API_ENGINE_STATE_IDLE */
SM_API(smReturnCode) smHTCurrentHeadPose(smEngineHandle engine, smEngineHeadPoseData *head_pose);

/*! @brief Signature of function for obtaining smEngineFaceData from a head-tracker

    Define your own function matching this interface and register it with the engine. 
    to receive face texture and landmark information.    

    @param user_data Value passed into @a user_data argument in smHTRegisterFaceDataCallback(). 
    @param face_data Face landmark positions and face-texture information.
    @see
    - smHTRegisterFaceDataCallback()
    
    @note Any pointers in @a face_data will point to memory allocated by the API that only exists 
          for the duration of the callback routine. Your callback routine must deep copy the 
          information you require from this data structure. */
typedef void (STDCALL *smHTFaceDataCallback)(void *user_data, smEngineFaceData face_data);

/*! @brief Registers a callback function for receiving smEngineFaceData data from an smEngineHandle

    Only one callback of this type can be registered with a particular smEngineHandle at any given time.

    @param engine_handle The engine to register the callback with.
    @param user_data Can be 0. This value is passed back to your callback routine. 
    It is typically used to pass the 'this' pointer of an object to enable the callback 
    to call a member function in the object.
	@param callback_fn Address of callback function, or 0 to unregister the callback.
	@return SM_API_OK if callback was registered successfully. 
    @note
    - This call will succeed even for engine versions that do not produce smEngineFaceData. 
      In that case, the callback never gets called.
    - Any pointers in @a face_data will point to memory allocated by the API that only exists 
      for the duration of the callback routine. Your callback routine must deep copy the 
      information you require from this data structure. */
SM_API(smReturnCode) smHTRegisterFaceDataCallback(smEngineHandle engine_handle, void *user_data, smHTFaceDataCallback callback_fn);

#ifdef __cplusplus
}
#endif
#endif
