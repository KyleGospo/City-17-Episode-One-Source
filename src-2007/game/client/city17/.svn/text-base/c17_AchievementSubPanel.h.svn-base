//================================ Copyright © 2009, Team Gear =============================//
//
// Purpose: City 17's achievements dialog sub panel. Build from code given to the VDC by lodle.
//
//==========================================================================================//
#ifndef ACHIEVEMENTSUBPANEL_H
#define ACHIEVEMENTSUBPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/panel.h>
#include <vgui_controls/Label.h>

#include "pd_panel_list.h"

using namespace vgui;

class AchievementSubPanel : public PD_BasePanel
{
	DECLARE_CLASS_SIMPLE( AchievementSubPanel, PD_BasePanel );

	AchievementSubPanel( Panel *parent, const char *name, const char *desc );

	void SetAchievementName( char* name ){ Q_strncpy(m_szAchievementName, name, 512); m_bNeedsUpdate=true; }
	void SetAchievementDesc( char* desc ){ Q_strncpy(m_szAchievementDesc, desc, 1024); m_bNeedsUpdate=true; }

private:
	void OnThink( void );
	void PaintBackground( void );

	vgui::Label *m_pAchievementNameLabel;
	vgui::Label *m_pAchievementDescriptionLabel;

	bool m_bNeedsUpdate;
	char m_szAchievementName[512];
	char m_szAchievementDesc[1024];
};

#endif //ACHIEVEMENTSUBPANEL_H
