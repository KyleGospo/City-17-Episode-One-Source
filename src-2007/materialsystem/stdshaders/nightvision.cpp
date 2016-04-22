//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"
#include "vstdlib/random.h"

#include "screenspace_simple_vs20.inc"
#include "nightvision_ps20.inc"
#include "nightvision_ps20b.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER( Nightvision, "Help for Nightvision" )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( FINENOISETEX, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( SPECTRUMTEX, SHADER_PARAM_TYPE_TEXTURE, "", "" )
	END_SHADER_PARAMS

	SHADER_INIT
	{
		if( params[FBTEXTURE]->IsDefined() )
		{
			LoadTexture( FBTEXTURE );
		}
		if( params[FINENOISETEX]->IsDefined() )
		{
			LoadTexture( FINENOISETEX );
		}
		if( params[SPECTRUMTEX]->IsDefined() )
		{
			LoadTexture( SPECTRUMTEX );
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
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
			int fmt = VERTEX_POSITION;
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

			// Pre-cache shaders
			DECLARE_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_STATIC_VERTEX_SHADER( screenspace_simple_vs20 );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_STATIC_PIXEL_SHADER( nightvision_ps20b );
				SET_STATIC_PIXEL_SHADER( nightvision_ps20b );
			}
			else
			{
				DECLARE_STATIC_PIXEL_SHADER( nightvision_ps20 );
				SET_STATIC_PIXEL_SHADER( nightvision_ps20 );
			}
		}

		DYNAMIC_STATE
		{
			float curTime = pShaderAPI->CurrentTime();
			float g_const0[4] = { curTime, 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 0, g_const0 );

			float g_const1[4] = { RandomFloat(0, 1.0), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 1, g_const1 );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
			BindTexture( SHADER_SAMPLER1, FINENOISETEX, -1 );
			BindTexture( SHADER_SAMPLER2, SPECTRUMTEX, -1 );
			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( nightvision_ps20b );
				SET_DYNAMIC_PIXEL_SHADER( nightvision_ps20b );
			}
			else
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( nightvision_ps20 );
				SET_DYNAMIC_PIXEL_SHADER( nightvision_ps20 );
			}
		}
		Draw();
	}
END_SHADER