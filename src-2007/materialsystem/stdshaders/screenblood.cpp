//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "screenblood_vs30.inc"
#include "screenblood_ps30.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER( ScreenBlood, "Help for Screen Blood" )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
		SHADER_PARAM( SPLATTERTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( SEED, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( DENSITY, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( SIZE, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( SHININESS, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( DIFFUSECOLOR, SHADER_PARAM_TYPE_COLOR, "[1 1 1]", "" )
		SHADER_PARAM( SPECULARCOLOR, SHADER_PARAM_TYPE_COLOR, "[1 1 1]", "" )
	END_SHADER_PARAMS

	SHADER_INIT
	{
		if( params[FBTEXTURE]->IsDefined() )
		{
			LoadTexture( FBTEXTURE );
		}
		if( params[SPLATTERTEXTURE]->IsDefined() )
		{
			LoadTexture( SPLATTERTEXTURE );
		}
	}
	
	SHADER_FALLBACK
	{
		// Requires SM3.0 or above.
		if ( !g_pHardwareConfig->SupportsShaderModel_3_0() )
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
			int fmt = VERTEX_POSITION;
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

			// Pre-cache shaders
			DECLARE_STATIC_VERTEX_SHADER( screenblood_vs30 );
			SET_STATIC_VERTEX_SHADER( screenblood_vs30 );
			DECLARE_STATIC_PIXEL_SHADER( screenblood_ps30 );
			SET_STATIC_PIXEL_SHADER( screenblood_ps30 );
		}

		DYNAMIC_STATE
		{
			int nWidth, nHeight;
			pShaderAPI->GetBackBufferDimensions( nWidth, nHeight );

			float const0[4]={ float(nWidth),float(nHeight),0,0}; //Window Size
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, const0 );
			
			float fSeed[4];
			fSeed[0] = params[SEED]->GetFloatValue();
			fSeed[1] = fSeed[2] = fSeed[3] = fSeed[0];
			pShaderAPI->SetPixelShaderConstant( 0, fSeed );

			float fDensity[4];
			fDensity[0] = params[DENSITY]->GetFloatValue();
			fDensity[1] = fDensity[2] = fDensity[3] = fDensity[0];
			pShaderAPI->SetPixelShaderConstant( 1, fDensity );

			float fSize[4];
			fSize[0] = params[SIZE]->GetFloatValue();
			fSize[1] = fSize[2] = fSize[3] = fSize[0];
			pShaderAPI->SetPixelShaderConstant( 2, fSize );

			float fShininess[4];
			fShininess[0] = params[SHININESS]->GetFloatValue();
			fShininess[1] = fShininess[2] = fShininess[3] = fShininess[0];
			pShaderAPI->SetPixelShaderConstant( 3, fShininess );

			float fDiffuseColor[3];
			params[DIFFUSECOLOR]->GetVecValue( fDiffuseColor, 3 );
			pShaderAPI->SetPixelShaderConstant( 4, fDiffuseColor, 1 );

			float fSpecularColor[3];
			params[SPECULARCOLOR]->GetVecValue( fSpecularColor, 3 );
			pShaderAPI->SetPixelShaderConstant( 5, fSpecularColor, 1 );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
			BindTexture( SHADER_SAMPLER1, SPLATTERTEXTURE, -1 );
			DECLARE_DYNAMIC_VERTEX_SHADER( screenblood_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( screenblood_vs30 );
			DECLARE_DYNAMIC_PIXEL_SHADER( screenblood_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( screenblood_ps30 );
		}
		Draw();
	}
END_SHADER