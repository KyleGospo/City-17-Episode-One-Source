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
#ifndef SM_API_STRING_H
#define SM_API_STRING_H

/*! @file
    Defines API string routines. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @struct smStringHandle
    @brief Used to send and receive strings to API routines.

    @see
    - @ref smStringCreate()    
    - @ref sm_api_handles */
SM_API_DECLARE_HANDLE(smStringHandle);

/*! @brief Creates a new empty smStringHandle.
    @param string The address of an invalid smStringHandle, cannot be 0.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringCreate(smStringHandle *string);

/*! @brief Determines if a string is valid (has been created and not destroyed)
    @param string A valid smStringHandle. 
    @param valid Set to SM_API_TRUE if string has been created and not destroyed, SM_API_FALSE otherwise. 
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringIsValid(smStringHandle string, smBool *valid);

/*! @brief Reads a buffer of characters into a smStringHandle. 

    Characters are read until the first termination character is reached or buf_len characters have been read.
    @param string A valid smStringHandle.
    @param buf User allocated memory buffer, may be 0 if buf_len is also 0 (this will set @a string to be empty)
    @param buf_len Size of memory buffer (bytes).
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringReadBuffer(smStringHandle string, const char *buf, int buf_len);

/*! @brief Reads a buffer of 16-bit wide characters into a smStringHandle.

    The characters in the buffer are interpreted in unicode UTF-16 format.
    Characters are read until the first termination character is reached or buf_len characters have been read.
    @param string A valid smStringHandle.
    @param buf User allocated memory buffer, may be 0 if buf_len is also 0 (this will set @a string to be empty)
    @param buf_len Size of memory buffer (words).
    @return SM_API_OK if the function completed successfully.
    @see
    - http://en.wikipedia.org/wiki/UTF-16 */
SM_API(smReturnCode) smStringReadBufferW(smStringHandle string, const unsigned short *buf, int buf_len);

/*! @brief Set buf to point to the the strings internal UNICODE memory buffer, which is 0 terminated.
    @param string A Valid smStringHandle.
    @param buf Address of a pointer to char. The pointer is set by the routine to point to the internal memory buffer of the smStringHandle.
    @param buf_len A pointer to an int. The value of the int is set to the length of the buffer, including the terminator.
    @return SM_API_OK if the function completed successfully.   
    @note The pointer will remain valid until the string contents are changed. */
SM_API(smReturnCode) smStringGetBufferW(smStringHandle string, unsigned short **buf, int *buf_len);

/*! @brief Writes an ASCII 0 terminated string into the buffer.

    If the provided buffer @a buf is too small to hold the entire string, a truncated string is written.
    @param string A valid smStringHandle.
    @param buf User allocated memory buffer. If 0, this function does nothing.
    @param buf_len Size of the memory buffer (bytes). If 0, this function does nothing.
    @note smStringHandle uses unicode internally. This function interprets into ASCII but may produce non-printable ASCII characters.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringWriteBuffer(smStringHandle string, char *buf, int buf_len);

/*! @brief Writes a unicode UTF-16 0 terminated string into the buffer.

    If the provided buffer @a buf is too small to hold the entire string, a truncated string is written.
    @param string A valid smStringHandle.
    @param buf User allocated memory buffer. If 0, this function does nothing.
    @param buf_len Size of the memory buffer (words). If 0, this function does nothing.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringWriteBufferW(smStringHandle string, unsigned short *buf, int buf_len);

/*! @brief Copies the @a src_string into @a dst_string.
    @param dst_string Destination string, must be valid.
    @param src_string Source string, must be valid.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringCopy(smStringHandle dst_string, smStringHandle src_string);

/*! @brief String equality test.
    @note Two empty strings are considered equal.
    @param string1 A valid smStringHandle 
    @param string2 A valid smStringHandle 
    @param equal Pointer to ane existing smBool. Set to SM_API_TRUE if the two strings contain equal characters, SM_API_FALSE otherwise.
    @return SM_API_FAIL_INVALID_ARGUMENT if either of the strings are invalid or @a equal is 0. */
SM_API(smReturnCode) smStringIsEqual(smStringHandle string1, smStringHandle string2, smBool *equal);

/*! @brief Test if string contains any characters
    @param string A valid smStringHandle
    @param empty Set to SM_API_TRUE if @a string contains 0 characters, SM_API_FALSE if not empty.
    @return SM_API_FAIL_INVALID_ARGUMENT if @a string is invalid or @a empty is 0. */
SM_API(smReturnCode) smStringIsEmpty(smStringHandle string, smBool *empty);

/*! @brief Clear the contents of a string (make it empty)
    @param string A valid smStringHandle 
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringClear(smStringHandle string);

/*! @brief Gets the length of the string in characters, not including any termination character.
    @param string A valid smStringHandle
    @param len Set to the length of string. Must point to an existing integer.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringLength(smStringHandle string, int *len);

/*! @brief Destroys an smStringHandle, making it invalid.

    Internal memory used for the string will be released. 
    @param string The address of a valid smStringHandle. 
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smStringDestroy(smStringHandle *string);

#ifdef __cplusplus
}
#endif
#endif
