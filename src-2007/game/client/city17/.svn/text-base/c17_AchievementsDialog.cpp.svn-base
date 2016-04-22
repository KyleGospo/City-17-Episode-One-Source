//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's achievements dialog, built from NightFall code, originally created by Matt Stafford. (And a little something else as well)
//
//==========================================================================================//

#include "cbase.h"
#include "c17_AchievementsDialog.h"
//#include "FileSystem.h"
#include "KeyValues.h"
#include "ienginevgui.h"

#include "c17_AchievementsSubMain.h"
#include "c17_AchievementsSubStats.h"

#include <vgui_controls/Button.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/Label.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/IVGui.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui/ILocalize.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

static CC17AchievementsDialog *g_pC17AchievementsDialog = NULL;

CON_COMMAND(OpenC17AchievementsDialog, "Opens the City17 Achievements Dialog")
{
	if ( !g_pC17AchievementsDialog )
	{
		g_pC17AchievementsDialog = new CC17AchievementsDialog( NULL );
		g_pC17AchievementsDialog->Run();
		g_pC17AchievementsDialog->SetVisible( false );
	}

	if ( !g_pC17AchievementsDialog->IsVisible() )
	{
		g_pC17AchievementsDialog->Run();
	}
	else
	{
		g_pC17AchievementsDialog->SetVisible( false );
	}
};

void C17Achievements_Destroy()
{
	if( g_pC17AchievementsDialog != NULL )
	{
		g_pC17AchievementsDialog->DestroySelf();
	}
}

//-----------------------------------------------------------------------------
// CC17AchievementsDialog - Constructor
//-----------------------------------------------------------------------------
CC17AchievementsDialog::CC17AchievementsDialog(vgui::Panel *parent) : PropertyDialog(parent, "AchievementsDialog")
{
	vgui::VPANEL pParent = enginevgui->GetPanel( PANEL_GAMEUIDLL );
	SetParent( pParent );
	SetBounds(0, 0, 630, 464);
	SetSizeable( false );
	SetPaintBackgroundEnabled( true );
	SetPaintBorderEnabled( true );
	SetDeleteSelfOnClose( true );

	SetTitle("#GameUI_C17Achievements", true);

	m_pC17MainPage = new CC17AchievementsSubMain( this, "MainPage" );
	m_pC17StatsPage = new CC17AchievementsSubStats( this, "StatsPage" );
	AddPage(m_pC17MainPage, "#GameUI_C17Achievements_Main");
	AddPage(m_pC17StatsPage, "#GameUI_C17Achievements_Stats");
	
	SetApplyButtonVisible( false );

	GetPropertySheet()->SetTabWidth(72);
}

//-----------------------------------------------------------------------------
// ~CC17AchievementsDialog - Destructor
//-----------------------------------------------------------------------------
CC17AchievementsDialog::~CC17AchievementsDialog()
{
	SetParent( (vgui::Panel *) NULL );

	if( g_pC17AchievementsDialog )
	{
		g_pC17AchievementsDialog->MarkForDeletion();
		g_pC17AchievementsDialog = NULL;
	}

	// Do we actually need to delete these?
	if( m_pC17MainPage )
	{
		m_pC17MainPage->MarkForDeletion();
		m_pC17MainPage = NULL;
	}

	if( m_pC17StatsPage )
	{
		m_pC17StatsPage->MarkForDeletion();
		m_pC17StatsPage = NULL;
	}
}

//-----------------------------------------------------------------------------
// Activate - Activates the dialog & brings it to the front
//-----------------------------------------------------------------------------
void CC17AchievementsDialog::DestroySelf()
{
	//Technically, this should be the same thing as FinishClose.
	SetVisible( false );
	MarkForDeletion();
}

//-----------------------------------------------------------------------------
// Activate - Activates the dialog & brings it to the front
//-----------------------------------------------------------------------------
void CC17AchievementsDialog::Activate()
{
	BaseClass::Activate();

	EnableApplyButton( false );
}

//-----------------------------------------------------------------------------
// Run: Opens the dialog
//-----------------------------------------------------------------------------
void CC17AchievementsDialog::Run()
{
	SetTitle("#GameUI_C17Achievements", true);
	Activate();
	PositionDialog();
}

//-----------------------------------------------------------------------------
// OnOK: Fired when the OK button is clicked
//-----------------------------------------------------------------------------
bool CC17AchievementsDialog::OnOK( bool applyOnly )
{
	BaseClass::OnOK( true );

	return true;
}

void CC17AchievementsDialog::OnCommand(const char *command)
{
	BaseClass::OnCommand( command );

	EnableApplyButton( true );
}

//-----------------------------------------------------------------------------
// OnClose:
//-----------------------------------------------------------------------------
void CC17AchievementsDialog::OnClose()
{
	BaseClass::OnClose();
}

//-----------------------------------------------------------------------------
// PositionDialog: Positions the dialog in the center of the screen,
// wmaintaining a correct size
//-----------------------------------------------------------------------------
void CC17AchievementsDialog::PositionDialog( void )
{
	int x, y, ww, wt, wide, tall;
	vgui::surface()->GetWorkspaceBounds( x, y, ww, wt );
	g_pC17AchievementsDialog->GetSize(wide, tall);

	g_pC17AchievementsDialog->SetPos(x + ((ww - wide) / 2), y + ((wt - tall) / 2));
}