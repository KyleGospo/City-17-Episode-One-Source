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
#ifndef SM_API_CONFIGURE_H
#define SM_API_CONFIGURE_H

/*! @file
    Defines OS and compiler specific types and macros. */

// Detect OS
#if !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#    define SM_API_OS_WIN32
#else
#    error "Library only compatible with Windows"
#endif

// Detect Compiler
#if defined(_MSC_VER)
#    define SM_API_CC_MSVC
#elif defined(__GNUC__)
#    define SM_API_CC_MSVC_CC_GNU
#elif defined(__INTEL_COMPILER)
#    define SM_API_CC_INTEL
#endif

#ifdef SM_API_CC_MSVC
#   define STDCALL __stdcall
#else
#   define STDCALL __attribute((stdcall))__
#endif

// Internal or external build
#ifdef SM_API_DLL_EXPORT
#   ifdef __cplusplus
#       define SM_API(type) __declspec(dllexport) type STDCALL
#   else
#       define SM_API(type) __declspec(dllexport) STDCALL
#   endif
#else
#   ifdef __cplusplus
#       define SM_API(type) __declspec(dllimport) type STDCALL
#   else
#       define SM_API(type) __declspec(dllimport) STDCALL
#   endif
#endif

// Integer types
#ifdef SM_API_CC_MSVC
typedef unsigned __int64 uint64;
typedef __int64 int64;
typedef unsigned __int32 uint32;
typedef __int32 int32;
typedef unsigned __int16 uint16;
typedef __int16 int16;
typedef __int8 int8;
typedef unsigned __int8 uint8;
#else
typedef u_int64_t uint64;
typedef int64_t int64;
typedef u_int32_t uint32;
typedef int32_t int32;
typedef u_int16_t uint16;
typedef int16_t int16;
typedef u_int8_t uint8;
typedef int8_t int8;
#endif

/*! @brief Boolean type. 0 is false, non-zero is true. */
typedef int smBool;
#define SM_API_FALSE 0
#define SM_API_TRUE 1

/*! @brief Macro declaring a handle to an opaque type exposed by the API.
    
    Each declared handle is a pointer to a unique structure. The compiler is therefore able to check that handles for
    one type of object are not passed to functions requiring handles of a different type.
    
    @see
    - @ref sm_api_handles
*/
#define SM_API_DECLARE_HANDLE(name) \
    struct name##__ { int unused; }; \
    typedef struct name##__* name

#endif


