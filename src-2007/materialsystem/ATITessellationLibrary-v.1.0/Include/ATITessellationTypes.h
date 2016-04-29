//=================================================================================================
//
// Game Computing Applications Group,
// Advanced Micro Devices, Inc.
//
// Type definitions
//                 
//=================================================================================================
// $Id: //depot/3darg/Common/ATITessellation/Include/ATITessellationTypes.h#6 $ 
//
// Last check-in:  $DateTime: 2008/05/21 13:03:09 $ 
// Last edited by: $Author: natasha $
//=================================================================================================
// (C) Advanced Micro Devices, Inc. All rights reserved.               
//=================================================================================================

#if defined( _WINDOWS ) || defined( WIN32 )
   //======================================================
   // 32-bit Windows using the Visual Studio 2005 compiler
   //======================================================
   typedef short   int16;
   typedef int     int32;
   typedef __int64 int64;

   typedef float       float32;
   typedef double      float64;

   //Unsigned
   typedef unsigned short   uint16;
   typedef unsigned int     uint32;
   typedef unsigned __int64 uint64;

#endif   // End of #if defined( _WINDOWS ) || defined( WIN32 )

#ifndef _ATI_TESSELLATION_TYPES_H_   
#define _ATI_TESSELLATION_TYPES_H_

   enum TSMode
   {
      TSMD_ENABLE_CONTINUOUS = 0,
      TSMD_ENABLE_ADAPTIVE,
      TSMD_DISABLE,

      NUM_TS_MODES

   };

   // Defines primitive types supported by tessellation draw-calls
   enum TSPrimitiveType
   {
      TSPT_TRIANGLELIST = 0,
      TSPT_TRIANGLESTRIP,
      TSPT_QUAD,

      NUM_TS_PRIM_TYPES

   }; // End of enum TSPrimitiveType

#endif // _ATI_TESSELLATION_TYPES_H_



