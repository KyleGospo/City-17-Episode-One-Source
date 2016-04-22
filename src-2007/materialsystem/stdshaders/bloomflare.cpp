//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "screenspace_simple_vs30.inc"
#include "bloomflare_ps30.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER_FLAGS( Bloom_Flare, "Help for Bloom Flare", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( BLURTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_SmallHDR0", "" )
		SHADER_PARAM( TEXSCALE, SHADER_PARAM_TYPE_FLOAT, "2.0", "Texture Scale" )
		SHADER_PARAM( BLURSCALE, SHADER_PARAM_TYPE_FLOAT, "0.15", "Blur Scale" )
		SHADER_PARAM( TINT, SHADER_PARAM_TYPE_VEC4, "[0.7 0.2 0.9 1.0]", "Color applied to the blurred scene" )
		SHADER_PARAM( THRESHOLD, SHADER_PARAM_TYPE_FLOAT, "0.1", "Threshold" )
	END_SHADER_PARAMS

	SHADER_INIT
	{
		if( params[BLURTEXTURE]->IsDefined() )
		{
			LoadTexture( BLURTEXTURE );
		}
	}
	
	SHADER_FALLBACK
	{
		// Requires DX9 + above
		if ( g_pHardwareConfig->GetDXSupportLevel() < 90 || !g_pHardwareConfig->SupportsShaderModel_3_0() )
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

			DECLARE_STATIC_VERTEX_SHADER( screenspace_simple_vs30 );
			SET_STATIC_VERTEX_SHADER( screenspace_simple_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( bloomflare_ps30 );
			SET_STATIC_PIXEL_SHADER( bloomflare_ps30 );
		}

		DYNAMIC_STATE
		{
			//Tex Scale - 0
			float g_const0[4] = { params[TEXSCALE]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 0, g_const0 );

			//Blur Scale - 1
			float g_const1[4] = { params[BLURSCALE]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 1, g_const1 );

			//Resolution - 2
			int nWidth, nHeight;
			pShaderAPI->GetBackBufferDimensions( nWidth, nHeight );
			float g_const2[4] = { float(nWidth/4), float(nHeight/4), 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 2, g_const2 );

			//Tint - 3
			SetPixelShaderConstant( 3, TINT );

			//Threshold - 4
			float g_const4[4] = { params[THRESHOLD]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 4, g_const4 );

			BindTexture( SHADER_SAMPLER0, BLURTEXTURE );

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( bloomflare_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( bloomflare_ps30 );
		}
		Draw();
	}
END_SHADER