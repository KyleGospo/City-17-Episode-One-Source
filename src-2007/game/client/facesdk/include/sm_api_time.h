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
#ifndef SM_API_TIME_H
#define SM_API_TIME_H

/*! @file
    Defines time-related types and functions. */
#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Defines a time value. */
typedef struct smTime
{
    int64 time_s;   /*!< Seconds part of time value. */
    int32 time_us;  /*!< Microseconds part of time value. */
} smTime;

/*! @brief Sets the Coordinated Universal Time into the time value.

    This function is @a reentrant.
    @param time Set to a precise drift-corrected UTC time measurement derived from the system real-time clock and the front-side-bus clock. 
    @see http://en.wikipedia.org/wiki/Coordinated_Universal_Time */
SM_API(smReturnCode) smTimeSetFromUTC(smTime *time);

/*! @brief Reformulates smTime::time_s and smTime::time_us so they are both positive or both negative, and within range. 
    
    This function is @a reentrant.
    If you directly change the value of the time_s or time_us members call this function to ensure the values are correct. 
    All other time functions that operate on an smTime ensure that the smTime struct is always normalized. 
    @param time Pointer to an existing smTime struct to be normalized.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smTimeNormalize(smTime *time);

/*! @brief Set the seconds part of the time struct, normalizing the struct after the value is set. 

    This function is @a reentrant.
    @param secs The new seconds value.
    @param time Pointer to a smTime struct to set the seconds into. Both the time_s and time_us values can change.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smTimeSetSecondsPart(int64 secs, smTime *time);

/*! @brief Set the microseconds part of the time struct, normalizing the struct after the value is set. 

    This function is @a reentrant.
    @param usecs The new microseconds value.
    @param time Pointer to a smTime struct to set the microseconds into. Both the time_s and time_us values can change.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smTimeSetMicroSecondsPart(int32 usecs, smTime *time);

/*! @brief Convert a value in seconds to an smTime struct 

    This function is @a reentrant.
    @param secs A time in seconds.
    @param time Set from the secs parameter. */
SM_API(smReturnCode) smTimeSetSecs(double secs, smTime *time);

/*! @brief Converts the smTime struct to a seconds value.

    This function is @a reentrant.
    @param time The time to convert.
    @param secs The time in seconds. */
SM_API(smReturnCode) smTimeGetSecs(const smTime* time, double *secs);

/*! @brief Gets the time difference between two time values.

    This function is @a reentrant.
    @param old The old or smallest time value
    @param young The young or newest time value
    @param diff Set to @a young - @a old (positive if @a old has smaller values than @a young). */
SM_API(smReturnCode) smTimeDiff(const smTime* old, const smTime* young, smTime *diff);

/*! @brief Adds two time values.

    This function is @a reentrant.
    @param t1 The first time value
    @param t2 The second time value
    @param add Set to the sum of @a t1 and @a t2. */
SM_API(smReturnCode) smTimeAdd(const smTime* t1, const smTime* t2, smTime *add);

/*! @brief Convert the time to a string of the form: [-]h:m:s:z (e.g. -999:59:59:999, or 1:1:1:1)

    This function is @a reentrant.
    @param time smTime struct to convert.
    @param sm_string Handle to an existing valid string that is rewritten with the time value.
    @return SM_API_OK if the function completed successfully. */
SM_API(smReturnCode) smTimeToString(const smTime* time, smStringHandle sm_string);

#ifdef __cplusplus
}
#endif
#endif

