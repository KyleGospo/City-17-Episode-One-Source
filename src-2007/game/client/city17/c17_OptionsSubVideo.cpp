//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's options dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Video Options submenu.
//
//==========================================================================================//

#include "cbase.h"
#include "c17_OptionsSubVideo.h"
#include "nf_CvarComboBox.h"
#include "cdll_client_int.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <stdio.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

extern ConVar r_radiosity_desired;
extern ConVar r_queued_decals_desired;
extern ConVar r_threaded_client_shadow_manager;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubVideo::CC17OptionsSubVideo( vgui::Panel *parent, const char *name ) : PropertyPage(parent, NULL)
{
	bool bSM30 = g_pMaterialSystemHardwareConfig->SupportsShaderModel_3_0();

	m_pRadBox = new CCvarComboBox(this, "RadBox");
	if ( m_pRadBox )
	{
		m_pRadBox->AddItem( "#GameUI_Disabled", "r_radiosity_desired_city17 4" );
		m_pRadBox->AddItem( "#GameUI_C17_r_radiosity_1", "r_radiosity_desired_city17 1" );
		m_pRadBox->AddItem( "#GameUI_C17_r_radiosity_3", "r_radiosity_desired_city17 3" );
		m_pRadBox->AddItem( "#GameUI_C17_r_radiosity_2", "r_radiosity_desired_city17 2" );
	}

	/*m_pAABox = new CCvarComboBox(this, "AABox");
	if ( m_pAABox )
	{
		m_pAABox->AddItem( "#GameUI_Disabled", "mat_software_aa_quality 0;mat_software_aa_strength 0;mat_software_aa_strength_vgui 0" );
		m_pAABox->AddItem( "#GameUI_Enabled", "mat_software_aa_quality 1;mat_software_aa_strength 1;mat_software_aa_strength_vgui 1" );
	}*/

	m_pMCBox = new CCvarComboBox(this, "MCBox");
	if ( m_pMCBox )
	{
		m_pMCBox->AddItem( "#GameUI_Disabled", "c17_multithreading 0;r_threaded_client_shadow_manager 0;r_queued_decals_desired_city17 0;r_queued_post_processing 0;r_queued_ropes 0" );
		m_pMCBox->AddItem( "#GameUI_C17ThreadMedium", "c17_multithreading 0;r_threaded_client_shadow_manager 1;r_queued_decals_desired_city17 1;r_queued_post_processing 1;r_queued_ropes 1" );
		m_pMCBox->AddItem( "#GameUI_Enabled", "c17_multithreading 1;r_threaded_client_shadow_manager 1;r_queued_decals_desired_city17 1;r_queued_post_processing 1;r_queued_ropes 1" );
	}

	m_pShadowFixBox = new CCvarComboBox(this, "ShadowFixBox");
	if ( m_pShadowFixBox )
	{
		m_pShadowFixBox->AddItem( "#GameUI_Disabled", "r_dynamicshadows_use_c17_improvements 0" );
		m_pShadowFixBox->AddItem( "#GameUI_Enabled", "r_dynamicshadows_use_c17_improvements 1" );
	}

	m_pMuzzleFlashProj = new CCvarComboBox(this, "FlashBox");
	if ( m_pMuzzleFlashProj )
	{
		m_pMuzzleFlashProj->AddItem( "#GameUI_Disabled", "c17_enable_muzzle_flash_light 0;muzzleflash_light 1" );
		m_pMuzzleFlashProj->AddItem( "#GameUI_Enabled", "c17_enable_muzzle_flash_light 1;muzzleflash_light 0" );
	}

	m_pProjectedShadows = new CCvarComboBox(this, "ShadowBox");
	if ( m_pProjectedShadows )
	{
		m_pProjectedShadows->AddItem( "#GameUI_C17ProjOff", "r_dynamic_shadow_mode 0" );
		m_pProjectedShadows->AddItem( "#GameUI_C17ProjFlash", "r_dynamic_shadow_mode 1" );
		m_pProjectedShadows->AddItem( "#GameUI_C17ProjAll", "r_dynamic_shadow_mode 2" );
	}

	m_pSunBox = new CCvarComboBox(this, "SunBox");
	if ( m_pSunBox )
	{
		m_pSunBox->AddItem( "#GameUI_Disabled", "r_post_sunshaft 0;r_post_sunshaft_quality 0" );
		if( bSM30 )
		{
			m_pSunBox->AddItem( "#GameUI_Low", "r_post_sunshaft 1;r_post_sunshaft_quality 0;r_post_sunshaft_blur 0" );
			m_pSunBox->AddItem( "#GameUI_Medium", "r_post_sunshaft 1;r_post_sunshaft_quality 0;r_post_sunshaft_blur 1" );
			m_pSunBox->AddItem( "#GameUI_High", "r_post_sunshaft 1;r_post_sunshaft_quality 1;r_post_sunshaft_blur 1" );
			m_pSunBox->AddItem( "#GameUI_JesusHChrist", "r_post_sunshaft 1;r_post_sunshaft_quality 2;r_post_sunshaft_blur 1" );
		}
	}

	m_pPhongBox = new CCvarComboBox(this, "PhongBox");
	if ( m_pPhongBox )
	{
		m_pPhongBox->AddItem( "#GameUI_Disabled", "mat_enable_lightmapped_phong 0" );
		m_pPhongBox->AddItem( "#GameUI_Enabled", "mat_enable_lightmapped_phong 1" );
	}

	m_pImpactBox = new CCvarComboBox(this, "ImpactBox");
	if ( m_pImpactBox )
	{
		m_pImpactBox->AddItem( "#GameUI_Disabled", "cl_new_impact_effects 0" );
		m_pImpactBox->AddItem( "#GameUI_Low", "cl_new_impact_effects 1;cl_new_impact_effects_cheap 1" );
		m_pImpactBox->AddItem( "#GameUI_Medium", "cl_new_impact_effects 1;cl_new_impact_effects_cheap 0" );
	}

	m_pWaterBox = new CCvarComboBox(this, "WaterBox");
	if ( m_pWaterBox )
	{
		m_pWaterBox->AddItem( "#GameUI_Disabled", "cl_particle_water 0" );
		m_pWaterBox->AddItem( "#GameUI_Enabled", "cl_particle_water 1" );
	}

	m_pPPBox = new CCvarComboBox(this, "PPBox");
	if ( m_pPPBox )
	{
		m_pPPBox->AddItem( "#GameUI_Disabled", "r_post_unsharp 0;r_post_healtheffects 0;r_post_watereffects 0;r_post_blurfx 0;r_post_ssao 0;r_post_anamorphic_bloom 0" );
		m_pPPBox->AddItem( "#GameUI_Low", "r_post_unsharp 0;r_post_healtheffects 1;r_post_watereffects 0;r_post_blurfx 0;r_post_ssao 0;r_post_anamorphic_bloom 0" );
		m_pPPBox->AddItem( "#GameUI_Medium", "r_post_unsharp 0;r_post_healtheffects 1;r_post_watereffects 1;r_post_blurfx 1;r_post_ssao 0;r_post_anamorphic_bloom 0" );
		m_pPPBox->AddItem( "#GameUI_High", "r_post_unsharp 1;r_post_healtheffects 1;r_post_watereffects 1;r_post_blurfx 1;r_post_ssao 0;r_post_anamorphic_bloom 0" );
		if( bSM30 )
		{
			m_pPPBox->AddItem( "#GameUI_Ultra", "r_post_unsharp 1;r_post_healtheffects 1;r_post_watereffects 1;r_post_blurfx 1;r_post_ssao 0;r_post_anamorphic_bloom 1" );
			m_pPPBox->AddItem( "#GameUI_JesusHChrist", "r_post_unsharp 1;r_post_healtheffects 1;r_post_watereffects 1;r_post_blurfx 1;r_post_ssao 1;r_post_anamorphic_bloom 1" );
		}
	}

	m_pFXAABox = new CCvarComboBox(this, "FXAABox");
	if ( m_pFXAABox )
	{
		m_pFXAABox->AddItem( "#GameUI_Disabled", "r_post_fxaa 0" );
		if( bSM30 )
		{
			m_pFXAABox->AddItem( "#GameUI_VeryLow", "r_post_fxaa 1; r_post_fxaa_quality 0" );
			m_pFXAABox->AddItem( "#GameUI_Low", "r_post_fxaa 1; r_post_fxaa_quality 1" );
			m_pFXAABox->AddItem( "#GameUI_Medium", "r_post_fxaa 1; r_post_fxaa_quality 2" );
			m_pFXAABox->AddItem( "#GameUI_High", "r_post_fxaa 1; r_post_fxaa_quality 3" );
			m_pFXAABox->AddItem( "#GameUI_Ultra", "r_post_fxaa 1; r_post_fxaa_quality 4" );
		}
	}

	LoadControlSettings("Resource\\C17_Video_Options.res");	
}

void CC17OptionsSubVideo::SetComboBoxDefaults()
{
	//r_radiosity_desired
	if( r_radiosity_desired.GetInt() == 4 )
	{
		m_pRadBox->ActivateItem( 0 );
	}
	else if( r_radiosity_desired.GetInt() == 3 )
	{
		m_pRadBox->ActivateItem( 1 );
	}
	else if( r_radiosity_desired.GetInt() == 1 )
	{
		m_pRadBox->ActivateItem( 2 );
	}
	else if( r_radiosity_desired.GetInt() == 2 )
	{
		m_pRadBox->ActivateItem( 3 );
	}

	//Software AA
	/*ConVarRef aaquality( "mat_software_aa_quality" );
	ConVarRef aastrength( "mat_software_aa_strength" );
	ConVarRef aastrengthvgui( "mat_software_aa_strength_vgui" );
	if( (aaquality.GetInt() > 0) && (aastrength.GetInt() > 0) && (aastrengthvgui.GetInt() > 0) )
	{
		m_pAABox->ActivateItem( 1 );
	}
	else
	{
		m_pAABox->ActivateItem( 0 );
	}*/

	ConVarRef queuedpost( "r_queued_post_processing" );
	ConVarRef queuedropes( "r_queued_ropes" );
	ConVarRef threadedparticles( "r_threaded_particles" );
	ConVarRef hostthread( "c17_multithreading" );
	if( (hostthread.GetBool()) && (r_queued_decals_desired.GetBool()) && (queuedpost.GetBool()) && (threadedparticles.GetBool()) && (queuedropes.GetBool()) && (r_threaded_client_shadow_manager.GetBool()) )
	{
		m_pMCBox->ActivateItem( 2 );
	}
	else if( !(hostthread.GetBool()) && (r_queued_decals_desired.GetBool()) && (queuedpost.GetBool()) && (threadedparticles.GetBool()) && (queuedropes.GetBool()) && (r_threaded_client_shadow_manager.GetBool()) )
	{
		m_pMCBox->ActivateItem( 1 );
	}
	else
	{
		m_pMCBox->ActivateItem( 0 );
	}

	ConVarRef projfix( "r_dynamicshadows_use_c17_improvements" );
	if( projfix.GetBool() )
	{
		m_pShadowFixBox->ActivateItem( 1 );
	}
	else
	{
		m_pShadowFixBox->ActivateItem( 0 );
	}

	ConVarRef projectedmuzzle( "c17_enable_muzzle_flash_light" );
	if( projectedmuzzle.GetBool() )
	{
		m_pMuzzleFlashProj->ActivateItem( 1 );
	}
	else
	{
		m_pMuzzleFlashProj->ActivateItem( 0 );
	}

	ConVarRef allowshadows( "r_dynamic_shadow_mode" );
	if( allowshadows.GetInt() == 2 )
	{
		m_pProjectedShadows->ActivateItem( 2 );
	}
	else if( allowshadows.GetInt() == 1 )
	{
		m_pProjectedShadows->ActivateItem( 1 );
	}
	else
	{
		m_pProjectedShadows->ActivateItem( 0 );
	}

	ConVarRef impactfx( "cl_new_impact_effects" );
	ConVarRef impactfx_cheap( "cl_new_impact_effects_cheap" );
	if( impactfx.GetBool() && !impactfx_cheap.GetBool() )
	{
		m_pImpactBox->ActivateItem( 2 );
	}
	else if( impactfx.GetBool() && impactfx_cheap.GetBool() )
	{
		m_pImpactBox->ActivateItem( 1 );
	}
	else
	{
		m_pImpactBox->ActivateItem( 0 );
	}

	ConVarRef waterfx( "cl_particle_water" );
	m_pWaterBox->ActivateItem( waterfx.GetBool() );

	ConVarRef phongvar( "mat_enable_lightmapped_phong" );
	m_pPhongBox->ActivateItem( phongvar.GetBool() );

	//FIXME: Finish shader and enable normal functionality
	ConVarRef sunvar( "r_post_sunshaft" );
	ConVarRef sunquality( "r_post_sunshaft_quality" );
	ConVarRef sunblur( "r_post_sunshaft_blur" );
	if( sunvar.GetBool() && sunquality.GetInt() == 2 )
	{
		m_pSunBox->ActivateItem( 4 );
	}
	else if( sunvar.GetBool() && sunquality.GetInt() == 1 )
	{
		m_pSunBox->ActivateItem( 3 );
	}
	else if( sunvar.GetBool() && sunblur.GetBool() )
	{
		m_pSunBox->ActivateItem( 2 );
	}
	else if( sunvar.GetBool() )
	{
		m_pSunBox->ActivateItem( 1 );
	}
	else
	{
		m_pSunBox->ActivateItem( 0 );
	}

	ConVarRef unsharp( "r_post_unsharp" );
	ConVarRef healtheffects( "r_post_healtheffects" );
	ConVarRef watereffect( "r_post_watereffects" );
	ConVarRef blurfx( "r_post_blurfx" );
	ConVarRef ssao( "r_post_ssao" );
	ConVarRef lensflare( "r_post_anamorphic_bloom" );
	if( ssao.GetBool() && unsharp.GetBool() && healtheffects.GetBool() && watereffect.GetBool() && blurfx.GetBool() && lensflare.GetBool() )
	{
		m_pPPBox->ActivateItem( 5 );
	}
	else if( unsharp.GetBool() && healtheffects.GetBool() && watereffect.GetBool() && blurfx.GetBool() && lensflare.GetBool() )
	{
		m_pPPBox->ActivateItem( 4 );
	}
	else if( unsharp.GetBool() && healtheffects.GetBool() && watereffect.GetBool() && blurfx.GetBool() )
	{
		m_pPPBox->ActivateItem( 3 );
	}
	else if( healtheffects.GetBool() && watereffect.GetBool() && blurfx.GetBool() )
	{
		m_pPPBox->ActivateItem( 2 );
	}
	else if( healtheffects.GetBool() )
	{
		m_pPPBox->ActivateItem( 1 );
	}
	else
	{
		m_pPPBox->ActivateItem( 0 );
	}

	ConVarRef fxaa( "r_post_fxaa" );
	ConVarRef fxaa_quality( "r_post_fxaa_quality" );
	if( fxaa.GetBool() && fxaa_quality.GetInt() == 4 )
	{
		m_pFXAABox->ActivateItem( 5 );
	}
	else if( fxaa.GetBool() && fxaa_quality.GetInt() == 3 )
	{
		m_pFXAABox->ActivateItem( 4 );
	}
	else if( fxaa.GetBool() && fxaa_quality.GetInt() == 2 )
	{
		m_pFXAABox->ActivateItem( 3 );
	}
	else if( fxaa.GetBool() && fxaa_quality.GetInt() == 1 )
	{
		m_pFXAABox->ActivateItem( 2 );
	}
	else if( fxaa.GetBool() )
	{
		m_pFXAABox->ActivateItem( 1 );
	}
	else
	{
		m_pFXAABox->ActivateItem( 0 );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubVideo::~CC17OptionsSubVideo()
{
	delete m_pRadBox;
	//delete m_pAABox;
	delete m_pMCBox;
	delete m_pShadowFixBox;
	delete m_pMuzzleFlashProj;
	delete m_pProjectedShadows;
	delete m_pPhongBox;
	delete m_pSunBox;
	delete m_pImpactBox;
	delete m_pWaterBox;
	delete m_pPPBox;
	delete m_pFXAABox;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubVideo::OnResetData()
{
	m_pRadBox->Reset();
	//m_pAABox->Reset();
	m_pMCBox->Reset();
	m_pShadowFixBox->Reset();
	m_pMuzzleFlashProj->Reset();
	m_pProjectedShadows->Reset();
	m_pPhongBox->Reset();
	m_pSunBox->Reset();
	m_pImpactBox->Reset();
	m_pWaterBox->Reset();
	m_pPPBox->Reset();
	m_pFXAABox->Reset();
	SetComboBoxDefaults();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubVideo::OnApplyChanges()
{
	m_pRadBox->ApplyChanges();
	//m_pAABox->ApplyChanges();
	m_pMCBox->ApplyChanges();
	m_pShadowFixBox->ApplyChanges();
	m_pMuzzleFlashProj->ApplyChanges();
	m_pProjectedShadows->ApplyChanges();
	m_pPhongBox->ApplyChanges();
	m_pSunBox->ApplyChanges();
	m_pImpactBox->ApplyChanges();
	m_pWaterBox->ApplyChanges();
	m_pPPBox->ApplyChanges();
	m_pFXAABox->ApplyChanges();
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CC17OptionsSubVideo::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}