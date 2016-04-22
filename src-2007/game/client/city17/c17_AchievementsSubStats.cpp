//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's Achievements dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Stats Achievements submenu.
//
//==========================================================================================//

#include "cbase.h"
#include "c17_AchievementsSubStats.h"

// Maxsi Distribution
#include "Maxsi/MaxsiDistributionSDK.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <stdio.h>
#include <vgui_controls/ListPanel.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17AchievementsSubStats::CC17AchievementsSubStats( vgui::Panel *parent, const char *name ) : PropertyPage(parent, NULL)
{
	//Create our achivements window here.
	m_pStatsPanel = new ListPanel(this, "StatsPanel");
	m_pStatsPanel->AddColumnHeader( 0, "Statistic", "Statistic", 200, ListPanel::COLUMN_RESIZEWITHWINDOW );
	m_pStatsPanel->AddColumnHeader( 1, "Value", "Value", 100, ListPanel::COLUMN_RESIZEWITHWINDOW );

	//Set up the list keyvalue.
	KeyValues *kv = new KeyValues( "LI" );

	//Set up the first Statistic option
	kv->SetString( "Statistic", "Highest Map Played:" );
	kv->SetString( "Value", "c17_city_day_01a" );
	m_pStatsPanel->AddItem( kv, 0, false, false );

	//Set up the second Statistic option
	kv->SetString( "Statistic", "Combine Killed:" );
	kv->SetString( "Value", "564" );
	m_pStatsPanel->AddItem( kv, 0, false, false );

	//Delete the list keyvalue.
	kv->deleteThis();

	LoadControlSettings("Resource\\C17_Stats_Achievements.res");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17AchievementsSubStats::~CC17AchievementsSubStats()
{
	delete m_pStatsPanel;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17AchievementsSubStats::OnResetData()
{
	
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CC17AchievementsSubStats::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	m_pStatsPanel->SetFont( pScheme->GetFont( "Default" ) );
}