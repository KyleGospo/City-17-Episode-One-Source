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
#ifndef SM_API_HEADTRACKERV1CONTROLS_H
#define SM_API_HEADTRACKERV1CONTROLS_H

/*! @file
    Defines functions for controlling the behaviour of the head-tracker V1.0. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Gets the quality threshold at which the tracker loses tracking and enters the search mode.
    Search mode is for fast tracking recovery.
    Higher values cause the tracker to enter search mode more often. Lower values
    cause the tracker to stay tracking more often, however the probability of false
    tracking also increases.    
	
    @param engine_handle The engine to query.
    @param percent Set to the current search threshold percent.
	@return SM_API_OK if the value was read successfully.
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see smHTV1SetSearchThreshold() */
SM_API(smReturnCode) smHTV1GetSearchThreshold(smEngineHandle engine_handle, float *percent);

/*! @brief Sets the quality threshold at which the tracker loses tracking and enters the search mode.    
    @param engine_handle The engine to change.
    @param percent The threshold to set, [0-100].
	@return SM_API_OK if the value was written successfully.
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see smHTV1GetSearchThreshold() */
SM_API(smReturnCode) smHTV1SetSearchThreshold(smEngineHandle engine_handle, float percent);

/*! @brief Gets the level of head-pose filtering.
    Low values create noisier but more responsive measurements.
    High values create smoother but less responsive measurements.   
    @param engine_handle The engine to query.
    @param percent Set to the filter level in percent.
	@return SM_API_OK if the value was read successfully.
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see smHTV1SetHeadPoseFilterLevel() */
SM_API(smReturnCode) smHTV1GetHeadPoseFilterLevel(smEngineHandle engine_handle, float *percent);

/*! @brief Sets the level of head-pose filtering.    	
    @param engine_handle The engine to change.
    @param percent The filter level in percent.
	@return SM_API_OK if the value was written successfully.
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see smHTV1GetHeadPoseFilterLevel() */
SM_API(smReturnCode) smHTV1SetHeadPoseFilterLevel(smEngineHandle engine_handle, float percent);

/*! @brief Gets the level of additional head-pose Z-coordinate filtering.
    Low values create noisier but more responsive measurements.
    High values create smoother but less responsive measurements.    	
    @param engine_handle The engine to query.
    @param percent Set to the filter level in percent.
	@return SM_API_OK if the value was read successfully.
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see smHTV1GetHeadPoseZFilterLevel() */
SM_API(smReturnCode) smHTV1GetHeadPoseZFilterLevel(smEngineHandle engine_handle, float *percent);

/*! @brief Sets the level of additional head-pose filtering.    	
    @param engine_handle The engine to change.
    @param percent The filter level in percent.
	@return SM_API_OK if the value was written successfully.
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see smHTV1SetHeadPoseZFilterLevel() */
SM_API(smReturnCode) smHTV1SetHeadPoseZFilterLevel(smEngineHandle engine_handle, float percent);

#ifdef __cplusplus
}
#endif
#endif
