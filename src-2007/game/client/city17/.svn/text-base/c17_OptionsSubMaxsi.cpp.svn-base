//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's options dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Maxsi Options submenu.
//
//==========================================================================================//

#include "cbase.h"
#include "c17_OptionsSubMaxsi.h"
#include "nf_CvarComboBox.h"

// Maxsi Distribution
#include "Maxsi/MaxsiDistributionSDK.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <stdio.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubMaxsi::CC17OptionsSubMaxsi( vgui::Panel *parent, const char *name ) : PropertyPage(parent, NULL)
{
	m_pMDBox = new CCvarComboBox(this, "MDBox");
	if ( m_pMDBox )
	{
		m_pMDBox->AddItem( "#GameUI_C17_MaxsiDisabled", "" );
		m_pMDBox->AddItem( "#GameUI_C17_MaxsiEnabled", "" );
	}

	LoadControlSettings("Resource\\C17_Maxsi_Options.res");
}

void CC17OptionsSubMaxsi::SetComboBoxDefaults()
{
	m_pMDBox->ActivateItem( 1 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubMaxsi::~CC17OptionsSubMaxsi()
{
	delete m_pMDBox;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubMaxsi::OnResetData()
{
	m_pMDBox->Reset();
	SetComboBoxDefaults();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubMaxsi::OnApplyChanges()
{
	m_pMDBox->ApplyChanges();
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CC17OptionsSubMaxsi::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}