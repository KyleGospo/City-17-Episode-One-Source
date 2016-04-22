//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's Achievements dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Main Achievements submenu.
//
//==========================================================================================//

#include "cbase.h"
#include "c17_AchievementsSubMain.h"
#include "pd_panel_list.h"
#include "c17_achievementsubpanel.h"

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
CC17AchievementsSubMain::CC17AchievementsSubMain( vgui::Panel *parent, const char *name ) : PropertyPage(parent, NULL)
{
	//Create our achivements window here.
	m_pAchievementsList = new PD_PanelList(this, "Achievements_List");

	//sets the x, y pos and the w, h
	m_pAchievementsList->SetBounds( 0, 0, 600, 330 );

	//Very Important. Means it will relay messages to this panel
	m_pAchievementsList->AddActionSignalTarget( this );

	//usefull if you want key commands to go up/down
	m_pAchievementsList->SetKeyBoardInputEnabled( true );

	//----------------------------
	// ACHIEVEMENTS START HERE
	//----------------------------
	if ( g_MaxsiAchievements )
	{
		MaxsiAchievement** Achievements;
		size_t NumAchievements;

		g_MaxsiAchievements->GetAchievements(&Achievements, &NumAchievements);

		if ( Achievements )
		{
			for ( size_t I = 0; I < NumAchievements; I++ )
			{
				//Setup our achivement name and description.
				char* Name = Achievements[I]->GetAchievementName();
				char* Desc = Achievements[I]->GetAchievementDescription();
		
				//Create the achivement panel.
				AchievementSubPanel *temp = new AchievementSubPanel( m_pAchievementsList, Name, Desc );
				temp->SetAchievementName( Name );
				temp->SetAchievementDesc( Desc );

				//Display the achivement in the console as a developer message.
				DevMsg( 1, "%s\n", Name );
				DevMsg( 1, "%s\n", Desc );
				DevMsg( 1, "---\n" );
				
				// if ->GetProgressMax() < 2 -- No progress bars.
		
				//Add the panel.
				if ( m_pAchievementsList->AddPanel( temp, false ) == -1 )
				{
					//If the panel is invalid (-1), delete it. This is needed because of a bug in the panel list control.
					delete temp;
				}

				m_pAchievementsList->ResetAllChildPos();
			}
		}
		g_MaxsiAchievements->DeleteAchievementList(Achievements, NumAchievements);
	}
	//----------------------------
	// END ACHIEVEMENTS
	//----------------------------


	LoadControlSettings("Resource\\C17_Main_Achievements.res");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CC17AchievementsSubMain::~CC17AchievementsSubMain()
{
	delete m_pAchievementsList;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17AchievementsSubMain::OnResetData()
{

}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void CC17AchievementsSubMain::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}