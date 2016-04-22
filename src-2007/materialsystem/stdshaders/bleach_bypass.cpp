//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "screenspace_simple_vs20.inc"
#include "bleach_bypass_ps20b.inc"
#include "bleach_bypass_ps20.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER_FLAGS( Bleach_Bypass, "Help for Bleach Bypass", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( OPACITY, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
	END_SHADER_PARAMS

	SHADER_INIT
	{
		if( params[FBTEXTURE]->IsDefined() )
		{
			LoadTexture( FBTEXTURE );
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

			DECLARE_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_STATIC_PIXEL_SHADER( bleach_bypass_ps20b );
				SET_STATIC_PIXEL_SHADER( bleach_bypass_ps20b );
			}
			else
			{
				DECLARE_STATIC_PIXEL_SHADER( bleach_bypass_ps20 );
				SET_STATIC_PIXEL_SHADER( bleach_bypass_ps20 );
			}
		}

		DYNAMIC_STATE
		{
			float g_const0[4] = { params[OPACITY]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 0, g_const0 );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE );

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( bleach_bypass_ps20b );
				SET_DYNAMIC_PIXEL_SHADER( bleach_bypass_ps20b );
			}
			else
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( bleach_bypass_ps20 );
				SET_DYNAMIC_PIXEL_SHADER( bleach_bypass_ps20 );
			}
		}
		Draw();
	}
END_SHADER