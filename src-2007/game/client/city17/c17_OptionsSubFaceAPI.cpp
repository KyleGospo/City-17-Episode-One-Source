//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's options dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the FaceAPI Options submenu.
//
//==========================================================================================//

#include "cbase.h"
#include "c17_OptionsSubFaceAPI.h"
#include "nf_CvarComboBox.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <stdio.h>
/*#include <vgui_controls/TextEntry.h>
#include <vgui_controls/checkbutton.h>
#include <vgui_controls/slider.h>
#include <vgui_controls/Tooltip.h>*/

#include "facesdk/head_tracking_convars.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubFaceAPI::CC17OptionsSubFaceAPI( vgui::Panel *parent, const char *name ) : PropertyPage(parent, NULL)
{
	m_pFaceBox = new CCvarComboBox(this, "FaceBox");
	if ( m_pFaceBox )
	{
		m_pFaceBox->AddItem( "#GameUI_Disabled", "faceapi_mode 0" );
		m_pFaceBox->AddItem( "#GameUI_C17PeeringMode", "faceapi_mode 1" );
		m_pFaceBox->AddItem( "#GameUI_C17WindowMode", "faceapi_mode 2" );
		m_pFaceBox->AddItem( "#GameUI_C17BothFMode", "faceapi_mode 3" );
	}

	LoadControlSettings("Resource\\C17_FaceAPI_Options.res");
}

void CC17OptionsSubFaceAPI::SetComboBoxDefaults()
{
	if( faceapi_mode.GetInt() == 0 )
	{
		m_pFaceBox->ActivateItem( 0 );
	}
	else if( faceapi_mode.GetInt() == 1 )
	{
		m_pFaceBox->ActivateItem( 1 );
	}
	else if( faceapi_mode.GetInt() == 2 )
	{
		m_pFaceBox->ActivateItem( 2 );
	}
	else
	{
		m_pFaceBox->ActivateItem( 3 );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17OptionsSubFaceAPI::~CC17OptionsSubFaceAPI()
{
	delete m_pFaceBox;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubFaceAPI::OnResetData()
{
	m_pFaceBox->Reset();
	SetComboBoxDefaults();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17OptionsSubFaceAPI::OnApplyChanges()
{
	m_pFaceBox->ApplyChanges();
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CC17OptionsSubFaceAPI::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}