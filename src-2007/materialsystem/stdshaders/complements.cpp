//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "screenspace_simple_vs20.inc"
#include "complements_ps20b.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER_FLAGS( Complements, "Help for Complements", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( GUIDEHUE, SHADER_PARAM_TYPE_VEC3, "[1 1 1]", "" )
		SHADER_PARAM( AMOUNT, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( CONCENTRATE, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( DESATCORR, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
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
		if ( g_pHardwareConfig->GetDXSupportLevel() < 90 || !g_pHardwareConfig->SupportsPixelShaders_2_b() )
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

			DECLARE_STATIC_PIXEL_SHADER( complements_ps20b );
			SET_STATIC_PIXEL_SHADER( complements_ps20b );
		}

		DYNAMIC_STATE
		{
			SetPixelShaderConstant( 0, GUIDEHUE );
		
			float fAmount[4];
			fAmount[0] = params[AMOUNT]->GetFloatValue();
			fAmount[1] = fAmount[2] = fAmount[3] = fAmount[0];
			pShaderAPI->SetPixelShaderConstant( 1, fAmount );

			float fConcentrate[4];
			fConcentrate[0] = params[CONCENTRATE]->GetFloatValue();
			fConcentrate[1] = fConcentrate[2] = fConcentrate[3] = fConcentrate[0];
			pShaderAPI->SetPixelShaderConstant( 2, fConcentrate );

			float fDesatCorr[4];
			fDesatCorr[0] = params[DESATCORR]->GetFloatValue();
			fDesatCorr[1] = fDesatCorr[2] = fDesatCorr[3] = fDesatCorr[0];
			pShaderAPI->SetPixelShaderConstant( 3, fDesatCorr );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE );

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_simple_vs20 );

			DECLARE_DYNAMIC_PIXEL_SHADER( complements_ps20b );
			SET_DYNAMIC_PIXEL_SHADER( complements_ps20b );
		}
		Draw();
	}
END_SHADER