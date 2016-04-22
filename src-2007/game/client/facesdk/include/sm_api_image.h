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
#ifndef SM_API_IMAGE_H
#define SM_API_IMAGE_H

/*! @file
    Defines functions that operate on images. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Creates an image using externally supplied memory. 

    This function allows the creation of an image that can be used with other API routines from
    externally supplied memory. The memory can be copied into a new image, or shared with a new image.
    Memory can be shared only if the start address of scanline is 32-byte aligned.

    @param image_info Contains the required information about the external memory: size, format, step-size, data address etc.
    @param copy_mode This parameter is read as an input argument and written as an output argument.
           It must point to an existing smImageMemoryCopyMode which specifies the desired memory copying behaviour. 
           Normally set to SM_API_IMAGE_MEMORYCOPYMODE_AUTO.
           The function will set the result to either SM_API_IMAGE_MEMORYCOPYMODE_DEEP or SM_API_IMAGE_MEMORYCOPYMODE_SHARED.
    @param image_handle Pointer to an existing invalid smImageHandle. A new image is created from the memory and this handle is set to it.
    @return SM_API_OK if the function returned successfully.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL, image_info is invalid,
        or image_handle != 0.

    @see
    - smImageInfo
    - smRegisterImageReleaseCallback() */
SM_API(smReturnCode) smImageCreateFromInfo(const smImageInfo* image_info, smImageMemoryCopyMode *copy_mode, smImageHandle *image_handle);

/*! @brief Creates an image loading it from a PNG file. 
    @param png_filename Full path and filename of a valid png file.
    @param image_handle Pointer to an existing (but invalid) smImageHandle handle. The image is created and the handle is set. 
    @return @ref smReturnCode SM_API_OK Image created ok.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL or image_handle != 0.
    @return @ref smReturnCode "SM_API_FAIL_OPEN_FILE" Failed to open @a png_filename.

        @see
    - smImageSaveToPNG() 
    @return SM_API_OK if the image was created successfully. */
SM_API(smReturnCode) smImageCreateFromPNG(smStringHandle png_filename, smImageHandle *image_handle);

/*! @brief Determines if an image is valid (has been created and not destroyed)
    @param image_handle An image to test for validity.
    @param valid Set to SM_API_TRUE if the image is valid, SM_API_FALSE otherwise.
    @return @ref smReturnCode "SM_API_OK" if the function completed successfully. 
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" if any pointer parameter is NULL.
*/
SM_API(smReturnCode) smImageIsValid(smImageHandle image_handle, smBool *valid);

/*! @brief Register a function that is called when an image that is using shared memory, is no longer in use by the API.
    
    Define your own function and register with this function to be notified that 
    image data that was passed into smImageCreateFromInfo() is no longer in use by any API internal routines.
    Typically, the callback function is used to free user image memory.
    @param callback_fn The address of the callback function, or 0 to unregister any existing function. 
    @return SM_API_OK if the callback was registered successfully. */
SM_API(smReturnCode) smRegisterImageReleaseCallback(smImageReleasedCallback callback_fn);

/*! @brief Obtain size, format and memory information about an smImageHandle. 
    @param image_handle A valid smImageHandle to obtain information about
    @param info Pointer to an existing smImageInfo struct, which is set. 
    @return SM_API_OK if the info was obtained successfully. 
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a image_handle is not valid or @a info is NULL.
*/
SM_API(smReturnCode) smImageGetInfo(smImageHandle image_handle, smImageInfo *info);

/*! @brief Saves an image to a PNG file. 
    @param image_handle An existing valid smImageHandle handle. 
    @param png_filename Full path and filename of the png file to create. Any existing file will be overwritten. 
    @return SM_API_OK if the image was saved successfully. 
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a image_handle or @a png_filename are null or not valid.
*/
SM_API(smReturnCode) smImageSaveToPNG(smImageHandle image_handle, smStringHandle png_filename);

/*! @brief Destroy the image. 
    @param image_handle A valid smImageHandle handle.
    @return @ref smReturnCode "SM_API_FAIL_INVALID_ARGUMENT" @a image_handle is null or not a valid handle.
    @note If the image was created from user supplied memory, the image-release callback will be called. 
    @see smRegisterImageReleaseCallback */
SM_API(smReturnCode) smImageDestroy(smImageHandle *image_handle);

#ifdef __cplusplus
}
#endif
#endif

