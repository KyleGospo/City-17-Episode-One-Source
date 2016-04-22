#include "cbase.h"
#include "screenspaceeffects.h"
#include "rendertexture.h"
#include "model_types.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterialvar.h"
#include "cdll_client_int.h"
#include "materialsystem/itexture.h"
#include "keyvalues.h"
#include "ClientEffectPrecacheSystem.h"
#include "viewrender.h"
#include "view_scene.h"
#include "c_basehlplayer.h"
#include "tier0/vprof.h"
#include "view.h"
#include "hl2_gamerules.h"

#include "c17_screeneffects.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar fov_desired;
extern ConVar r_post_healtheffects_radial;
extern CViewRender g_DefaultViewRender;
extern bool g_bRenderingScreenshot;

void CUnsharpEffect::Init( void )
{
	m_UnsharpBlurFB.InitRenderTarget( ScreenWidth()/2, ScreenHeight()/2, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_NONE, false, "_rt_UnsharpBlur" );

	PrecacheMaterial( "effects/shaders/unsharp_blur" );
	PrecacheMaterial( "effects/shaders/unsharp" );

	m_UnsharpBlur.Init( materials->FindMaterial("effects/shaders/unsharp_blur", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_Unsharp.Init( materials->FindMaterial("effects/shaders/unsharp", TEXTURE_GROUP_PIXEL_SHADERS, true) );
}

void CUnsharpEffect::Shutdown( void )
{
	m_UnsharpBlurFB.Shutdown();
	m_UnsharpBlur.Shutdown();
	m_Unsharp.Shutdown();
}

ConVar r_post_unsharp( "r_post_unsharp", "1", FCVAR_ARCHIVE );
ConVar r_post_unsharp_debug( "r_post_unsharp_debug", "0", FCVAR_CHEAT );
ConVar r_post_unsharp_strength( "r_post_unsharp_strength", "0.3", FCVAR_CHEAT );
ConVar r_post_unsharp_blursize( "r_post_unsharp_blursize", "5.0", FCVAR_CHEAT );
void CUnsharpEffect::Render( int x, int y, int w, int h )
{
	VPROF( "CUnsharpEffect::Render" );

	if( !r_post_unsharp.GetBool() || ( IsEnabled() == false ) )
		return;

	// Grab the render context
	CMatRenderContextPtr pRenderContext( materials );

	// Set to the proper rendering mode.
	pRenderContext->MatrixMode( MATERIAL_VIEW );
	pRenderContext->PushMatrix();
	pRenderContext->LoadIdentity();
	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->PushMatrix();
	pRenderContext->LoadIdentity();

	IMaterialVar *var;
	var = m_UnsharpBlur->FindVar( "$blursize", NULL );
	var->SetFloatValue( r_post_unsharp_blursize.GetFloat() );

	if( r_post_unsharp_debug.GetBool() )
	{
		DrawScreenEffectMaterial( m_UnsharpBlur, x, y, w, h );
		return;
	}

	Rect_t actualRect;
	UpdateScreenEffectTexture( 0, x, y, w, h, false, &actualRect );
	pRenderContext->PushRenderTargetAndViewport( m_UnsharpBlurFB );
		DrawScreenEffectQuad( m_UnsharpBlur, m_UnsharpBlurFB->GetActualWidth(), m_UnsharpBlurFB->GetActualHeight() );
	pRenderContext->PopRenderTargetAndViewport();

	//Restore our state
	pRenderContext->MatrixMode( MATERIAL_VIEW );
	pRenderContext->PopMatrix();
	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->PopMatrix();

	var = m_Unsharp->FindVar( "$fbblurtexture", NULL );
	var->SetTextureValue( m_UnsharpBlurFB );
	var = m_Unsharp->FindVar( "$unsharpstrength", NULL );
	var->SetFloatValue( r_post_unsharp_strength.GetFloat() );
	var = m_Unsharp->FindVar( "$blursize", NULL );
	var->SetFloatValue( r_post_unsharp_blursize.GetFloat() );

	DrawScreenEffectMaterial( m_Unsharp, x, y, w, h );
}

void CSSAOEffect::Init( void )
{
	m_SSAOFB0.InitRenderTarget( ScreenWidth()/2, ScreenHeight()/2, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_NONE, false, "_rt_SSAOFB0" );
	m_SSAOFB1.InitRenderTarget( ScreenWidth()/2, ScreenHeight()/2, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_NONE, false, "_rt_SSAOFB1" );
	m_SSAOFB2.InitRenderTarget( ScreenWidth()/2, ScreenHeight()/2, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_NONE, false, "_rt_SSAOFB2" );

	PrecacheMaterial( "debug/showz" );

	m_ShowZ.Init( materials->FindMaterial("debug/showz", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_SSAOBlur.Init( materials->FindMaterial("effects/shaders/ssao_blur", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_SSAO.Init( materials->FindMaterial("effects/shaders/ssao_unsharp", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_SSAOCombine.Init( materials->FindMaterial("effects/shaders/ssao_final", TEXTURE_GROUP_PIXEL_SHADERS, true) );
}

void CSSAOEffect::Shutdown( void )
{
	m_SSAOFB0.Shutdown();
	m_SSAOFB1.Shutdown();
	m_SSAOFB2.Shutdown();

	m_ShowZ.Shutdown();
	m_SSAOBlur.Shutdown();
	m_SSAO.Shutdown();
	m_SSAOCombine.Shutdown();
}

ConVar r_post_ssao( "r_post_ssao", "0", FCVAR_ARCHIVE );
ConVar r_post_ssao_debug( "r_post_ssao_debug", "0", FCVAR_CHEAT );
ConVar r_post_ssao_blursize( "r_post_ssao_blursize", "0.5", FCVAR_CHEAT );
ConVar r_post_ssao_darkening( "r_post_ssao_darkening", "0", FCVAR_CHEAT, 0, true, 0.0, true, 12.0 );
ConVar r_post_ssao_darkening_edge( "r_post_ssao_darkening_edge", "0", FCVAR_CHEAT, 0, true, -4.0, true, 28.0 );
void CSSAOEffect::Render( int x, int y, int w, int h )
{
	VPROF( "CSSAOEffect::Render" );

	if( !r_post_ssao.GetBool() || ( IsEnabled() == false ) )
		return;

	// Grab the render context
	CMatRenderContextPtr pRenderContext( materials );

	// Set to the proper rendering mode.
	pRenderContext->MatrixMode( MATERIAL_VIEW );
	pRenderContext->PushMatrix();
	pRenderContext->LoadIdentity();
	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->PushMatrix();
	pRenderContext->LoadIdentity();

	pRenderContext->PushRenderTargetAndViewport( m_SSAOFB0 );
		pRenderContext->DrawScreenSpaceQuad( m_ShowZ );
	pRenderContext->PopRenderTargetAndViewport();

	IMaterialVar *var;
	var = m_SSAOBlur->FindVar( "$blursize", NULL );
	var->SetFloatValue( r_post_ssao_blursize.GetFloat() );

	pRenderContext->PushRenderTargetAndViewport( m_SSAOFB1 );
		pRenderContext->DrawScreenSpaceQuad( m_SSAOBlur );
	pRenderContext->PopRenderTargetAndViewport();

	//Restore our state
	pRenderContext->MatrixMode( MATERIAL_VIEW );
	pRenderContext->PopMatrix();
	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->PopMatrix();

	Rect_t actualRect;
	UpdateScreenEffectTexture( 0, x, y, w, h, false, &actualRect );
	pRenderContext->PushRenderTargetAndViewport( m_SSAOFB2 );
		pRenderContext->DrawScreenSpaceQuad( m_SSAO );
	pRenderContext->PopRenderTargetAndViewport();

	var = m_SSAOCombine->FindVar( "$maskstrength", NULL );
	var->SetFloatValue( r_post_ssao_darkening.GetFloat() );
	var = m_SSAOCombine->FindVar( "$edgestrength", NULL );
	var->SetFloatValue( r_post_ssao_darkening_edge.GetFloat() );

	DrawScreenEffectMaterial( m_SSAOCombine, x, y, w, h );
}

ConVar r_post_sunshaft_blur( "r_post_sunshaft_blur", "1", FCVAR_ARCHIVE );
ConVar r_post_sunshaft_blur_amount( "r_post_sunshaft_blur_amount", "0.5", FCVAR_CHEAT );
void CSunShaftEffect::Init( void )
{
	PrecacheMaterial( "effects/shaders/blurx" );
	PrecacheMaterial( "effects/shaders/blury" );
	PrecacheMaterial( "effects/shaders/sunshaft_base" );
	PrecacheMaterial( "effects/shaders/sunshaft_final" );

	m_SunShaft_BlurX.Init( materials->FindMaterial("effects/shaders/blurx", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_SunShaft_BlurY.Init( materials->FindMaterial("effects/shaders/blury", TEXTURE_GROUP_PIXEL_SHADERS, true) );

	m_SunShaftBlendMat.Init( materials->FindMaterial("effects/shaders/sunshaft_final", TEXTURE_GROUP_CLIENT_EFFECTS, true) );
	m_SunShaftMask.Init( materials->FindMaterial("effects/shaders/sunshaft_base", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_SunShaftDebug.Init( materials->FindMaterial("effects/shaders/sunshaft_base", TEXTURE_GROUP_CLIENT_EFFECTS, true) );
}

void CSunShaftEffect::Shutdown( void )
{
	m_SunShaft_BlurX.Shutdown();
	m_SunShaft_BlurY.Shutdown();

	m_SunShaftBlendMat.Shutdown();

	m_SunShaftMask.Shutdown();
	m_SunShaftDebug.Shutdown();
}

extern ConVar r_post_sunshaft;
ConVar r_post_sunshaft_debug( "r_post_sunshaft_debug", "0", FCVAR_CHEAT );

bool CSunShaftEffect::ShaftsRendering( void )
{
	return ( r_post_sunshaft.GetBool() && engine->IsSkyboxVisibleFromPoint( CurrentViewOrigin() ) && IsEnabled() );
}

static void SetRenderTargetAndViewPort(ITexture *rt)
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetRenderTarget(rt);
	pRenderContext->Viewport(0,0,rt->GetActualWidth(),rt->GetActualHeight());
}

void CSunShaftEffect::Render( int x, int y, int w, int h )
{
	VPROF( "CSunShaftEffect::Render" );

	if( !ShaftsRendering() )
		return;

	if( r_post_sunshaft_debug.GetInt() == 1 )
	{
		DrawScreenEffectMaterial( m_SunShaftMask, x, y, w, h );
		return;
	}

	IMaterialVar *var;
	CMatRenderContextPtr pRenderContext( materials );

	pRenderContext->PushRenderTargetAndViewport();

	ITexture *dest_rt0 = materials->FindTexture( "_rt_SmallFB0", TEXTURE_GROUP_RENDER_TARGET );
	ITexture *dest_rt1 = materials->FindTexture( "_rt_SmallFB1", TEXTURE_GROUP_RENDER_TARGET );

	SetRenderTargetAndViewPort( dest_rt0 );

	pRenderContext->DrawScreenSpaceRectangle( m_SunShaftMask, 0, 0, w/4, h/4,
											0, 0, w/4-1, h/4-1,
											w/4, h/4 );

	if ( IsX360() )
	{
		pRenderContext->CopyRenderTargetToTextureEx( dest_rt0, 0, NULL, NULL );
	}

	//Render the gaussian blur pass over our shafts.
	if( r_post_sunshaft_blur.GetBool() )
	{
		var = m_SunShaft_BlurX->FindVar( "$fbtexture", NULL );
		var->SetTextureValue( dest_rt0 );
		var = m_SunShaft_BlurX->FindVar( "$resdivisor", NULL );
		var->SetIntValue( 4 );
		var = m_SunShaft_BlurX->FindVar( "$blursize", NULL );
		var->SetFloatValue( r_post_sunshaft_blur_amount.GetFloat() );

		SetRenderTargetAndViewPort( dest_rt1 );
		pRenderContext->DrawScreenSpaceRectangle(	m_SunShaft_BlurX, 0, 0, w/4, h/4,
													0, 0, w/4-1, h/4-1,
													w/4, h/4 );
		if ( IsX360() )
		{
			pRenderContext->CopyRenderTargetToTextureEx( dest_rt1, 0, NULL, NULL );
		}

		var = m_SunShaft_BlurY->FindVar( "$fbtexture", NULL );
		var->SetTextureValue( dest_rt1 );
		var = m_SunShaft_BlurY->FindVar( "$resdivisor", NULL );
		var->SetIntValue( 4 );
		var = m_SunShaft_BlurY->FindVar( "$blursize", NULL );
		var->SetFloatValue( r_post_sunshaft_blur_amount.GetFloat() );

		SetRenderTargetAndViewPort( dest_rt0 );
		pRenderContext->DrawScreenSpaceRectangle(	m_SunShaft_BlurY, 0, 0, w/4, h/4,
													0, 0, w/4-1, h/4-1,
													w/4, h/4 );
		if ( IsX360() )
		{
			pRenderContext->CopyRenderTargetToTextureEx( dest_rt0, 0, NULL, NULL );
		}
	}

	pRenderContext->PopRenderTargetAndViewport();

	if( r_post_sunshaft_debug.GetInt() == 2 )
	{
		pRenderContext->DrawScreenSpaceRectangle( m_SunShaftDebug, 0, 0, w, h,
												0, 0, w-1, h-1,
												w, h );
		return;
	}

	//Render our sun to the screen additively.
	DrawScreenEffectMaterial( m_SunShaftBlendMat, x, y, w, h );
}

ConVar r_post_watereffects_underwater_chromaticoffset( "r_post_watereffects_underwater_chromaticoffset", "1.0", FCVAR_CHEAT );
ConVar r_post_watereffects_underwater_amount( "r_post_watereffects_underwater_amount", "0.1", FCVAR_CHEAT );
ConVar r_post_watereffects_underwater_viscosity( "r_post_watereffects_underwater_viscosity", "1.0", FCVAR_CHEAT );
ConVar r_post_watereffects_lerp_viscosity( "r_post_watereffects_lerp_viscosity", "0.01", FCVAR_CHEAT );
ConVar r_post_watereffects_lerp_amount( "r_post_watereffects_lerp_amount", "0.005", FCVAR_CHEAT );
ConVar r_post_watereffects_underwater_gaussianamount( "r_post_watereffects_underwater_gaussianamount", "1.5", FCVAR_CHEAT );
void CWaterEffects::Init( void )
{
	fViscosity = 0.01;
	fAmount = 0;
	m_bUnderwater = false;

	PrecacheMaterial( "effects/shaders/chromaticDisp" );
	PrecacheMaterial( "effects/shaders/screenwater" );
	PrecacheMaterial( "effects/shaders/screen_blurx" );
	PrecacheMaterial( "effects/shaders/screen_blury" );

	m_ChromaticDisp.Init( materials->FindMaterial("effects/shaders/chromaticDisp", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_WaterFX.Init( materials->FindMaterial("effects/shaders/screenwater", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_BlurX.Init( materials->FindMaterial("effects/shaders/screen_blurx", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_BlurY.Init( materials->FindMaterial("effects/shaders/screen_blury", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
}

void CWaterEffects::Shutdown( void )
{
	m_ChromaticDisp.Shutdown();
	m_WaterFX.Shutdown();
	m_BlurX.Shutdown();
	m_BlurY.Shutdown();
}

void CWaterEffects::SetParameters( KeyValues *params )
{
	if( IsUnderwater() )
		return;

	float in, temp;

	if( params->FindKey( "amount" ) )
	{
		in = params->GetFloat( "amount" );
		temp = GetAmount();
		temp += in;
		if( temp > 0.1f )
			temp = 0.1f;

		SetAmount( temp );
	}

	if( params->FindKey( "viscosity" ) )
	{
		in = params->GetFloat( "viscosity" );
		temp = GetViscosity();
		temp += in;
		if( temp > 1.0f )
			temp = 1.0f;

		SetViscosity( temp );
	}
}

ConVar r_post_watereffects( "r_post_watereffects", "1", FCVAR_ARCHIVE );
ConVar r_post_watereffects_debug( "r_post_watereffects_debug", "0", FCVAR_CHEAT );
void CWaterEffects::Render( int x, int y, int w, int h )
{
	VPROF( "CWaterEffects::Render" );

	if( !r_post_watereffects.GetBool() || ( IsEnabled() == false ) )
		return;

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if(!pPlayer)
		return;

	IMaterialVar *var;

	if(pPlayer->GetWaterLevel() >= 3)
	{
		m_bUnderwater = true;
		fViscosity = r_post_watereffects_underwater_viscosity.GetFloat();
		fAmount = r_post_watereffects_underwater_amount.GetFloat();

		//Gaussian Blur the screen
		var = m_BlurX->FindVar( "$BLURSIZE", NULL );
		var->SetFloatValue( r_post_watereffects_underwater_gaussianamount.GetFloat() );
		var = m_BlurX->FindVar( "$RESDIVISOR", NULL );
		var->SetIntValue( 1 );
		DrawScreenEffectMaterial( m_BlurX, x, y, w, h );
		var = m_BlurY->FindVar( "$BLURSIZE", NULL );
		var = m_BlurY->FindVar( "$RESDIVISOR", NULL );
		var->SetIntValue( 1 );
		var->SetFloatValue( r_post_watereffects_underwater_gaussianamount.GetFloat() );
		DrawScreenEffectMaterial( m_BlurY, x, y, w, h );

		//Render Chromatic Dispersion
		var = m_ChromaticDisp->FindVar( "$FOCUSOFFSET", NULL );
		var->SetFloatValue( r_post_watereffects_underwater_chromaticoffset.GetFloat() );
		var = m_ChromaticDisp->FindVar( "$radial", NULL );
		var->SetIntValue( 0 );
		DrawScreenEffectMaterial( m_ChromaticDisp, x, y, w, h );
	}
	else
	{
		m_bUnderwater = false;

		if( fViscosity != 0.01 )
			fViscosity = FLerp( fViscosity, 0.01, r_post_watereffects_lerp_viscosity.GetFloat() );
	
		if( fAmount != 0 )
			fAmount = FLerp( fAmount, 0, r_post_watereffects_lerp_amount.GetFloat() );

		if( fAmount < 0.01 )
		{
			if( r_post_watereffects_debug.GetBool() )
			{
				DevMsg( "Water Effects Stopped.\n" );
			}
			return;
		}
	}

	var = m_WaterFX->FindVar( "$AMOUNT", NULL );
	var->SetFloatValue( fAmount );
	var = m_WaterFX->FindVar( "$VISCOSITY", NULL );
	var->SetFloatValue( fViscosity );
	DrawScreenEffectMaterial( m_WaterFX, x, y, w, h );

	if( r_post_watereffects_debug.GetBool() )
	{
		DevMsg( "Water Amount: %.2f\n", fAmount );
		DevMsg( "Water Viscosity: %.2f\n", fViscosity );
	}
}

ConVar r_post_vignetting_darkness( "r_post_vignetting_darkness", "1.25", FCVAR_CHEAT, "Controls the vignetting shader's power. 0 for off." );
ConVar r_post_vignetting_darkness_lowhealth( "r_post_vignetting_darkness_lowhealth", "2.5", FCVAR_CHEAT, "Controls the vignetting shader's power when health is low or the player is in a smoke volume. 0 for off." );
ConVar r_post_vignetting_darkness_dead( "r_post_vignetting_darkness_dead", "3.0", FCVAR_CHEAT, "Controls the vignetting shader's power when the player is dead. 0 for off." );
ConVar r_post_vignetting_darkness_stamina( "r_post_vignetting_darkness_stamina", "2.0", FCVAR_CHEAT, "Controls the vignetting shader's power when the player is low on stamina." );
ConVar r_post_chromatic_dispersion_offset( "r_post_chromatic_dispersion_offset", "1.3", FCVAR_CHEAT, "Controls constant chromatic dispersion strength, 0 for off." );
ConVar r_post_chromatic_dispersion_offset_lowhealth( "r_post_chromatic_dispersion_offset_lowhealth", "1.0", FCVAR_CHEAT, "Controls constant chromatic dispersion strength when health is low. 0 for off." );
ConVar r_post_chromatic_dispersion_offset_heavydamage( "r_post_chromatic_dispersion_offset_heavydamage", "1.5", FCVAR_CHEAT, "Controls constant chromatic dispersion strength when the player takes heavy damage." );
//ConVar r_post_chromatic_dispersion_offset_damage( "r_post_chromatic_dispersion_offset_damage", "8.0", FCVAR_CHEAT, "Controls constant chromatic dispersion strength when the player takes damage." );
ConVar r_post_chromatic_dispersion_offset_fog( "r_post_chromatic_dispersion_offset_fog", "0.3", FCVAR_CHEAT, "Controls constant chromatic dispersion strength when the player is in a fog volume." );
ConVar r_post_chromatic_dispersion_offset_ironsighted( "r_post_chromatic_dispersion_offset_ironsighted", "3.5", FCVAR_CHEAT, "Controls constant chromatic dispersion strength when the player is ironsighted." );
ConVar r_post_healtheffects_debug( "r_post_healtheffects_debug", "0", FCVAR_CHEAT );

ConVar r_post_noise_debug( "r_post_noise_debug", "0", FCVAR_CHEAT );
ConVar r_post_noise_amount( "r_post_noise_amount", "0.4", FCVAR_CHEAT );
ConVar r_post_noise_saturation( "r_post_noise_saturation", "3.75", FCVAR_CHEAT );
ConVar r_post_noise_brightness( "r_post_noise_brightness", "-0.3", FCVAR_CHEAT );
ConVar r_post_scanline_amount( "r_post_scanline_amount", "0", FCVAR_CHEAT );
ConVar r_post_scanline_count( "r_post_scanline_count", "2048", FCVAR_CHEAT );
ConVar r_post_noise_amount_fog( "r_post_noise_amount_fog", "0.6", FCVAR_CHEAT );
ConVar r_post_scanline_amount_fog( "r_post_scanline_amount_fog", "0", FCVAR_CHEAT );

ConVar r_post_stamina_blur_amount( "r_post_stamina_blur_amount", "1.7", FCVAR_CHEAT );
ConVar r_post_stamina_noise_amount( "r_post_stamina_noise_amount", "0.1", FCVAR_CHEAT );
ConVar r_post_stamina_osc_c( "r_post_stamina_osc_c", "1.3", FCVAR_CHEAT );
ConVar r_post_stamina_lerpspeed( "r_post_stamina_lerpspeed", "0.03", FCVAR_CHEAT );

ConVar r_post_radial_blur_width( "r_post_radial_blur_width_pain", "-0.1", FCVAR_CHEAT );

ConVar r_post_desaturate_low_health( "r_post_desaturate_low_health", "0.6", FCVAR_CHEAT );

ConVar r_post_ironsight_blur_amount( "r_post_ironsight_blur_amount", "10.0", FCVAR_CHEAT );

ConVar r_post_ironsight_useconvars( "r_post_ironsight_useconvars", "0", FCVAR_CHEAT );

void CHealthEffects::Init( void )
{
	PrecacheMaterial( "effects/shaders/chromaticDisp" );
	PrecacheMaterial( "effects/shaders/vignetting" );
	PrecacheMaterial( "effects/shaders/noise" );
	PrecacheMaterial( "effects/shaders/screen_blurx" );
	PrecacheMaterial( "effects/shaders/screen_blury" );
	PrecacheMaterial( "effects/shaders/screen_radial_blurx" );
	PrecacheMaterial( "effects/shaders/screen_radial_blury" );
	PrecacheMaterial( "effects/shaders/radial_blur" );
	PrecacheMaterial( "effects/shaders/desaturate" );
	PrecacheMaterial( "effects/shaders/pain" );

	m_ChromaticDisp.Init( materials->FindMaterial("effects/shaders/chromaticDisp", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_VignetMat.Init( materials->FindMaterial("effects/shaders/vignetting", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_Noise.Init( materials->FindMaterial("effects/shaders/noise", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_BlurX.Init( materials->FindMaterial("effects/shaders/screen_blurx", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_BlurY.Init( materials->FindMaterial("effects/shaders/screen_blury", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_RadialBlurX.Init( materials->FindMaterial("effects/shaders/screen_radial_blurx", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_RadialBlurY.Init( materials->FindMaterial("effects/shaders/screen_radial_blury", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_RadialBlurMat.Init( materials->FindMaterial("effects/shaders/radial_blur", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_DesatMat.Init( materials->FindMaterial("effects/shaders/desaturate", TEXTURE_GROUP_PIXEL_SHADERS, true ) );
	m_Pain.Init( materials->FindMaterial("effects/shaders/pain", TEXTURE_GROUP_PIXEL_SHADERS, true ) );

	fVignettingAmount = r_post_vignetting_darkness.GetFloat();
	fDispersionAmount = r_post_chromatic_dispersion_offset.GetFloat();
	fNoiseAmount = r_post_noise_amount.GetFloat();
	fScanAmount = r_post_scanline_amount.GetFloat();
	fVignettingLerpTo = r_post_vignetting_darkness.GetFloat();
	fDispersionLerpTo = r_post_chromatic_dispersion_offset.GetFloat();
	fNoiseLerpTo = r_post_noise_amount.GetFloat();
	fScanLerpTo = r_post_scanline_amount.GetFloat();
	fStaminaBlurAmount = 0;
	fStaminaBlurAmountLerpTo = 0;
	fSightDOF = 0;
	fSightDOFLerpTo = 0;
	fBlurWidth = 0.0f;
	fBlurWidthLerpTo = 0.0f;
	fDesat = 0.0f;
	fDesatLerpTo = 0.0f;
	fPain = 0.0f;
	fPainLerpTo = 0.0f;

	//iLastHealth = -1;
	//bDisableDispersionOneFrame = false;
}

void CHealthEffects::Shutdown( void )
{
	m_ChromaticDisp.Shutdown();
	m_VignetMat.Shutdown();
	m_Noise.Shutdown();
	m_BlurX.Shutdown();
	m_BlurY.Shutdown();
	m_RadialBlurX.Shutdown();
	m_RadialBlurY.Shutdown();
	m_RadialBlurMat.Shutdown();
	m_DesatMat.Shutdown();
	m_Pain.Shutdown();
}

ConVar r_post_healtheffects( "r_post_healtheffects", "1", FCVAR_ARCHIVE );
ConVar r_post_blurfx( "r_post_blurfx", "1", FCVAR_ARCHIVE );
void CHealthEffects::Render( int x, int y, int w, int h )
{
	VPROF( "CHealthEffects::Render" );

	if( !r_post_healtheffects.GetBool() || ( IsEnabled() == false ) )
		return;

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if(!pPlayer)
		return;

	if( pPlayer->HasLowStamina() )
	{
		float fDispC = gpGlobals->curtime * r_post_stamina_osc_c.GetFloat();
		fDispC = cos(fDispC) * cos(fDispC) * cos(fDispC) * cos(fDispC) * cos(fDispC);
		fStaminaBlurAmountLerpTo = r_post_stamina_blur_amount.GetFloat() * fDispC * 60 * gpGlobals->frametime;
		if( !pPlayer->InSmokeVolume() )
		{
			float fNoise = r_post_stamina_noise_amount.GetFloat() * fDispC * 60 * gpGlobals->frametime;
			if( fNoise < 0 )
				fNoise *= -1;

			fNoiseLerpTo = fNoise;
		}
	}
	else
	{
		if( !pPlayer->IsAlive() )
			fStaminaBlurAmountLerpTo = 1.0f;
		else
			fStaminaBlurAmountLerpTo = 0.0f;

		if( !pPlayer->InSmokeVolume() )
			fNoiseLerpTo = r_post_noise_amount.GetFloat();
	}

	if( pPlayer->GetActiveWeapon() && pPlayer->GetActiveWeapon()->IsIronsighted() )
	{
		if( r_post_ironsight_useconvars.GetBool() )
			fSightDOFLerpTo = r_post_ironsight_blur_amount.GetFloat();
		else
			fSightDOFLerpTo = pPlayer->GetActiveWeapon()->GetBlurAmount();
	}
	else
	{
		fSightDOFLerpTo = 0.0f;
	}

	if( pPlayer->GetHealth() < 20 )
	{
		fPainLerpTo = 1.0f;
	}
	else
	{
		fPainLerpTo = 0.0f;
	}

	if( pPlayer->InSmokeVolume() )
	{
		fVignettingLerpTo = r_post_vignetting_darkness_lowhealth.GetFloat();
		fDispersionLerpTo = r_post_chromatic_dispersion_offset_lowhealth.GetFloat();
		fNoiseLerpTo = r_post_noise_amount_fog.GetFloat();
		fBlurWidthLerpTo = 0.0f;
		fDesat = 0.0f;
	}
	else if( pPlayer->HasLowStamina() )
	{
		fVignettingLerpTo = r_post_vignetting_darkness_stamina.GetFloat();
		fDispersionLerpTo = r_post_chromatic_dispersion_offset.GetFloat();
		fBlurWidthLerpTo = 0.0f;
		fDesat = 0.0f;
		fNoiseLerpTo = r_post_noise_amount.GetFloat();
	}
	else
	{
		fVignettingLerpTo = r_post_vignetting_darkness.GetFloat();
		if( pPlayer->GetActiveWeapon() && pPlayer->GetActiveWeapon()->IsIronsighted() )
		{
			if( r_post_ironsight_useconvars.GetBool() )
				fDispersionLerpTo = r_post_chromatic_dispersion_offset_ironsighted.GetFloat();
			else
				fDispersionLerpTo = pPlayer->GetActiveWeapon()->GetChromaticAmount();
		}
		else
		{
			fDispersionLerpTo = r_post_chromatic_dispersion_offset.GetFloat();
		}

		fNoiseLerpTo = r_post_noise_amount.GetFloat();
		fScanLerpTo = r_post_scanline_amount.GetFloat();
		fBlurWidthLerpTo = 0.0f;
		fDesat = 0.0f;
	}

	/*if( iLastHealth > pPlayer->GetHealth() && pPlayer->GetHealth() > 15 )
	{
		//Player took damage.
		if( iLastHealth - pPlayer->GetHealth() >= 20 )
		{
			fDispersionAmount = r_post_chromatic_dispersion_offset_heavydamage.GetFloat();
		}
		else
		{
			fDispersionAmount = r_post_chromatic_dispersion_offset_damage.GetFloat();
		}
		bDisableDispersionOneFrame = true;
	}

	iLastHealth = pPlayer->GetHealth();*/

	if( fDispersionAmount != fDispersionLerpTo /*&& !bDisableDispersionOneFrame*/ )
		fDispersionAmount = FLerp( fDispersionAmount, fDispersionLerpTo, 0.1f );
	/*else if( bDisableDispersionOneFrame )
		bDisableDispersionOneFrame = false;*/

	if( fVignettingAmount != fVignettingLerpTo )
		fVignettingAmount = FLerp( fVignettingAmount, fVignettingLerpTo, 0.03f );

	if( fNoiseAmount != fNoiseLerpTo )
	{
		if( !pPlayer->InSmokeVolume() )
			fNoiseAmount = FLerp( fNoiseAmount, fNoiseLerpTo, r_post_stamina_lerpspeed.GetFloat() );
		else
			fNoiseAmount = FLerp( fNoiseAmount, fNoiseLerpTo, 0.1f );
	}

	if( fScanAmount != fScanLerpTo )
		fScanAmount = FLerp( fScanAmount, fScanLerpTo, 0.1f );

	if( fStaminaBlurAmount != fStaminaBlurAmountLerpTo )
	{
		if( !pPlayer->IsAlive() )
			fStaminaBlurAmount = FLerp( fStaminaBlurAmount, fStaminaBlurAmountLerpTo, 0.01f );
		else
			fStaminaBlurAmount = FLerp( fStaminaBlurAmount, fStaminaBlurAmountLerpTo, r_post_stamina_lerpspeed.GetFloat() );
	}

	if( fSightDOF != fSightDOFLerpTo )
		fSightDOF = FLerp( fSightDOF, fSightDOFLerpTo, 0.1f );

	if( fBlurWidth != fBlurWidthLerpTo )
		fBlurWidth = FLerp( fBlurWidth, fBlurWidthLerpTo, 0.03f );

	if( fDesat != fDesatLerpTo )
		fDesat = FLerp( fDesat, fDesatLerpTo, 0.03f );

	if( fPain != fPainLerpTo )
		fPain = FLerp( fPain, fPainLerpTo, 0.03f );

	IMaterialVar *var;

	if( fSightDOF >= 0.01f && r_post_blurfx.GetBool() )
	{
		var = m_RadialBlurX->FindVar( "$BLURSIZE", NULL );
		var->SetFloatValue( fSightDOF );
		var = m_RadialBlurY->FindVar( "$BLURSIZE", NULL );
		var->SetFloatValue( fSightDOF );
		DrawScreenEffectMaterial( m_RadialBlurX, x, y, w, h );
		DrawScreenEffectMaterial( m_RadialBlurY, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg("Current DOF: %.2f\n", fSightDOF);
	}

	if( !pPlayer->IsAlive() || (pPlayer->HasLowStamina() && !(pPlayer->GetWaterLevel() >= 3) && r_post_blurfx.GetBool()) )
	{
		var = m_BlurX->FindVar( "$BLURSIZE", NULL );
		var->SetFloatValue( fStaminaBlurAmount );
		var = m_BlurY->FindVar( "$BLURSIZE", NULL );
		var->SetFloatValue( fStaminaBlurAmount );
		DrawScreenEffectMaterial( m_BlurX, x, y, w, h );
		DrawScreenEffectMaterial( m_BlurY, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg("Current Screen Blur: %.2f\n", fStaminaBlurAmount);
	}

	if( fDispersionAmount >= 0.01f )
	{
		var = m_ChromaticDisp->FindVar( "$FOCUSOFFSET", NULL );
		var->SetFloatValue( fDispersionAmount );
		var = m_ChromaticDisp->FindVar( "$radial", NULL );
		if( pPlayer->InSmokeVolume() )
			var->SetIntValue( 0 );
		else
			var->SetIntValue( 1 );
		DrawScreenEffectMaterial( m_ChromaticDisp, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg( "Dispersion Amount: %.2f\n", fDispersionAmount );
	}

	if( fScanAmount >= 0.01f || fNoiseAmount >= 0.01f )
	{	
		var = m_Noise->FindVar( "$NOISE_INTENSITY", NULL );
		var->SetFloatValue( fNoiseAmount );
		var = m_Noise->FindVar( "$SCAN_INTENSITY", NULL );
		var->SetFloatValue( fScanAmount );
		var = m_Noise->FindVar( "$SCAN_AMOUNT", NULL );
		var->SetFloatValue( r_post_scanline_count.GetFloat() );
		var = m_Noise->FindVar( "$NOISE_SATURATION", NULL );
		var->SetFloatValue( r_post_noise_saturation.GetFloat() );
		var = m_Noise->FindVar( "$NOISE_BRIGHTNESS", NULL );
		var->SetFloatValue( r_post_noise_brightness.GetFloat() );
		var = m_Noise->FindVar( "$SHOW_GRAIN_CONTROL", NULL );
		var->SetIntValue( r_post_noise_debug.GetInt() );
		DrawScreenEffectMaterial( m_Noise, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
		{
			DevMsg( "Noise Amount: %.2f\n", fNoiseAmount );
			DevMsg( "Scanline Amount: %.2f\n", fScanAmount );
		}
	}

	if( fVignettingAmount >= 0.01f && !g_bRenderingScreenshot )
	{
		var = m_VignetMat->FindVar( "$VIGNETDARKNESS", NULL );
		var->SetFloatValue( fVignettingAmount );
		DrawScreenEffectMaterial( m_VignetMat, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg( "Vignetting Amount: %.2f\n", fVignettingAmount );
	}

	if( g_pMaterialSystemHardwareConfig->SupportsShaderModel_3_0() && !(fBlurWidth <= 0.01f && fBlurWidth >= -0.01f) )
	{
		Rect_t actualRect;
		UpdateScreenEffectTexture( 0, x, y, w, h, false, &actualRect );
		ITexture *pTexture = GetFullFrameFrameBufferTexture( 0 );

		CMatRenderContextPtr pRenderContext( materials );
		
		var = m_RadialBlurMat->FindVar("$BLURSTART", NULL);
		var->SetFloatValue( 1.0f );
		var = m_RadialBlurMat->FindVar("$BLURWIDTH", NULL);
		var->SetFloatValue( fBlurWidth );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg( "Radial Blur Width: %.2f\n", fBlurWidth );

		var = m_RadialBlurMat->FindVar("$gCX", NULL);
		var->SetFloatValue( 0.5f );
		var = m_RadialBlurMat->FindVar("$gCY", NULL);
		var->SetFloatValue( 0.5f );
		var = m_RadialBlurMat->FindVar("$TEXELOFFSETS", NULL);
		var->SetVecValue( actualRect.x, actualRect.y );

		pRenderContext->DrawScreenSpaceRectangle( m_RadialBlurMat, x, y, w, h,
			actualRect.x, actualRect.y, actualRect.x+actualRect.width-1, actualRect.y+actualRect.height-1, 
			pTexture->GetActualWidth(), pTexture->GetActualHeight() );
	}

	if( fDesat >= 0.01f )
	{
		var = m_DesatMat->FindVar( "$STRENGTH", NULL );
		var->SetFloatValue( fDesat );
		DrawScreenEffectMaterial( m_DesatMat, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg( "Desaturation Strength: %.2f\n", fDesat );
	}

	if( fPain >= 0.01f )
	{
		var = m_Pain->FindVar( "$MUTABLE_01", NULL );
		var->SetFloatValue( fPain );
		DrawScreenEffectMaterial( m_Pain, x, y, w, h );
		if( r_post_healtheffects_debug.GetBool() )
			DevMsg( "Pain Strength: %.2f\n", fPain );
	}
}

static float rBlack[4] = {0,0,0,1};
static float rWhite[4] = {1,1,1,1};
 
void CEntGlowEffect::Init( void ) 
{
	PrecacheMaterial( "effects/shaders/l4dglow_composite" );
	PrecacheMaterial( "effects/shaders/l4dglow_white" );
	PrecacheMaterial( "effects/shaders/blurx" );
	PrecacheMaterial( "effects/shaders/blury" );

	m_EffectMaterial.Init( materials->FindMaterial("effects/shaders/l4dglow_composite", TEXTURE_GROUP_CLIENT_EFFECTS, true) );
	m_WhiteMaterial.Init( materials->FindMaterial("effects/shaders/l4dglow_white", TEXTURE_GROUP_CLIENT_EFFECTS, true) );

	// Initialize render targets for our blurring
	m_GlowBuff1.InitRenderTarget( 128, 128, RT_SIZE_HDR, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_SEPARATE, false, "_rt_l4dglowbuff1" );
	m_GlowBuff2.InitRenderTarget( 128, 128, RT_SIZE_HDR, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_SEPARATE, false, "_rt_l4dglowbuff2" );
 
	// Load the blur textures
	m_BlurX.Init( materials->FindMaterial("effects/shaders/blurx", TEXTURE_GROUP_CLIENT_EFFECTS, true) );
	m_BlurY.Init( materials->FindMaterial("effects/shaders/blury", TEXTURE_GROUP_CLIENT_EFFECTS, true) );
}
 
void CEntGlowEffect::Shutdown( void )
{
	// Tell the materials we are done referencing them
	m_WhiteMaterial.Shutdown();
	m_EffectMaterial.Shutdown();
 
	m_GlowBuff1.Shutdown();
	m_GlowBuff2.Shutdown();
 
	m_BlurX.Shutdown();
	m_BlurY.Shutdown();
}
 
//-----------------------------------------------------------------------------
// Purpose: Render the effect
//-----------------------------------------------------------------------------
ConVar r_post_glow( "r_post_glow", "1", FCVAR_CHEAT );
ConVar cl_ge_glowscale( "r_post_glow_scale", "0.5", FCVAR_CHEAT );
ConVar cl_ge_glowstencil( "r_post_glow_stencil", "1", FCVAR_CHEAT );
 
void CEntGlowEffect::Render( int x, int y, int w, int h )
{
	VPROF( "CEntGlowEffect::Render" );
 
	// Don't bother rendering if we have nothing to render!
	if ( !r_post_glow.GetBool() || !m_vGlowEnts.Count() || g_bRenderingScreenshot || ( IsEnabled() == false ) )
		return;

	// Grab the render context
	CMatRenderContextPtr pRenderContext( materials );
 
	// Apply our glow buffers as the base textures for our blurring operators
	IMaterialVar *var;
	// Set our effect material to have a base texture of our primary glow buffer
	var = m_BlurX->FindVar( "$FBTEXTURE", NULL );
	var->SetTextureValue( m_GlowBuff1 );
	var = m_BlurX->FindVar( "$RESDIVISOR", NULL );
	var->SetIntValue( 4 );
	var = m_BlurX->FindVar( "$BLURSIZE", NULL );
	var->SetFloatValue( cl_ge_glowscale.GetFloat() );
	var = m_BlurY->FindVar( "$FBTEXTURE", NULL );
	var->SetTextureValue( m_GlowBuff2 );
	var = m_BlurY->FindVar( "$RESDIVISOR", NULL );
	var->SetIntValue( 4 );
	var = m_BlurY->FindVar( "$BLURSIZE", NULL );
	var->SetFloatValue( cl_ge_glowscale.GetFloat() );
 
	// Clear the glow buffer from the previous iteration
	pRenderContext->ClearColor4ub( 0, 0, 0, 255 );
	pRenderContext->PushRenderTargetAndViewport( m_GlowBuff1 );
	pRenderContext->ClearBuffers( true, true );
	pRenderContext->PopRenderTargetAndViewport();
 
	pRenderContext->PushRenderTargetAndViewport( m_GlowBuff2 );
	pRenderContext->ClearBuffers( true, true );
	pRenderContext->PopRenderTargetAndViewport();
 
	// Clear the stencil buffer in case someone dirtied it this frame
	pRenderContext->ClearStencilBufferRectangle( 0, 0, ScreenWidth(), ScreenHeight(), 0 );
 
	// Iterate over our registered entities and add them to the cut-out stencil and the glow buffer
	for ( int i=0; i < m_vGlowEnts.Count(); i++ )
	{
		if ( cl_ge_glowstencil.GetInt() )
			RenderToStencil( i, pRenderContext );
		RenderToGlowTexture( i, pRenderContext );
	}
 
	// Now we take the built up glow buffer (m_GlowBuff1) and blur it two ways
	// the intermediate buffer (m_GlowBuff2) allows us to do this properly
	pRenderContext->PushRenderTargetAndViewport( m_GlowBuff2 );
		DrawScreenEffectQuad( m_BlurX, m_GlowBuff2->GetActualWidth(), m_GlowBuff2->GetActualHeight() );
	pRenderContext->PopRenderTargetAndViewport();
 
	pRenderContext->PushRenderTargetAndViewport( m_GlowBuff1 );
		DrawScreenEffectQuad( m_BlurY, m_GlowBuff1->GetActualWidth(), m_GlowBuff1->GetActualHeight() );
	pRenderContext->PopRenderTargetAndViewport();
 
	if ( cl_ge_glowstencil.GetBool() )
	{
		// Setup the renderer to only draw where the stencil is not 1
		pRenderContext->SetStencilEnable( true );
		pRenderContext->SetStencilReferenceValue( 0 );
		pRenderContext->SetStencilTestMask( 1 );
		pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_EQUAL );
		pRenderContext->SetStencilPassOperation( STENCILOPERATION_ZERO );
	}
 
	// Finally draw our blurred result onto the screen
	DrawScreenEffectMaterial( m_EffectMaterial, x, y, w, h );
 
	pRenderContext->SetStencilEnable( false );
}
 
void CEntGlowEffect::RenderToStencil( int idx, IMatRenderContext *pRenderContext )
{
	if ( idx < 0 || idx >= m_vGlowEnts.Count() )
		return;
 
	C_BaseEntity *pEnt = m_vGlowEnts[idx]->m_hEnt.Get();
	if ( !pEnt )
	{
		// We don't exist anymore, remove us!
		delete m_vGlowEnts[idx];
		m_vGlowEnts.Remove(idx);
		return;
	}
 
	pRenderContext->SetStencilEnable( true );
	pRenderContext->SetStencilFailOperation( STENCILOPERATION_KEEP );
	pRenderContext->SetStencilZFailOperation( STENCILOPERATION_KEEP );
	pRenderContext->SetStencilPassOperation( STENCILOPERATION_REPLACE );
	pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_ALWAYS );
	pRenderContext->SetStencilWriteMask( 1 );
	pRenderContext->SetStencilReferenceValue( 1 );
 
	pRenderContext->DepthRange( 0.0f, 0.01f );
	render->SetBlend( 0 );
 
	modelrender->ForcedMaterialOverride( m_WhiteMaterial );
		pEnt->DrawModel( STUDIO_RENDER );
	modelrender->ForcedMaterialOverride( NULL );
 
	render->SetBlend( 1 );
	pRenderContext->DepthRange( 0.0f, 1.0f );
 
	pRenderContext->SetStencilEnable( false );
}
 
void CEntGlowEffect::RenderToGlowTexture( int idx, IMatRenderContext *pRenderContext )
{
	if ( idx < 0 || idx >= m_vGlowEnts.Count() )
		return;
 
	C_BaseEntity *pEnt = m_vGlowEnts[idx]->m_hEnt.Get();
	if ( !pEnt )
	{
		// We don't exist anymore, remove us!
		delete m_vGlowEnts[idx];
		m_vGlowEnts.Remove(idx);
		return;
	}
 
	pRenderContext->PushRenderTargetAndViewport( m_GlowBuff1 );
 
	modelrender->SuppressEngineLighting( true );
	render->SetColorModulation( m_vGlowEnts[idx]->m_fColor );
 
	modelrender->ForcedMaterialOverride( m_WhiteMaterial );
		pEnt->DrawModel( STUDIO_RENDER );
	modelrender->ForcedMaterialOverride( NULL );
 
	render->SetColorModulation( rWhite );
	modelrender->SuppressEngineLighting( false );
 
	pRenderContext->PopRenderTargetAndViewport();
}
 
void CEntGlowEffect::RegisterEnt( EHANDLE hEnt, Color glowColor /*=Color(255,255,255,64)*/, float fGlowScale /*=1.0f*/ )
{
	// Don't add duplicates
	if ( FindGlowEnt(hEnt) != -1 || !hEnt.Get() )
		return;

	sGlowEnt *newEnt = new sGlowEnt;
	newEnt->m_hEnt = hEnt;
	newEnt->m_fColor[0] = glowColor.r() / 255.0f;
	newEnt->m_fColor[1] = glowColor.g() / 255.0f;
	newEnt->m_fColor[2] = glowColor.b() / 255.0f;
	newEnt->m_fColor[3] = glowColor.a() / 255.0f;
	newEnt->m_fGlowScale = fGlowScale;
	m_vGlowEnts.AddToTail( newEnt );
}
 
void CEntGlowEffect::DeregisterEnt( EHANDLE hEnt )
{
	int idx = FindGlowEnt(hEnt);
	if ( idx == -1 )
		return;

	delete m_vGlowEnts[idx];
	m_vGlowEnts.Remove( idx );
}
 
void CEntGlowEffect::SetEntColor( EHANDLE hEnt, Color glowColor )
{
	int idx = FindGlowEnt(hEnt);
	if ( idx == -1 )
		return;
 
	m_vGlowEnts[idx]->m_fColor[0] = glowColor.r() / 255.0f;
	m_vGlowEnts[idx]->m_fColor[1] = glowColor.g() / 255.0f;
	m_vGlowEnts[idx]->m_fColor[2] = glowColor.b() / 255.0f;
	m_vGlowEnts[idx]->m_fColor[3] = glowColor.a() / 255.0f;
}
 
void CEntGlowEffect::SetEntGlowScale( EHANDLE hEnt, float fGlowScale )
{
	int idx = FindGlowEnt(hEnt);
	if ( idx == -1 )
		return;
 
	m_vGlowEnts[idx]->m_fGlowScale = fGlowScale;
}
 
int CEntGlowEffect::FindGlowEnt( EHANDLE hEnt )
{
	for ( int i=0; i < m_vGlowEnts.Count(); i++ )
	{
		if ( m_vGlowEnts[i]->m_hEnt == hEnt )
			return i;
	}
 
	return -1;
}

ConVar r_post_negative("r_post_negative", "0", FCVAR_CHEAT );

ConVar r_post_bleach_bypass("r_post_bleach_bypass", "0", FCVAR_CHEAT );
ConVar r_post_bleach_bypass_opacity("r_post_bleach_bypass_opacity", "1.0", FCVAR_CHEAT );

ConVar r_post_color_clipping("r_post_color_clipping", "0", FCVAR_CHEAT );
ConVar r_post_color_clipping_mincolor_r("r_post_color_clipping_mincolor_r", "0", FCVAR_CHEAT );
ConVar r_post_color_clipping_mincolor_g("r_post_color_clipping_mincolor_g", "0", FCVAR_CHEAT );
ConVar r_post_color_clipping_mincolor_b("r_post_color_clipping_mincolor_b", "0", FCVAR_CHEAT );
ConVar r_post_color_clipping_mincolor_a("r_post_color_clipping_mincolor_a", "1", FCVAR_CHEAT );
ConVar r_post_color_clipping_maxcolor_r("r_post_color_clipping_maxcolor_r", "1", FCVAR_CHEAT );
ConVar r_post_color_clipping_maxcolor_g("r_post_color_clipping_maxcolor_g", "1", FCVAR_CHEAT );
ConVar r_post_color_clipping_maxcolor_b("r_post_color_clipping_maxcolor_b", "1", FCVAR_CHEAT );
ConVar r_post_color_clipping_maxcolor_a("r_post_color_clipping_maxcolor_a", "1", FCVAR_CHEAT );
ConVar r_post_color_clipping_squish("r_post_color_clipping_squish", "1", FCVAR_CHEAT );

ConVar r_post_cross_processing("r_post_cross_processing", "0", FCVAR_CHEAT );
ConVar r_post_cross_processing_saturation("r_post_cross_processing_saturation", "0.8", FCVAR_CHEAT );
ConVar r_post_cross_processing_contrast("r_post_cross_processing_contrast", "1.0", FCVAR_CHEAT );
ConVar r_post_cross_processing_brightness("r_post_cross_processing_brightness", "0.0", FCVAR_CHEAT );
ConVar r_post_cross_processing_intensity("r_post_cross_processing_intensity", "0.2", FCVAR_CHEAT );

ConVar r_post_complements_guidehue_r("r_post_complements_guidehue_r", "1", FCVAR_CHEAT );
ConVar r_post_complements_guidehue_g("r_post_complements_guidehue_g", "1", FCVAR_CHEAT );
ConVar r_post_complements_guidehue_b("r_post_complements_guidehue_b", "1", FCVAR_CHEAT );
ConVar r_post_complements_amount("r_post_complements_amount", "1.0", FCVAR_CHEAT );
ConVar r_post_complements_concentrate("r_post_complements_concentrate", "1.0", FCVAR_CHEAT );
ConVar r_post_complements_desatcorr("r_post_complements_desatcorr", "1.0", FCVAR_CHEAT );

ConVar r_post_cubic_distortion("r_post_cubic_distortion", "0", FCVAR_CHEAT );
ConVar r_post_cubic_distortion_amount("r_post_cubic_distortion_amount", "-0.15", FCVAR_CHEAT);
ConVar r_post_cubic_distortion_cubicamount("r_post_cubic_distortion_cubicamount", "0.5", FCVAR_CHEAT);

ConVar r_post_desaturate("r_post_desaturate", "0", FCVAR_CHEAT );
ConVar r_post_desaturate_strength("r_post_desaturate_strength", "1.0", FCVAR_CHEAT );

ConVar r_post_nightvision("r_post_nightvision", "0", FCVAR_CHEAT );

void nextgen_callback( IConVar *pConVar, char const *pOldString, float flOldValue );

static ConVar r_post_nextgen( "r_post_nextgen", "0", FCVAR_CHEAT, "THE MOST IMPORTANT ASPECT OF CITY17! IT'LL BLOW. YOUR. MIND.", nextgen_callback );

extern ConVar r_post_complements;

void nextgen_callback( IConVar *pConVar, char const *pOldString, float flOldValue )
{
	if ( r_post_nextgen.GetBool() )
	{
		Msg( "Shit just got unreal!\n" );
	}
}

void CColorCorrectionEffect::Init( void )
{
	m_Negative.Init( materials->FindMaterial("effects/shaders/negative", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_BleachBypass.Init( materials->FindMaterial("effects/shaders/bleach_bypass", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_ColorClipping.Init( materials->FindMaterial("effects/shaders/color_clipping", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_CrossProcessing.Init( materials->FindMaterial("effects/shaders/cross_processing", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_NextGen.Init( materials->FindMaterial("effects/shaders/nextgen", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_Complements.Init( materials->FindMaterial("effects/shaders/complements", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_CubicDistortion.Init( materials->FindMaterial("effects/shaders/cubic_distortion", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_Desaturate.Init( materials->FindMaterial("effects/shaders/desaturate", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_Nightvision.Init( materials->FindMaterial("effects/shaders/nightvision", TEXTURE_GROUP_PIXEL_SHADERS, true) );
}

void CColorCorrectionEffect::Shutdown( void )
{
	m_Negative.Shutdown();
	m_BleachBypass.Shutdown();
	m_ColorClipping.Shutdown();
	m_CrossProcessing.Shutdown();
	m_NextGen.Shutdown();
	m_Complements.Shutdown();
	m_CubicDistortion.Shutdown();
	m_Desaturate.Shutdown();
	m_Nightvision.Shutdown();
}

ConVar r_post_colorcorrection( "r_post_colorcorrection", "1", FCVAR_ARCHIVE );
void CColorCorrectionEffect::Render( int x, int y, int w, int h )
{
	VPROF( "CColorCorrectionEffect::Render" );

	if( !r_post_colorcorrection.GetBool() || !IsEnabled() )
		return;

	IMaterialVar *var;

	// Grab the render context
	CMatRenderContextPtr pRenderContext( materials );
	
	if( r_post_negative.GetBool() )
	{
		DrawScreenEffectMaterial( m_Negative, x, y, w, h );
	}

	if( r_post_bleach_bypass.GetBool() )
	{
		var = m_BleachBypass->FindVar( "$OPACITY", NULL );
		var->SetFloatValue( r_post_bleach_bypass_opacity.GetFloat() );
		DrawScreenEffectMaterial( m_BleachBypass, x, y, w, h );
	}

	if( r_post_color_clipping.GetBool() )
	{	
		var = m_ColorClipping->FindVar( "$mincolor", NULL );
		var->SetVecValue( r_post_color_clipping_mincolor_r.GetFloat(), r_post_color_clipping_mincolor_g.GetFloat(), r_post_color_clipping_mincolor_b.GetFloat(), r_post_color_clipping_mincolor_a.GetFloat() );
		var = m_ColorClipping->FindVar( "$maxcolor", NULL );
		var->SetVecValue( r_post_color_clipping_maxcolor_r.GetFloat(), r_post_color_clipping_maxcolor_g.GetFloat(), r_post_color_clipping_maxcolor_b.GetFloat(), r_post_color_clipping_maxcolor_a.GetFloat() );
		var = m_ColorClipping->FindVar( "$squish", NULL );
		var->SetIntValue( r_post_color_clipping_squish.GetInt() );
		DrawScreenEffectMaterial( m_ColorClipping, x, y, w, h );
	}

	if( r_post_cross_processing.GetBool() )
	{
		var = m_CrossProcessing->FindVar( "$MUTABLE_01", NULL );
		var->SetFloatValue( r_post_cross_processing_saturation.GetFloat() );
		var = m_CrossProcessing->FindVar( "$MUTABLE_02", NULL );
		var->SetFloatValue( r_post_cross_processing_contrast.GetFloat() );
		var = m_CrossProcessing->FindVar( "$MUTABLE_03", NULL );
		var->SetFloatValue( r_post_cross_processing_brightness.GetFloat() );
		var = m_CrossProcessing->FindVar( "$MUTABLE_04", NULL );
		var->SetFloatValue( r_post_cross_processing_intensity.GetFloat() );
		DrawScreenEffectMaterial( m_CrossProcessing, x, y, w, h );
	}

	if( r_post_nextgen.GetBool() )
	{	
		DrawScreenEffectMaterial( m_NextGen, x, y, w, h );
	}

	if( r_post_complements.GetBool() )
	{
		var = m_Complements->FindVar( "$guidehue", NULL );
		var->SetVecValue( r_post_complements_guidehue_r.GetFloat(), r_post_complements_guidehue_g.GetFloat(), r_post_complements_guidehue_b.GetFloat() );
		var = m_Complements->FindVar( "$amount", NULL );
		var->SetFloatValue( r_post_complements_amount.GetFloat() );
		var = m_Complements->FindVar( "$concentrate", NULL );
		var->SetFloatValue( r_post_complements_concentrate.GetFloat() );
		var = m_Complements->FindVar( "$desatcorr", NULL );
		var->SetFloatValue( r_post_complements_desatcorr.GetFloat() );
		DrawScreenEffectMaterial( m_Complements, x, y, w, h );
	}

	if( r_post_cubic_distortion.GetBool() )
	{
		var = m_CubicDistortion->FindVar( "$distortion", NULL );
		var->SetFloatValue( r_post_cubic_distortion_amount.GetFloat() );
		var = m_CubicDistortion->FindVar( "$cubicdistortion", NULL );
		var->SetFloatValue( r_post_cubic_distortion_cubicamount.GetFloat() );
		DrawScreenEffectMaterial( m_CubicDistortion, x, y, w, h );
	}

	if( r_post_desaturate.GetBool() )
	{
		var = m_Desaturate->FindVar( "$strength", NULL );
		var->SetFloatValue( r_post_desaturate_strength.GetFloat() );
		DrawScreenEffectMaterial( m_Desaturate, x, y, w, h );
	}

	if( r_post_nightvision.GetBool() )
	{
		DrawScreenEffectMaterial( m_Nightvision, x, y, w, h );
	}
}

void CFXAA::Init( void )
{
	PrecacheMaterial( "effects/shaders/fxaa_luma" );
	PrecacheMaterial( "effects/shaders/fxaa" );

	m_Luma.Init( materials->FindMaterial("effects/shaders/fxaa_luma", TEXTURE_GROUP_PIXEL_SHADERS, true) );
	m_FXAA.Init( materials->FindMaterial("effects/shaders/fxaa", TEXTURE_GROUP_PIXEL_SHADERS, true) );
}

void CFXAA::Shutdown( void )
{
	m_Luma.Shutdown();
	m_FXAA.Shutdown();
}

extern ConVar r_post_fxaa;
ConVar r_post_fxaa_quality( "r_post_fxaa_quality", "4", FCVAR_ARCHIVE, "0 = Very Low, 1 = Low, 2 = Medium, 3 = High, 4 = Very High", true, 0, true, 4 );
void CFXAA::Render( int x, int y, int w, int h )
{
	VPROF( "CFXAA::Render" );

	if( !r_post_fxaa.GetBool() || ( IsEnabled() == false ) )
		return;

	IMaterialVar *var;
	var = m_FXAA->FindVar( "$QUALITY", NULL );
	var->SetIntValue( r_post_fxaa_quality.GetInt() );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->OverrideDepthEnable( true, false );
	DrawScreenEffectMaterial( m_Luma, x, y, w, h );
	DrawScreenEffectMaterial( m_FXAA, x, y, w, h );
	pRenderContext->OverrideDepthEnable( false, true );
}