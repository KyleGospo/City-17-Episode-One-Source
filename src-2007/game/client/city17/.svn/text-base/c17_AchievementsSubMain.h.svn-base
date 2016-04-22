//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's achievements dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Main Achievements submenu.
//
//==========================================================================================//

#ifndef C17_ACHIEVEMENTS_SUB_MAIN_H
#define C17_ACHIEVEMENTS_SUB_MAIN_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>

class PD_PanelList;

namespace vgui
{
    class Label;
    class Panel;
}


class CC17AchievementsSubMain : public vgui::PropertyPage
{
public:
	CC17AchievementsSubMain( vgui::Panel *parent, const char *name );
	~CC17AchievementsSubMain();

	virtual void OnResetData();

	void SetupTooltips( void );

protected:
    virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	PD_PanelList		*m_pAchievementsList;

	typedef vgui::PropertyPage BaseClass;
};



#endif // C17_ACHIEVEMENTS_SUB_MAIN_H
