//===== Copyright © 2011, GearDev Software, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//======================================================================//

#include "BaseVSShader.h"

#include "passthru_vs30.inc"
#include "sun_shafts_ps30.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER_FLAGS( Sun_Shafts, "Help for Sun Shafts", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( SUNBUFFER, SHADER_PARAM_TYPE_TEXTURE, "_rt_SunShaftBlack", "" )
		SHADER_PARAM( LIGHTVECTOR, SHADER_PARAM_TYPE_VEC2, "[0 0]", "" )
		SHADER_PARAM( DENSITY, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( DECAY, SHADER_PARAM_TYPE_FLOAT, "0.99", "" )
		SHADER_PARAM( WEIGHT, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( EXPOSURE, SHADER_PARAM_TYPE_FLOAT, "0.15", "" )
		SHADER_PARAM( SUNBEHIND, SHADER_PARAM_TYPE_BOOL, "0", "" )
		SHADER_PARAM( QUALITY, SHADER_PARAM_TYPE_INTEGER, "1", "" )
	END_SHADER_PARAMS
	SHADER_INIT_PARAMS()
	{
		if( !params[LIGHTVECTOR]->IsDefined() )
		{
			params[LIGHTVECTOR]->SetVecValue( 1.0f, 1.0f );
		}
	}

	SHADER_INIT
	{
		if( params[SUNBUFFER]->IsDefined() )
		{
			LoadTexture( SUNBUFFER );
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

			DECLARE_STATIC_VERTEX_SHADER( passthru_vs30 );
			SET_STATIC_VERTEX_SHADER( passthru_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( sun_shafts_ps30 );
			SET_STATIC_PIXEL_SHADER( sun_shafts_ps30 );
		}

		DYNAMIC_STATE
		{
			SetPixelShaderConstant( 0, LIGHTVECTOR );

			float g_const1[4] = { params[DENSITY]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 1, g_const1 );

			float g_const2[4] = { params[DECAY]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 2, g_const2 );

			float g_const3[4] = { params[WEIGHT]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 3, g_const3 );
		
			float g_const4[4] = { params[EXPOSURE]->GetFloatValue(), 0.0f, 0.0f, 0.0f };
			pShaderAPI->SetPixelShaderConstant( 4, g_const4 );

			BindTexture( SHADER_SAMPLER0, SUNBUFFER );

			DECLARE_DYNAMIC_VERTEX_SHADER( passthru_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( passthru_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( sun_shafts_ps30 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( SUNBEHIND, params[SUNBEHIND]->GetIntValue() );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( QUALITY, params[QUALITY]->GetIntValue() );
			SET_DYNAMIC_PIXEL_SHADER( sun_shafts_ps30 );
		}
		Draw();
	}
END_SHADER