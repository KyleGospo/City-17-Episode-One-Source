#include "cbase.h"
#include "c17_optionspanel.h"

// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

extern ConVar r_radiosity_desired;
extern ConVar r_queued_decals_desired;
extern ConVar cl_viewbob_enabled;
extern ConVar r_threaded_client_shadow_manager;

PanelGlobals(CC17Options, CC17OptionsPanel, c17options);

CON_COMMAND( OpenC17OptionsDialog, "Opens the City 17 Options dialog" )
{
	if ( !c17options )
		return;

	c17options->GetPanel()->SetVisible( true );
}

CC17OptionsPanel::CC17OptionsPanel( vgui::VPANEL parent ) : BaseClass( NULL, "c17optionspanel" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );

	m_pRadBox = new CCvarComboBox(this, "RadBox");
	if ( m_pRadBox )
	{
		m_pRadBox->AddItem( "#GameUI_Disabled", "r_radiosity_desired_city17 4" );
		m_pRadBox->AddItem( "#GameUI_C17_r_radiosity_1", "r_radiosity_desired_city17 1" );
		m_pRadBox->AddItem( "#GameUI_C17_r_radiosity_3", "r_radiosity_desired_city17 3" );
		m_pRadBox->AddItem( "#GameUI_C17_r_radiosity_2", "r_radiosity_desired_city17 2" );
	}

	m_pHeadBox = new CCvarComboBox(this, "HeadBox");
	if ( m_pHeadBox )
	{
		m_pHeadBox->AddItem( "#GameUI_Disabled", "cl_headbob_enabled 0" );
		m_pHeadBox->AddItem( "#GameUI_Enabled", "cl_headbob_enabled 1" );
	}

	m_pAABox = new CCvarComboBox(this, "AABox");
	if ( m_pAABox )
	{
		m_pAABox->AddItem( "#GameUI_Disabled", "mat_software_aa_quality 0;mat_software_aa_strength 0;mat_software_aa_strength_vgui 0;mat_software_aa_edge_threshold 1" );
		m_pAABox->AddItem( "#GameUI_Enabled", "mat_software_aa_quality 1;mat_software_aa_strength 1;mat_software_aa_strength_vgui 1;mat_software_aa_edge_threshold 1.4" );
	}

	m_pMCBox = new CCvarComboBox(this, "MCBox");
	if ( m_pMCBox )
	{
		m_pMCBox->AddItem( "#GameUI_Disabled", "r_threaded_client_shadow_manager 0;r_threaded_renderables 0;r_threaded_particles 0;r_queued_decals_desired_city17 0;r_queued_post_processing 0;r_queued_ropes 0" );
		m_pMCBox->AddItem( "#GameUI_Enabled", "r_threaded_client_shadow_manager 1;r_threaded_renderables 1;r_threaded_particles 1;r_queued_decals_desired_city17 1;r_queued_post_processing 1;r_queued_ropes 1" );
	}

	m_pSndBox = new CCvarComboBox(this, "SndBox");
	if ( m_pSndBox )
	{
		m_pSndBox->AddItem( "#GameUI_Disabled", "dsp_enhance_stereo 0" );
		m_pSndBox->AddItem( "#GameUI_Enabled", "dsp_enhance_stereo 1" );
	}

	m_pMBBox = new CCvarComboBox(this, "MBBox");
	if ( m_pMBBox )
	{
		m_pMBBox->AddItem( "#GameUI_Disabled", "mat_motion_blur_forward_enabled 0" );
		m_pMBBox->AddItem( "#GameUI_Enabled", "mat_motion_blur_forward_enabled 1" );
	}

	m_pShadowFixBox = new CCvarComboBox(this, "ShadowFixBox");
	if ( m_pShadowFixBox )
	{
		m_pShadowFixBox->AddItem( "#GameUI_Disabled", "r_flashlight_use_c17_improvements 1;r_projected_use_c17_improvements 1" );
		m_pShadowFixBox->AddItem( "#GameUI_Enabled", "r_flashlight_use_c17_improvements 0;r_projected_use_c17_improvements 0" );
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
		m_pProjectedShadows->AddItem( "#GameUI_Disabled", "r_projected_allow_shadows 0" );
		m_pProjectedShadows->AddItem( "#GameUI_Enabled", "r_projected_allow_shadows 1" );
	}

	m_pSunBox = new CCvarComboBox(this, "SunBox");
	if ( m_pSunBox )
	{
		m_pSunBox->AddItem( "#GameUI_Disabled", "r_sunshafts 0" );
		m_pSunBox->AddItem( "#GameUI_Low", "r_sunshafts 1" );
		m_pSunBox->AddItem( "#GameUI_Medium", "r_sunshafts 2" );
		m_pSunBox->AddItem( "#GameUI_High", "r_sunshafts 3" );
	}

	LoadControlSettings( "Resource/UI/C17OptionsPanel.res" );
	CenterThisPanelOnScreen();
	SetVisible( false );
	SetSizeable( false );
}

void CC17OptionsPanel::OnApplyChanges()
{
	m_pRadBox->ApplyChanges();
	m_pHeadBox->ApplyChanges();
	m_pAABox->ApplyChanges();
	m_pMCBox->ApplyChanges();
	m_pSndBox->ApplyChanges();
	m_pMBBox->ApplyChanges();
	m_pShadowFixBox->ApplyChanges();
	m_pMuzzleFlashProj->ApplyChanges();
	m_pProjectedShadows->ApplyChanges();
	m_pSunBox->ApplyChanges();
}

void CC17OptionsPanel::SetComboBoxDefaults()
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

	//cl_viewbob_enabled
	m_pHeadBox->ActivateItem( cl_viewbob_enabled.GetBool() );

	//Software AA
	ConVar *aaquality = cvar->FindVar( "mat_software_aa_quality" );
	ConVar *aastrength = cvar->FindVar( "mat_software_aa_strength" );
	if( (aaquality->GetInt() == 1) && (aastrength->GetInt() == 1) )
	{
		m_pAABox->ActivateItem( 1 );
	}
	else
	{
		m_pAABox->ActivateItem( 0 );
	}

	ConVar *threadedrender = cvar->FindVar( "r_threaded_renderables" );
	ConVar *queuedpost = cvar->FindVar( "r_queued_post_processing" );
	ConVar *queuedropes = cvar->FindVar( "r_queued_ropes" );
	ConVar *threadedparticles = cvar->FindVar( "r_threaded_particles" );
	if( (threadedrender->GetBool()) && (r_queued_decals_desired.GetBool()) && (queuedpost->GetBool()) && (threadedparticles->GetBool()) && (queuedropes->GetBool()) && (r_threaded_client_shadow_manager.GetBool()) )
	{
		m_pMCBox->ActivateItem( 1 );
	}
	else
	{
		m_pMCBox->ActivateItem( 0 );
	}

	ConVar *sndquality = cvar->FindVar( "dsp_enhance_stereo" );
	m_pSndBox->ActivateItem( sndquality->GetInt() );

	ConVar *forwardMB = cvar->FindVar( "mat_motion_blur_forward_enabled" );
	m_pMBBox->ActivateItem( forwardMB->GetBool() );

	ConVar *flashlightfix = cvar->FindVar( "r_flashlight_use_c17_improvements" );
	ConVar *projectedfix = cvar->FindVar( "r_projected_use_c17_improvements" );
	if( flashlightfix->GetBool() && projectedfix->GetBool() )
	{
		m_pShadowFixBox->ActivateItem( 0 );
	}
	else
	{
		m_pShadowFixBox->ActivateItem( 1 );
	}

	ConVar *projectedmuzzle = cvar->FindVar( "c17_enable_muzzle_flash_light" );
	if( projectedmuzzle->GetBool() )
	{
		m_pMuzzleFlashProj->ActivateItem( 1 );
	}
	else
	{
		m_pMuzzleFlashProj->ActivateItem( 0 );
	}

	ConVar *allowshadows = cvar->FindVar( "r_projected_allow_shadows" );
	if( allowshadows->GetBool() )
	{
		m_pProjectedShadows->ActivateItem( 1 );
	}
	else
	{
		m_pProjectedShadows->ActivateItem( 0 );
	}

	//FIXME: Finish shader and enable normal functionality
	m_pSunBox->ActivateItem( 0 );
	//m_pSunBox->ActivateItem( r_sunshafts.GetInt() );
}

void CC17OptionsPanel::SetVisible(bool state)
{
	if( state )
	{
		SetComboBoxDefaults();
	}

	BaseClass::SetVisible( state );
}

void CC17OptionsPanel::OnCommand(const char* pcCommand)
{
	if(!Q_stricmp(pcCommand, "cancel"))
	{
		SetVisible( false );
	}
	
	if(!Q_stricmp(pcCommand, "ok"))
	{
		OnApplyChanges();
		SetVisible( false );
	}

	if(!Q_stricmp(pcCommand, "apply"))
	{
		OnApplyChanges();
	}
}