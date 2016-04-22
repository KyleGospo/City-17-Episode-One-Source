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
#ifndef SM_API_HEADTRACKERV2CONTROLS_H
#define SM_API_HEADTRACKERV2CONTROLS_H

/*! @file
    Defines functions for controlling the behaviour of the head-tracker V2.0. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Gets the tracking restart-timeout enabled state.

    When restart-timeout is enabled, the tracker will be automatically restarted
    when a the restart-timeout period has elapsed and no tracking has occured.\n
	
    @param engine_handle The engine to query.
    @param enabled Pointer to an existing smBool. Set to the enabled state of the tracking timeout.
	@return SM_API_OK if the value was read successfully. 
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see 
    - smHTV2SetRestartTimeoutEnabled()
    - smHTGetRestartTimeout()
    - smHTSetRestartTimeout() */
SM_API(smReturnCode) smHTV2GetRestartTimeoutEnabled(smEngineHandle engine_handle, smBool *enabled);

/*! @brief Sets the tracking restart-timeout enabled state.
	
    @param engine_handle The engine to change.
    @param enabled Sets the enabled state of the tracking timeout.
	@return SM_API_OK if the value was set successfully. 
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see 
    - smHTV2GetRestartTimeoutEnabled()
    - smHTGetRestartTimeout()
    - smHTSetRestartTimeout() */
SM_API(smReturnCode) smHTV2SetRestartTimeoutEnabled(smEngineHandle engine_handle, smBool enabled);

/*! @brief Gets the tracking restart on poor-tracking enabled state.
	
    @param engine_handle The engine to change.
    @param enabled Gets the enabled state of the poor-tracking detector.
	@return SM_API_OK if the value was set successfully. 
    @pre state >= SM_API_ENGINE_STATE_IDLE
    @see 
    - smHTV2SetRestartPoorTrackingEnabled() 
    - smHTGetRestartThreshold() 
    - smHTSetRestartThreshold() */
SM_API(smReturnCode) smHTV2GetRestartPoorTrackingEnabled(smEngineHandle engine_handle, smBool *enabled);

/*! @brief Sets the tracking restart on poor-tracking enabled state.

	@pre state >= SM_API_ENGINE_STATE_IDLE
    @param engine_handle The engine to change.
    @param enabled Sets the enabled state of the poor-tracking detector.
	@return SM_API_OK if the value was set successfully. 
    @see 
    - smHTV2GetRestartPoorTrackingEnabled() 
    - smHTGetRestartThreshold() 
    - smHTSetRestartThreshold() */
SM_API(smReturnCode) smHTV2SetRestartPoorTrackingEnabled(smEngineHandle engine_handle, smBool enabled);

#ifdef __cplusplus
}
#endif
#endif

