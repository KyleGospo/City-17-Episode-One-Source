//============ Copyright © 1996-2005, Gear Development, All rights reserved. =============
//
// Purpose: 
//
//========================================================================================

#include "BaseVSShader.h"
#include "convar.h"

BEGIN_VS_SHADER( VIGNETTING_COMBINE, "vignetting_combine" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( VIGNETTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( VIGNETSTRENGTH, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
	END_SHADER_PARAMS

	SHADER_INIT
	{	
		if( params[FBTEXTURE]->IsDefined() )
		{
			LoadTexture( FBTEXTURE );
		}

		if( params[VIGNETTEXTURE]->IsDefined() )
		{
			LoadTexture( VIGNETTEXTURE );
		}
	}

	SHADER_FALLBACK
	{
		// Requires DX8 + above
		if (!g_pHardwareConfig->SupportsVertexAndPixelShaders())
			return "Wireframe";
		return 0;
	}

	bool NeedsFrameBufferTexture( IMaterialVar **params ) const
	{
		return true;
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{	
			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

			pShaderShadow->EnableDepthWrites( false );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, 0, 0 );
			pShaderShadow->SetVertexShader( "vignetting_vs20", 0 );
			pShaderShadow->SetPixelShader( "vignetting_combine_ps20" );
			DefaultFog();
		}

		DYNAMIC_STATE
		{
			float fVignetStrength[4];
			fVignetStrength[0] = params[VIGNETSTRENGTH]->GetFloatValue();
			fVignetStrength[1] = fVignetStrength[2] = fVignetStrength[3] = fVignetStrength[0];
			pShaderAPI->SetPixelShaderConstant( 0, fVignetStrength );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1);
			BindTexture( SHADER_SAMPLER1, FBTEXTURE, -1);
		}
		Draw();
	}

END_SHADER