//=================================================================================================
//
// Game Computing Applications Group
// Advanced Micro Devices, Inc.
//
// Declaration file for AMD tessellation extension interface for D3D9
//
//=================================================================================================
// (C) Advanced Micro Devices, Inc. All rights reserved.
//=================================================================================================

#ifndef _IATI_TESS_D3D9_
#define _IATI_TESS_D3D9_

#include <d3d9.h>
#include <d3dx9mesh.h>

#include "ATITessellationTypes.h"

//--------------------------------------------------------------------------------------------------------------
/// An ATI extension interface providing a wrapper for the tessellation functionality in D3D9
///
//--------------------------------------------------------------------------------------------------------------
class IATITessellationD3D9
{

public: 
   
   //=================================================================================================
   /// Factory construction method for IATITessellationD3D9. Constructs a new D3D9 tessellation API 
   /// object given a D3D9 device. Note that users must call the Initialize() method on the object 
   /// immediatedly after construction. The device and D3D objects provided with this construction
   /// call will be stored for the duration of the life span of this object and used for all subsequent
   /// draw calls.
   ///
   /// \sa Create
   ///
   ///    \param pD3D
   ///            D3D9 API object
   ///    \param pD3DDevice
   ///           Active D3D9 device
   ///
   /// Returns a pointer to the tessellation interface or NULL if GPU tessellation is not supported
   /// on the device given.
   //=================================================================================================
   static IATITessellationD3D9* Create( IDirect3D9* pD3D, IDirect3DDevice9* pD3D9Device );

   //=================================================================================================
   // Destructor
   //=================================================================================================
   virtual ~IATITessellationD3D9() {};

   //=================================================================================================
   /// Check tessellation support and query particular features. Once the query has been completed
   /// simply returns the result for subsequent requests for a faster result.
   ///
   /// \sa IsTessellationSupported
   ///
   /// \return True if tessellation is supported, false otherwise
   //=================================================================================================
   virtual bool IsTessellationSupported() = 0;

   //=================================================================================================
   /// Queries for support for a specific tessellation mode. 
   ///
   ///    \param eMode
   ///           Tessellation mode to query support for
   ///
   /// Returns true if the mode is supported, false otherwise
   //=================================================================================================
   virtual bool CheckTessellationModeSupport( TSMode eMode ) = 0;

   //=================================================================================================
   /// Queries for support for a specific primitive type with tessellation for a given tessellation
   /// mode and indexing mode for the draw call.
   ///
   ///    \param eMode
   ///           Tessellation mode 
   ///    \param ePrimType
   ///           Primitive type we're querying support for
   ///    \param bIndexed
   ///           Specifies whether we're querying for an indexed (=true) or a non-indexed (=false) draw call.
   ///
   /// Returns true if the primitive type is supported for the given tessellation mode for the specified
   /// indexing method.
   //=================================================================================================
   virtual bool CheckPrimitiveTypeSupport( TSMode eMode, TSPrimitiveType ePrimType, bool bIndexed ) = 0;

   //=================================================================================================
   /// Queries whether unlimited vertex fetch feature combined with indices retrieval is supported
   /// in the given tessellation mode.
   ///
   ///    \param eMode
   ///           Tessellation mode 
   ///
   /// Returns true if supported and false otherwise.
   //=================================================================================================
   virtual bool CheckUnlimitedFetchSupport( TSMode eMode ) = 0;

   //=================================================================================================
   /// Enable tessellation and set active tessellation mode for subsequent draw-calls.
   ///
   /// \sa SetMode
   ///
   ///    \param eMode
   ///           Tessellation mode parameter, specifies what type of tessellation mode to enable
   ///           or whether tessellation is disabled
   ///    \param dwTessFactorVBSourceIndex
   ///           Optional parameter. This parameter specifies the index of the stream source containing
   ///           tessellation factors during adaptive tessellation draw call. This parameter is ignored
   ///           when enabling continuous tessellation or disabling tessellation.
   ///           Default value is 1, where stream 0 will contain the regular vertex information.
   ///
   /// \return true if tessellation is supported and the set call is successful, false otherwise
   //=================================================================================================
   virtual bool SetMode( TSMode eTessMode, DWORD dwTessFactorVBSourceIndex = 1 ) = 0;

   //=================================================================================================
   /// Sets minimum tessellation levels to be used for subsequent draw calls. Only necessary for
   /// adaptive tessellation draw calls
   ///
   /// \sa SetMinLevel
   ///
   ///    \param fMin
   ///           Minimum tessellation level specified for all draw subsequent calls within
   ///           [ATI_TESS_MIN_TESSELLATION_LEVEL; ATI_TESS_MAX_TESSELLATION_LEVEL] range of values.
   ///
   /// \return true if tessellation is supported and the set call is successful, false otherwise
   //=================================================================================================
   virtual bool SetMinLevel( float32 fMinLevel ) = 0;

   //=================================================================================================
   /// Sets maximum tessellation levels to be used for subsequent draw calls.
   ///
   /// \sa SetMaxLevel
   ///
   ///    \param fMax
   ///           Maximum tessellation level specified for all draw subsequent calls within
   ///           [ATI_TESS_MIN_TESSELLATION_LEVEL; ATI_TESS_MAX_TESSELLATION_LEVEL] range of values.
   ///
   /// \return true if tessellation is supported and the set call is successful, false otherwise
   //=================================================================================================
   virtual bool SetMaxLevel( float32 fMaxLevel ) = 0;

   //=================================================================================================
   /// A draw call for rendering indexed primitives with the tessellation library. This function 
   /// can also be used to render regular geometry (without tessellation). 
   /// However, for tessellation-specific features (such as quad rendering), this method 
   /// can only be used to render draw calls with continuous tessellation enabled, and as such will exit 
   /// with failure code if this condition is not satisfied.
   /// For rendering tessellated objects, this draw call currently only supports continuous tessellation. 
   /// Prior to calling this method, the user must call 'SetMode' and enable tessellation, 
   /// setting tessellation mode to continuous. The user should also specify maximum tessellation level 
   /// prior to this draw call. By default, the maximum is set to ATI_TESS_MAX_TESSELLATION_LEVEL value.
   ///
   /// Note that unlike other calls (such as SetMode, SetMin/MaxLevel) the tessellation library will propagate draw
   /// calls even if tessellation is not enabled.
   ///
   /// \sa DrawIndexed
   ///
   ///    \param ePrimType
   ///           Primitive type. 
   ///    \param nBaseVertexIndex
   ///           Offset from the start of the vertex buffer to the first vertex
   ///    \param nMinVertex
   ///           Minimum vertex index for vertices used during this call. This is a zero based index relative to nBaseVertexIndex.
   ///           Known as 'MinIndex' in Direct3D9 DrawIndexedPrimitive call
   ///    \param nNumVertices
   ///           Number of vertices used during this call. The first vertex is located at index: nBaseVertexIndex + nFirstVertex.
   ///    \param nStartIndex
   ///           Index of the first index to use when accesssing the vertex buffer. Beginning at nFirstIndex to index vertices
   ///           from the vertex buffer. Known as 'StartIndex' in Direct3D9 DrawIndexedPrimitive call
   ///    \param nNumPrimitives
   ///           Number of primitives to render. The number of vertices used is a function of the primitive count and the 
   ///           primitive type ('PrimitiveCount' in Direct3D9 DrawIndexedPrimitive call)
   ///
   ///  \return S_OK upon successful completion and E_FAIL in the following cases:
   ///          - If the specified primitive type is not supported
   ///          - If continuous tessellation is not enabled prior to attempting to draw quad or patch primitives
   ///          - In case of any other D3D9 error messages (check with FAILED( TSDrawIndexed ) for errors)
   ///
   //=================================================================================================
   virtual HRESULT DrawIndexed( TSPrimitiveType   ePrimType,
                                uint32            nBaseVertexIndex,  
                                uint32            nMinVertex,        
                                uint32            nNumVertices,      
                                uint32            nStartIndex,       
                                uint32            nNumPrimitives ) = 0;   


   //=================================================================================================
   /// A draw call for rendering non-indexed primitives with the tessellation library. This method 
   /// can also be used to render regular geometry (without tessellation).
   /// However, for tessellation-specific features (such as quad or patch rendering), this method 
   /// can only be used to render draw calls with tessellation enabled, and as such will exit 
   /// with failure code if this condition is not satisfied. Supports either adaptive or continuous 
   /// tessellation. For rendering tessellated objects, prior to calling this method, the user must call 
   /// 'SetMode' and enable tessellation, setting tessellation mode to continuous or 
   /// adaptive tessellation. The user should also specify maximum tessellation level (and minimum 
   /// for adaptive tessellation) prior to this draw call. By default, the maximum is set to 
   /// ATI_TESS_MAX_TESSELLATION_LEVEL value, and the minimum level is set to 
   /// ATI_TESS_MIN_TESSELLATION_LEVEL value.
   ///
   /// Note that unlike other calls (such as SetMode, SetMin/MaxLevel) the tessellation library will propagate draw
   /// calls even if tessellation is not enabled.
   ///
   /// \sa DrawNonIndexed
   ///
   ///    \param ePrimType
   ///           Primitive type. 
   ///    \param nStartVertex
   ///           Index of the first vertex to load. Beginning at nStartVertex the correct number of vertices will be read out of the vertex buffer.
   ///    \param nNumPrimitives
   ///           Number of primitives to render.
   ///
   ///  \return S_OK upon successful completion and E_FAIL in the following cases:
   ///          - If the specified primitive type is not supported
   ///          - In case of any other D3D9 error messages (check with FAILED( TSDrawNonIndexed ) for errors)
   //=================================================================================================
   virtual HRESULT DrawNonIndexed( TSPrimitiveType  ePrimType,
                                   uint32           nStartVertex,
                                   uint32           nNumPrimitives ) = 0;

   //=================================================================================================
   /// This draw call is used to draw a given subset for a ID3DXMesh with the tessellation library. 
   /// This method can be used to render non-tessellated as well as tessellated objects. In order
   /// to render tessellated objects, prior to calling this method, the user must call 
   /// 'SetMode' and enable tessellation, setting tessellation mode to continuous. 
   /// The user should also specify maximum tessellation level prior to this draw call. By default, 
   /// the maximum is set to ATI_TESS_MAX_TESSELLATION_LEVEL value. The subset is always drawn as an 
   /// indexed D3DPT_TRIANGLELIST. 
   ///
   /// Note that unlike other calls (such as SetMode, SetMin/MaxLevel) the tessellation library will propagate draw
   /// calls even if tessellation is not enabled.
   /// 
   /// \sa DrawMeshSubset
   ///
   ///    \param pMesh     ID3DXMesh to render
   ///    \param nSubset   Specifies which subset of the mesh to render with this draw call
   /// 
   ///  \return S_OK upon successful completion and E_FAIL in the following cases:
   ///          - If tessellation is enabled and the tessellation mode is not set to continuous
   ///          - If the mesh subset draw call failed during the DirectX run-time call (see 
   ///            ID3DXMesh::DrawSubset(..) for description of possible errors
   //=================================================================================================
   virtual HRESULT DrawMeshSubset( ID3DXMesh* pMesh, uint32 nSubset ) = 0;

   //=================================================================================================
   /// Specifies tessellation factors stream source ID. This method is only useful for adaptive
   /// tessellation mode, although no error will be generated if called in continous mode, it will simply
   /// not have any effect.
   ///
   /// \sa SetTessellationFactorsVBSource
   ///
   ///    \param dwTessFactorVBSourceIndex
   ///           This parameter specifies the index of the stream source containing tessellation factors
   ///           during adaptive tessellation draw call. This parameter is ignored when enabling
   ///           continuous tessellation. Default value is 1, where stream 0 will contain the regular
   ///           vertex information.
   /// \return True if tessellation is supported and the set call is successful, false otherwise
   //=================================================================================================
   virtual bool SetTessellationFactorsVBSource( DWORD dwTessFactorVBSourceIndex = 1 ) = 0;


   //=================================================================================================
   /// This method computes the number of primitives generated by tessellation for a given number of
   /// input primitives from the super-primitive mesh (low-resolution mesh). Note that this is only
   /// valid in continuous tessellation mode. Uses currently set maximum tessellation level.
   ///
   /// \sa ComputeNumTessellatedPrimitives
   ///
   ///   \param eTSPrimType         Primitive type for the tessellated draw call 
   ///   \param nInputPrimitives    Number of input primitives
   ///
   //=================================================================================================
   virtual uint32 ComputeNumTessellatedPrimitives( TSPrimitiveType eTSPrimType, uint32 nInputPrimitives ) = 0;

   //=================================================================================================
   /// This method computes the number of primitives generated by tessellation for a given number of
   /// input primitives from the super-primitive mesh (low-resolution mesh) and selected tessellation
   /// factor.
   ///
   /// \sa ComputeNumTessellatedPrimitives
   ///
   ///    \param eTSPrimType            Primitive type for the tessellated draw call 
   ///    \param nInputPrimitives       Number of input primitives
   ///    \param fTessellationLevel     Tessellation level to compute number of primitives from
   ///
   ///
   //=================================================================================================
   virtual uint32 ComputeNumTessellatedPrimitives( TSPrimitiveType eTSPrimType, uint32 nInputPrimitives, float32 fTessellationLevel ) = 0;
   
   //=================================================================================================
   /// This method toggles whether the vertex indices of the original input low-resolution mesh
   /// are propagated a vertex input register (BLENDINDICES semantic) to be used by the 
   /// post-tessellation evaluation vertex shader. Then the programmer can use these vertex indices 
   /// to retrieve the data directly in the shader from a vertex sampler-bound texture. 
   /// When this state is enabled and a texture storing the contents of the vertex buffer 
   /// is mapped as a vertex sampler, the shader writer can retrieve any number of vertex data components 
   /// for the original mesh vertices and thus not be limited by DirectX9 limitation of 16 vertex inputs.
   /// Note that when this state is enabled, indices are propagated using the index buffer (if used) 
   /// or just the order in the submitted vertex buffer (for non-indexed draw calls). 
   ///
   /// \sa ToggleIndicesRetrieval
   ///
   ///    \param bEnable      Toggles unlimited vertex data fetch state
   ///
   /// \return True if tessellation is supported and the set call is successful, false otherwise
   //=================================================================================================
   virtual bool ToggleIndicesRetrieval( bool bEnable ) = 0;


   static float32 ATI_TESS_MIN_TESSELLATION_LEVEL;    
   static float32 ATI_TESS_MAX_TESSELLATION_LEVEL;


}; // End of class IATITessellationD3D9 declaration



#endif   // End of _IATI_TESS_D3D9_
