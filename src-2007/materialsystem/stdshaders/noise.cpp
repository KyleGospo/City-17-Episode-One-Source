//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "screenspace_simple_vs20.inc"
#include "noise_ps20.inc"
#include "noise_ps20b.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER( Noise, "Help for Noise" )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( NOISE_INTENSITY, SHADER_PARAM_TYPE_FLOAT, "0.4", "" )
		SHADER_PARAM( SCAN_INTENSITY, SHADER_PARAM_TYPE_FLOAT, "0.1", "" )
		SHADER_PARAM( SCAN_AMOUNT, SHADER_PARAM_TYPE_FLOAT, "2048", "" )
		SHADER_PARAM( NOISE_SATURATION, SHADER_PARAM_TYPE_FLOAT, "3.75", "" )
		SHADER_PARAM( NOISE_BRIGHTNESS, SHADER_PARAM_TYPE_FLOAT, "-0.3", "" )
		SHADER_PARAM( SHOW_GRAIN_CONTROL, SHADER_PARAM_TYPE_INTEGER, "0", "" )
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

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			int fmt = VERTEX_POSITION;
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

			// Pre-cache shaders
			DECLARE_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_STATIC_PIXEL_SHADER( noise_ps20b );
				SET_STATIC_PIXEL_SHADER( noise_ps20b );
			}
			else
			{
				DECLARE_STATIC_PIXEL_SHADER( noise_ps20 );
				SET_STATIC_PIXEL_SHADER( noise_ps20 );
			}
		}

		DYNAMIC_STATE
		{
			float curTime = pShaderAPI->CurrentTime();
			float g_const0[4] = { params[NOISE_INTENSITY]->GetFloatValue(), params[SCAN_INTENSITY]->GetFloatValue(), params[SCAN_AMOUNT]->GetFloatValue(), curTime };
			pShaderAPI->SetPixelShaderConstant( 0, g_const0 );

			float g_const1[4] = { params[NOISE_SATURATION]->GetFloatValue(), params[NOISE_BRIGHTNESS]->GetFloatValue(), 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 1, g_const1 );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( noise_ps20b );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( SHOWGRAINCONTROL, params[SHOW_GRAIN_CONTROL]->GetIntValue() );
				SET_DYNAMIC_PIXEL_SHADER( noise_ps20b );
			}
			else
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( noise_ps20 );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( SHOWGRAINCONTROL, params[SHOW_GRAIN_CONTROL]->GetIntValue() );
				SET_DYNAMIC_PIXEL_SHADER( noise_ps20 );
			}
		}
		Draw();
	}
END_SHADER