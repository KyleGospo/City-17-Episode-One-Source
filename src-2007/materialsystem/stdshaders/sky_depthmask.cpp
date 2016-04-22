//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "screenspace_simple_vs20.inc"
#include "sky_depthmask_ps20.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER_FLAGS( SKY_DEPTHMASK, "Help for Sky_DepthMask", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( DEPTHBUFFER, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameDepth_Alt", "" )
		SHADER_PARAM( FRAMEBUFFER, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( LUMINANCE, SHADER_PARAM_TYPE_FLOAT, "0.44", "" )
		SHADER_PARAM( WHITECUTOFF, SHADER_PARAM_TYPE_FLOAT, "0.20", "" )
		SHADER_PARAM( MIDDLEGRAY, SHADER_PARAM_TYPE_FLOAT, "0.99", "" )
	END_SHADER_PARAMS

	SHADER_INIT
	{
		if( params[DEPTHBUFFER]->IsDefined() )
		{
			LoadTexture( DEPTHBUFFER );
		}
		if( params[FRAMEBUFFER]->IsDefined() )
		{
			LoadTexture( FRAMEBUFFER );
		}
	}
	
	SHADER_FALLBACK
	{
		// Requires DX9 + above
		if ( g_pHardwareConfig->GetDXSupportLevel() < 90 )
		{
			Assert( 0 );
			return "Wireframe";
		}
		return 0;
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->EnableDepthWrites( false );
			int fmt = VERTEX_POSITION;
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

			DECLARE_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );

			DECLARE_STATIC_PIXEL_SHADER( sky_depthmask_ps20 );
			SET_STATIC_PIXEL_SHADER( sky_depthmask_ps20 );
		}

		DYNAMIC_STATE
		{
			float zDeltaFar = pShaderAPI->GetFloatRenderingParameter( FLOAT_RENDERPARM_FARZ );
			float zDeltaNear = pShaderAPI->GetFloatRenderingParameter( FLOAT_RENDERPARM_NEARZ );
			float depthrange[2] = {zDeltaNear, zDeltaFar};
			pShaderAPI->SetPixelShaderConstant( 0, depthrange );

			float g_const1[4] = { params[LUMINANCE]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 1, g_const1 );

			float g_const2[4] = { params[WHITECUTOFF]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 2, g_const2 );

			float g_const3[4] = { params[MIDDLEGRAY]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 3, g_const3 );

			BindTexture( SHADER_SAMPLER0, DEPTHBUFFER );
			BindTexture( SHADER_SAMPLER1, FRAMEBUFFER );

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );

			DECLARE_DYNAMIC_PIXEL_SHADER( sky_depthmask_ps20 );
			SET_DYNAMIC_PIXEL_SHADER( sky_depthmask_ps20 );
		}
		Draw();
	}
END_SHADER