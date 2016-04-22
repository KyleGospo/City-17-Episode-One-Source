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
#ifndef SM_API_CAMERA_H
#define SM_API_CAMERA_H

/*! @file 
    @ingroup sm_api_group_camera
    Defines functions for working with cameras. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Register types of cameras for detection. 

    This function only needs to be called for hardware cameras. 
    The types @ref smCameraType "SM_API_CAMERA_TYPE_IMAGE_PUSH" and @ref smCameraType "SM_API_CAMERA_TYPE_FILE"
    do not need to be registered.
    
    On the first call, this will scan the system and attempt to connect to each instance of the camera type.
    If many cameras are connected to the system, this call may take a while.\n
    
    If called a second time with the same argument, the function will have no effect.    
    
    @param type Set this to one of the possible smCameraType values.

    @return SM_API_OK if the camera type was registered successfully.

    @note
    - On windows, the category SM_API_CAMERA_TYPE_WDM will cause most cameras to be detected unless they have
    a custom driver supplied by the camera manufacturer.\n
    - For custom cameras, you will need to create an SM_API_CAMERA_TYPE_IMAGE_PUSH camera, obtain images using the 
    vendor-specific API and use the smCameraImagePush() function.

    @see
    - smCameraDeregisterTypes()
    
    @ingroup sm_api_group_camera */
SM_API(smReturnCode) smCameraRegisterType(smCameraType type);

/*! @brief Deregisters all camera types.
    
    Use this to force redetection of cameras on the next call to smCameraRegisterType() */
SM_API(smReturnCode) smCameraDeregisterTypes();

/*! @brief Creates a list of information for all the available cameras. 
    @param info_list Must point to an existing smCameraInfoList structure.
           The function allocates memory for the smCameraInfoList.info array which is deallocated by smCameraDestroyInfoList().
    @return SM_API_OK if camera detection process was successful.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a info_list is null.
    @note
    - The list will be empty unless smCameraRegisterType() has been called.
    - If a camera is already in use by this process or has been claimed by another process it will still appear in this list. 
    To redetect cameras available for creation, call smCameraDeregisterTypes() followed by smCameraRegisterType().
    
    @ingroup sm_api_group_camera */
SM_API(smReturnCode) smCameraCreateInfoList(smCameraInfoList *info_list);

/*! @brief Frees any memory allocated by smCameraCreateInfoList()
    @param info_list must have been passed to smCameraCreateInfoList() before being passed to this routine.
    @return SM_API_OK if the info list was destroyed correctly.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a info_list is null.
    
    @ingroup sm_api_group_camera*/
SM_API(smReturnCode) smCameraDestroyInfoList(smCameraInfoList *info_list);

/*! @brief Creates the camera defined by @a camera_info and initializes it using any supplied smCameraSettings (optional).
    
    @param camera_info Must be one of the list entries returned by smCameraCreateInfoList(). Do not modify the contents.
    @param settings Optional settings for the camera. If 0, default settings will be used.
    @param camera_handle Pointer to smCameraHandle. On success set to a valid smCameraHandle handle.

    @return SM_API_OK if the creation process was successful.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a camera_info or @a camera_handle are null.

    @note
    - Use smCameraDestroy to destroy the created camera.
    - If the camera has been previously created and not yet destroyed, then the function will fail. 
    - The function will fail if during the time between the call to smCameraCreateInfoList() and smCameraCreate()
      another process has taken ownership of the camera. */
SM_API(smReturnCode) smCameraCreate(const smCameraInfo* camera_info, const smCameraSettings *settings, smCameraHandle *camera_handle);

/*! @brief Creates an "Image Push" camera, which can then be used by the client to "push" image data into the camera,
    using smCameraImagePush().

    Use an Image Push camera to pass your own image data into API functions that require a smCameraHandle argument.

    @param video_format Format of the video the camera will produce. 
           Images pushed into the camera must be of the same resolution and smImageCode as defined by this format.
    @param max_fifo_len Maximum number of video-frames that can be stored internally in the camera before the oldest
           video-frame will be discarded (dropped).
           If the camera is being used by an engine, the engine will consume the images, so this fifo is used to 
           buffer timing fluctuations between the producer (you) and the consumer (the engine).
           Set to SM_API_CAMERA_DEFAULT_FIFO_LEN to remove any upper limit on the fifo (limited by RAM).
    @param cal_filename Optional lens calibration file (*.lens) generated by CamCal tool. Can be NULL handle (ie. 0).
    @param approx_hfov_deg Optional If no cal_filename is specified then this must point to an integer value between 1 and 89 degrees, otherwise can be 0.
    @param push_camera_handle Pointer to smCameraHandle. On success this is set to a valid smCameraHandle handle.

    @return SM_API_OK if the creation process was successful. 
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a video_format or @a push_camera_handle are null.

    @note
    - The image resolution specified by @a video_format must be larger than 0.
    - For @ref smImageCode "SM_API_IMAGECODE_YUY2" images, the image width must be divisible by 2. 
    - For @ref smImageCode "SM_API_IMAGECODE_I420", both the image width and height must be divisible by 2.         
    - Use smCameraDestroy to destroy the created camera.
    - If the camera has been previously created and not yet destroyed, then the function will fail.
    - All subsequent calls to smCameraImagePush must provide image arguments that are consistent with the specified @a vdeo_format. */  
SM_API(smReturnCode) smCameraCreateImagePush(const smCameraVideoFormat* video_format, int max_fifo_len, smStringHandle cal_filename, int *approx_hfov_deg, smCameraHandle *push_camera_handle);

/*! @brief Creates a camera that obtains its image data from a movie file.

    Valid file types are .mov, .avi, .wmv, .asf, .mp4 and potentially others depending on the
    codecs installed on the PC. As a general guide if the file is playable in a media-player application
    and is internally indexed by frame-number, then it is likely to work. Files that do not have valid durations 
    under the file properties (right-click the file under windows explorer) generally cannot be used. 
    
    @param filename Path to the movie file including file extension.
    @param cal_filename Optional lens calibration file (*.lens) generated by CamCal tool. Can be NULL handle (ie. 0).
    @param approx_hfov_deg If no cal_filename is specified then this must point to an integer value between 1 and 89 degrees, otherwise can be 0.
    @param file_camera_handle Pointer to an smCameraHandle. On success this is set to a valid smCameraHandle handle.

    @return SM_API_OK if the camera was created and the movie file was successfully checked to be valid. */
SM_API(smReturnCode) smCameraCreateFile(smStringHandle filename, smStringHandle cal_filename, int *approx_hfov_deg, smCameraHandle *file_camera_handle);

/*! @brief Determines if an smCameraHandle is valid (has been created and not destroyed)
    @param camera_handle A camera to test for validity.
    @param valid Set to SM_API_TRUE if the image is valid, SM_API_FALSE otherwise.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smCameraIsValid(smCameraHandle camera_handle, smBool *valid);

/*! @brief Gets the smCameraInfo from a specific smCameraHandle.
    @param camera_handle The camera to obtain information from. Must be an existing valid camera.
    @param buffer Pointer to a buffer of memory to receive camera information or NULL if the caller
        wants to determine the number of bytes required for the call to succeed..
    @param buffer_size On input, pointer to an unsigned int containing the number of bytes available at @a buffer. On
            return, it contains the number of bytes required to hold the camera information.
    @return SM_API_OK if the function completed successfully.
    @return SM_API_FAIL_BUFFER_TOO_SMALL The @a buffer_size value is too small, it has been updated to the
        minimum size for the call to succeed.
    
    @note On success, the caller must call smStringDestroy on smCameraInfo::model. Also,
        it is possible that the returned 
    
    <b>Example code:</b>
    @code
    unsigned int size = 0;
    void* buffer = 0;
    const smCameraInfo* camera_info = 0;
    smReturnCode return_code = smCameraGetInfo(camera, 0, &size);
    while (SM_API_FAIL_BUFFER_TOO_SMALL == return_code)
    {
        buffer = (unsigned char*)realloc(buffer, size);
        return_code = smCameraGetInfo(camera, buffer, &size);
    }
    assert(return_code == SM_API_OK);
    camera_info = (const smCameraInfo*)buffer;
    // do stuff
    smStringDestroy(&camera_info->model);
    free(buffer);
    @endcode
*/
SM_API(smReturnCode) smCameraGetInfo(smCameraHandle camera_handle, void* buffer, unsigned int* buffer_size);

/*! @brief Push an smImage into an "Image Push" smCameraHandle
    
    An smEngine must be created using the image-push camera ( using smEngineCreateWithCamera() ) before smCameraImagePush()
    can be called.    

    @param push_camera_handle An "Image Push" camera. This must previously have been created with smCameraCreateImagePush().
    @param video_frame Contains the image and other information to be pushed.  See smImageCreateFromInfo().
    @param lens_params Optional argument (can be 0) that can be used to override any existing lens-parameters the camera is using.
           This argument is only necessary if the lens is changing dynamically (e.g motorized zoom lens).
    @param src_rect <b>NOTE: This argument is not currently supported. Set to 0 only.</b>\n
		  Optional argument (can be 0) describing the position and size of the region inside the image dimensions of the 
           image used to calibrate the sensor, where the image being pushed is obtained from.\n
           This is typically used when a camera driver is able to provide a region-of-interest (ROI)
           sub-image. This argument then defines where that sub-image is located.\n
           It can also be used to specify a ROI image that has also been resized / decimated by the camera driver.
           For example, if the camera was calibrated at the native sensor resolution of (1280,1024) and
           is then configured to deliver (640,480) sized images at particular offsets, then the @a top_left 
           parameter is set to the offset in the (1280,1024) image.
           The following @ref image_push_camera "diagram" illustrates the @a src_rect concept.
                
    @return @ref smReturnCode "SM_API_OK" if the push succeeds.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a sm_camera_handle is null or invalid, @a sm_video_frame is null.
    
    @see
    - smImageHandle
    - smImageInfo    
    - smRegisterImageReleaseCallback() 
    - smCameraCreateImagePush() */
SM_API(smReturnCode) smCameraImagePush(smCameraHandle push_camera_handle, const smCameraVideoFrame* video_frame, const smCameraLensParams* lens_params, smImageRect *src_rect);

/*! @brief Loads the lens parameters from a filename.
    
    Lens parameter files can be generated by the API tool "CamCal.exe", which is part of the API distribution.
    @param filename Path and filename of the calibration file.
    @param params Must point to an existing smCameraLensParams struct (cannot be 0) 
    @return SM_API_OK if the load occured ok. */
SM_API(smReturnCode) smCameraLoadLensParamsFile(smStringHandle filename, smCameraLensParams *params);

/*! @brief Sets the lens parameters from an approximate field-of-view measurement

    This generates an approximate set of lens coefficients, which may be suitable for some algorithms to use. 
    @param hfov_deg Approximate field-of-view of the lens, >0 and <180 degrees.
    @param res Resolution of the images that the camera will produce when tracking.
    @param params Must point to an existing smCameraLensParams struct (cannot be 0), which is set.
    
    @return SM_API_OK if params was set ok.
    
    @see
    - smCameraLoadLensParamsFile() */
SM_API(smReturnCode) smCameraLensParamsFromHFOV(int hfov_deg, smSize2i res, smCameraLensParams *params);

/*! @brief Set the lens-parameters of the camera.
    
    Lens parameters are required for accurate measurement and for measurements in world-coordinates such as the 
    maximum and minimum tracking ranges to make sense. If the lens parameters are not set, camera are constructed
    to use an approximate lens configuration, which may be suitable for some applications.
    
    @param camera_handle The camera to assign the lens-parameters for.
    @param lens_params The lens parameters to assign. This parameter must have been previously initialized 
        with valid parameters using smCameraLensParamsFromHFOV() or smCameraLoadLensParamsFile().
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a sm_camera_handle is null or invalid or @a lens_params is null.
    
    @note
    - To calibrate the camera lens accurately, use the tool "CamCal.exe" provided as part of this API. The tool
    produces a lens-calibration file which can be loaded using smCameraLoadLensParamsFile(). */
SM_API(smReturnCode) smCameraSetLensParams(smCameraHandle camera_handle, const smCameraLensParams* lens_params);

/*! @brief Get the current lens-parameters the camera is using.
    @param camera_handle Must be a valid smCameraHandle.
    @param lens_params Pointer to an existing smCameraLensParams. The contents are changed to be the lens parameters the camera is using. 
    @return SM_API_OK if the function succeeded. */
SM_API(smReturnCode) smCameraGetLensParams(smCameraHandle camera_handle, smCameraLensParams *lens_params);

/*! @brief Destroys the specific camera, freeing any internally allocated resources and releasing any handles.
    @param camera_handle Pointer to an existing valid (created) smCameraHandle. Set to 0 upon success. */
SM_API(smReturnCode) smCameraDestroy(smCameraHandle *camera_handle);

/*! @brief Shows a modal camera control panel (exposure etc).

    Under windows, this usually displays the driver control panel for the camera. 
    @param camera_handle Existing (created) smCameraHandle handle. */
SM_API(smReturnCode) smCameraShowControlPanel(smCameraHandle camera_handle);

#ifdef __cplusplus
}
#endif
#endif
