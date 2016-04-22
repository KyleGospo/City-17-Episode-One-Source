//======== Copyright © 2009-2010, Denis Baranov, All rights reserved. =========
//
// Purpose: Shader for lightmapped world geometry
//
//=============================================================================

#ifndef LIGHTMAPPEDADV_HELPER_H
#define LIGHTMAPPEDADV_HELPER_H

#include "BaseVSShader.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IMaterialVar;
class IShaderDynamicAPI;
class IShaderShadow;


//-----------------------------------------------------------------------------
// Init params/ init/ draw methods
//-----------------------------------------------------------------------------
struct LightmappedAdv_DX9_Vars_t
{
	LightmappedAdv_DX9_Vars_t() { memset( this, 0xFF, sizeof(LightmappedAdv_DX9_Vars_t) ); }

	int m_nBaseTexture;
	int m_nBaseTextureFrame;
	int m_nBaseTextureTransform;
	int m_nAlbedo;
	int m_nSelfIllumTint;

	int m_nAlpha2; // Hack for DoD srgb blend issues on overlays

	int m_nDetail;
	int m_nDetailFrame;
	int m_nDetailScale;
	int m_nDetailTextureCombineMode;
	int m_nDetailTextureBlendFactor;
	int m_nDetailTint;

	int m_nEnvmap;
	int m_nEnvmapFrame;
	int m_nEnvmapMask;
	int m_nEnvmapMaskFrame;
	int m_nEnvmapMaskTransform;
	int m_nEnvmapTint;
	int m_nBumpmap;
	int m_nBumpFrame;
	int m_nBumpTransform;
	int m_nEnvmapContrast;
	int m_nEnvmapSaturation;
	int m_nFresnelReflection;
	int m_nNoDiffuseBumpLighting;
	int m_nBumpmap2;
	int m_nBumpFrame2;
	int m_nBumpTransform2;
	int m_nBumpMask;
	int m_nBaseTexture2;
	int m_nBaseTexture2Frame;
	int m_nBaseTextureNoEnvmap;
	int m_nBaseTexture2NoEnvmap;
	int m_nDetailAlphaMaskBaseTexture;
	int m_nFlashlightTexture;
	int m_nFlashlightTextureFrame;
	int m_nLightWarpTexture;
	int m_nBlendModulateTexture;
	int m_nMaskedBlending;
	int m_nBlendMaskTransform;
	int m_nSelfShadowedBumpFlag;
	int m_nSeamlessMappingScale;
	int m_nAlphaTestReference;

	int m_nSoftEdges;
	int m_nEdgeSoftnessStart;
	int m_nEdgeSoftnessEnd;

	int m_nOutline;
	int m_nOutlineColor;
	int m_nOutlineAlpha;
	int m_nOutlineStart0;
	int m_nOutlineStart1;
	int m_nOutlineEnd0;
	int m_nOutlineEnd1;

	int m_nPhong;
	int m_nPhongBoost;
	int m_nPhongFresnelRanges;
	int m_nPhongExponent;
};


enum PhongMaskVariant_t
{
	PHONGMASK_NONE,
	PHONGMASK_BASEALPHA,
	PHONGMASK_NORMALALPHA,
	PHONGMASK_STANDALONE,
};

struct LightmappedAdvFlashlight_DX9_Vars_t : public CBaseVSShader::DrawFlashlight_dx90_Vars_t
{
	LightmappedAdvFlashlight_DX9_Vars_t()
		: m_nPhong( -1 )
		, m_nPhongBoost( -1 )
		, m_nPhongFresnelRanges( -1 )
		, m_nPhongExponent( -1 )
		, m_nPhongMask( -1 )
		, m_nPhongMaskFrame( -1 )
	{
	}
	int m_nPhong;
	int m_nPhongBoost;
	int m_nPhongFresnelRanges;
	int m_nPhongExponent;
	int m_nPhongMask;
	int m_nPhongMaskFrame;
};


void InitParamsLightmappedAdv_DX9( CBaseVSShader *pShader, IMaterialVar** params, const char *pMaterialName, LightmappedAdv_DX9_Vars_t &info );
void InitLightmappedAdv_DX9( CBaseVSShader *pShader, IMaterialVar** params, LightmappedAdv_DX9_Vars_t &info );
void DrawLightmappedAdv_DX9( CBaseVSShader *pShader, IMaterialVar** params, 
							 IShaderDynamicAPI *pShaderAPI, IShaderShadow* pShaderShadow, 
							 LightmappedAdv_DX9_Vars_t &info, CBasePerMaterialContextData **pContextDataPtr );


#endif // LIGHTMAPPEDADV_HELPER_H
