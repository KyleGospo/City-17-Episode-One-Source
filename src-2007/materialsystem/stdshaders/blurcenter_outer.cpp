//============ Copyright © 1996-2005, Gear Development, All rights reserved. =============
//
// Purpose: 
//
//========================================================================================

#include "BaseVSShader.h"
#include "convar.h"

BEGIN_VS_SHADER( BLURCENTER_OUTER, "blurcenter_outer" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( SAMPLEDIST, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
		SHADER_PARAM( SAMPLESTRENGTH, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
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

			pShaderShadow->EnableDepthWrites( false );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, 0, 0 );
			pShaderShadow->SetVertexShader( "screenspace_simple_vs20", 0 );
			pShaderShadow->SetPixelShader( "blurcenter_outer_ps20" );
			DefaultFog();
		}

		DYNAMIC_STATE
		{
			float fSampleDist[4];
			fSampleDist[0] = params[SAMPLEDIST]->GetFloatValue();
			fSampleDist[1] = fSampleDist[2] = fSampleDist[3] = fSampleDist[0];
			pShaderAPI->SetPixelShaderConstant( 0, fSampleDist );

			float fSampleStrength[4];
			fSampleStrength[0] = params[SAMPLESTRENGTH]->GetFloatValue();
			fSampleStrength[1] = fSampleStrength[2] = fSampleStrength[3] = fSampleStrength[0];
			pShaderAPI->SetPixelShaderConstant( 1, fSampleStrength );

			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1);
		}
		Draw();
	}

END_SHADER