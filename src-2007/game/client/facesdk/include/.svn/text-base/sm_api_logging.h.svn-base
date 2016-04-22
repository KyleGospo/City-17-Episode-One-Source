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
#ifndef SM_API_LOGGING_H
#define SM_API_LOGGING_H

/*! @file 
    Defines routines for handling API log messages. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Signature of function for obtaining logging messages from API functions.

    Define your own function matching this interface and register using smRegisterLoggingCallback() 
    @param user_data Value passed into @a user_data argument in smLoggingRegisterCallback(). 
    @param buf 0 (null) terminated buffer of char. Do not delete / free. 
    @param buf_len Size of the buffer (including the trailing @c '\\0'). 
    @note
    - To construct a std::string from this data you would construct with std::string(buf)
    - This function can be called before smAPIInit() */
typedef void (STDCALL *smLoggingCallback)(void *user_data, const char *buf, int buf_len);

/*! @brief Registers logging callback function. 

    Only one function can be registered at a time.
    @param user_data Can be 0. This value is passed back to your callback routine. 
    It is typically used to pass the 'this' pointer of an object to enable the callback 
    to call a member function in the object.
	@param callback_fn Address of callback function, or 0 to unregister the callback.
	@return SM_API_OK if callback was registered successfully. 
    @note
    - This function can be called before smAPIInit() */
SM_API(smReturnCode) smLoggingRegisterCallback(void *user_data, smLoggingCallback callback_fn);

/*! @brief Set logging to a file enable / disable.

    Logging to file is disabled by default.
    @param enable Set to non-zero to enable logging to a file.
    @see smLoggingGetFileOutputEnable() smLoggingGetPath() smLoggingSetPath() 
    @return SM_API_OK if the logging was enabled successfully.
    @note
    - This function can be called before smAPIInit() */
SM_API(smReturnCode) smLoggingSetFileOutputEnable(int enable);

/*! @brief Get logging to a file enable / disable.

    Logging to file is disabled by default.
    @param enable Pointer to an existing int. Set to SM_API_FALSE when logging to file is disabled, SM_API_TRUE when enabled.
    @see smLoggingSetFileOutputEnable() smLoggingGetPath() smLoggingSetPath() 
    @return SM_API_OK if the value was retreived successfully.
    @note
    - This function can be called before smAPIInit() */
SM_API(smReturnCode) smLoggingGetFileOutputEnable(int *enable);

/*! @brief Gets the path where logfiles are stored.
    @param path Handle to an existing smStringHandle created with smStringCreate(). Set to the path where logfiles are created.
    @see smLoggingSetPath() smLoggingSetFileOutputEnable()
    @return SM_API_OK if the path was obtained successfully.
    @note
    - The API must be initialized before this function can be called. */
SM_API(smReturnCode) smLoggingGetPath(smStringHandle path);

/*! @brief Sets path to directory where logfiles are created.

    The default path is "C:\Documents and Settings\<Username>\Application Data\Seeing Machines\Temp\Run Logs"
    @param path Path to directory where logfiles will be created.
	@return SM_API_OK if the path was set successfully.
    @note
    - The API must be initialized before this function can be called. */
SM_API(smReturnCode) smLoggingSetPath(smStringHandle path);

#ifdef __cplusplus
}
#endif
#endif
