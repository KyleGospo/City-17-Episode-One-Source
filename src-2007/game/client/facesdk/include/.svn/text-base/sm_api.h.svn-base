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
#ifndef SM_API_H
#define SM_API_H

/*! @file
    The master include file for the Face Tracking API. */

// Compiler and OS configuration
#include "sm_api_configure.h"

// Basic includes
#include "sm_api_returncodes.h"
#include "sm_api_string.h"
#include "sm_api_logging.h"
#include "sm_api_geomtypes.h"
#include "sm_api_imagetypes.h"
#include "sm_api_image.h"
#include "sm_api_time.h"
#include "sm_api_facelandmark.h"
#include "sm_api_facetexture.h"

// Engine includes
#include "sm_api_cameratypes.h"
#include "sm_api_camera.h"
#include "sm_api_engine.h"
#include "sm_api_videodisplay.h"
#include "sm_api_headtracker.h"
#include "sm_api_headtrackercontrols.h"
#include "sm_api_headtrackerv1controls.h"
#include "sm_api_headtrackerv2controls.h"

// File and image tracking functions
#include "sm_api_facesearch.h"
#include "sm_api_filetrack.h"

// Utility functions
#include "sm_api_coordutils.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Gets the version of the API.

    This function is @a reentrant.
    @param major Major number. The API interface has changed you will need to recompile.
    @param minor Minor number. New interfaces may have been added to the API, but binary compatibility is maintained.
    @param maint Maintenance number. The API interface is unaltered. 
    @note This is the only function that the API does not need to be initialized prior to calling. */
SM_API(smReturnCode) smAPIVersion(int *major, int *minor, int *maint);

/*! @return SM_API_TRUE if the API is a non-commercial license, SM_API_FALSE otherwise. 
    @see @ref sm_api_license_info */
SM_API(smBool) smAPINonCommercialLicense();

/*! @brief Initializes the API. This should be the first API function your program calls.

    All other functions will return SM_API_FAIL_INVALID_CALL until this function is called. 
    @note This function should be calling on the main thread. Calling this function will
    initialize COM for the thread as a Single Threaded Apartment. At present the API
    may not work if COM has already been initialized as MTA. */
SM_API(smReturnCode) smAPIInit();

//! Call at the end of your program to clean up API resources
SM_API(smReturnCode) smAPIQuit();

/*! @brief Manually process any window events

    This is only required for console-style applications which
    need to show a video display window and that do not have an 
    existing window event loop. It may also be called to manually
    update the video window when an application is showing a modal
    dialog.

    <b>Example code:</b>
    @code
    void main()
    {
        // Initialize API...
        // Create and start an engine...
        // Create a video-display
        while (!quit)
        {
            // Perhaps check if the ESC key has been hit
            ...
            // Refresh video display
            smAPIProcessEvents();
            // Prevent CPU overload in this loop
            Sleep(30);
        }
    }
    @endcode */
SM_API(smReturnCode) smAPIProcessEvents();

/*! @brief If a VideoDisplay is shown, then this function needs to be called before entering a modal event loop.
    
    <b>Example code:</b>
    @code
    smAPIEnterModalLoop();
    MessageBox(...);
    smAPIExitModalLoop();
    @endcode
    
    @see
    - smAPIExitModalLoop() */
SM_API(smReturnCode) smAPIEnterModalLoop();

/*! @brief If a VideoDisplay is shown, then this function needs to be called after exiting a modal event loop.

    <b>Example code:</b>
    @code
    smAPIEnterModalLoop();
    MessageBox(...);
    smAPIExitModalLoop();
    @endcode
    
    @see
    - smAPIEnterModalLoop() */
SM_API(smReturnCode) smAPIExitModalLoop();

#ifdef __cplusplus
}
#endif
#endif
