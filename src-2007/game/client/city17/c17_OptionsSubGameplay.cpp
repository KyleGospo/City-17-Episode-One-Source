//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's options dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Gameplay Options submenu.
//
//==========================================================================================//

#include "cbase.h"
#include "c17_OptionsSubGameplay.h"
#include "nf_CvarComboBox.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <stdio.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

extern ConVar cl_viewbob_enabled;
extern ConVar cl_jumpkick_enabled;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubGameplay::CC17OptionsSubGameplay( vgui::Panel *parent, const char *name ) : PropertyPage(parent, NULL)
{
	m_pHeadBox = new CCvarComboBox(this, "HeadBox");
	if ( m_pHeadBox )
	{
		m_pHeadBox->AddItem( "#GameUI_Disabled", "cl_headbob_enabled 0;cl_jumpkick_enabled 0;sv_rollangle 0" );
		m_pHeadBox->AddItem( "#GameUI_Enabled", "cl_headbob_enabled 1;cl_jumpkick_enabled 1;sv_rollangle 1.5" );
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

	m_pCrossBox = new CCvarComboBox(this, "CrossBox");
	if ( m_pCrossBox )
	{
		m_pCrossBox->AddItem( "#GameUI_C17HUDMod0", "crosshair 1;cl_drawhud 1" );
		m_pCrossBox->AddItem( "#GameUI_C17HUDMod1", "crosshair 0;cl_drawhud 1" );
		m_pCrossBox->AddItem( "#GameUI_C17HUDMod2", "crosshair 0;cl_drawhud 0" );
	}

	m_pIronBox = new CCvarComboBox(this, "IronBox");
	if ( m_pIronBox )
	{
		m_pIronBox->AddItem( "#GameUI_C17IronToggle", "ironsight_mode 0" );
		m_pIronBox->AddItem( "#GameUI_C17IronHold", "ironsight_mode 1" );
	}

	LoadControlSettings("Resource\\C17_Gameplay_Options.res");
}

void CC17OptionsSubGameplay::SetComboBoxDefaults()
{
	if( cl_viewbob_enabled.GetBool() && cl_jumpkick_enabled.GetBool() )
	{
		m_pHeadBox->ActivateItem( 1 );
	}
	else
	{
		m_pHeadBox->ActivateItem( 0 );
	}

	ConVarRef sndquality( "dsp_enhance_stereo" );
	m_pSndBox->ActivateItem( sndquality.GetInt() );

	ConVarRef forwardMB( "mat_motion_blur_forward_enabled" );
	m_pMBBox->ActivateItem( forwardMB.GetBool() );

	ConVarRef crosshair( "crosshair" );
	ConVarRef drawhud( "cl_drawhud" );
	if( !drawhud.GetBool() )
	{
		m_pCrossBox->ActivateItem( 2 );
	}
	else if( !crosshair.GetBool() )
	{
		m_pCrossBox->ActivateItem( 1 );
	}
	else
	{
		m_pCrossBox->ActivateItem( 0 );
	}

	ConVarRef ironsightmode( "ironsight_mode" );
	m_pIronBox->ActivateItem( ironsightmode.GetBool() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubGameplay::~CC17OptionsSubGameplay()
{
	delete m_pHeadBox;
	delete m_pSndBox;
	delete m_pMBBox;
	delete m_pCrossBox;
	delete m_pIronBox;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubGameplay::OnResetData()
{
	m_pHeadBox->Reset();
	m_pSndBox->Reset();
	m_pMBBox->Reset();
	m_pCrossBox->Reset();
	m_pIronBox->Reset();
	SetComboBoxDefaults();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubGameplay::OnApplyChanges()
{
	m_pHeadBox->ApplyChanges();
	m_pSndBox->ApplyChanges();
	m_pMBBox->ApplyChanges();
	m_pCrossBox->ApplyChanges();
	m_pIronBox->ApplyChanges();
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CC17OptionsSubGameplay::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}