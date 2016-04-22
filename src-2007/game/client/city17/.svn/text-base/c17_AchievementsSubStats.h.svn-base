//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's achievements dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Stats Achievements submenu.
//
//==========================================================================================//

#ifndef C17_ACHIEVEMENTS_SUB_STATS_H
#define C17_ACHIEVEMENTS_SUB_STATS_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>

/*namespace vgui
{
    class Label;
    class Panel;
}*/

using namespace vgui;

class CC17AchievementsSubStats : public vgui::PropertyPage
{
public:
	CC17AchievementsSubStats( vgui::Panel *parent, const char *name );
	~CC17AchievementsSubStats();

	virtual void OnResetData();

	void SetupTooltips( void );

protected:
    virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	ListPanel		*m_pStatsPanel;

	typedef vgui::PropertyPage BaseClass;
};



#endif // C17_ACHIEVEMENTS_SUB_STATS_H
