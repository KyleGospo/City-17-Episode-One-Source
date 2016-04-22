//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's Achievements dialog, built from NightFall code, originally created by Matt Stafford.
//
//==========================================================================================//

#ifndef C17_ACHIEVEMENTSDIALOG_H
#define C17_ACHIEVEMENTSDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include "c17_AchievementsSubMain.h"
#include "c17_AchievementsSubStats.h"
#include <vgui_controls/Frame.h>
#include <vgui_controls/PropertyDialog.h>

namespace vgui
{
class Button;
class CheckButton;
class Label;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CC17AchievementsDialog : public vgui::PropertyDialog
{
public:
	CC17AchievementsDialog(vgui::Panel *parent);
	~CC17AchievementsDialog();

	void DestroySelf();

	void Run();
	virtual void Activate();
	void PositionDialog( void );

private:
	CC17AchievementsSubMain *m_pC17MainPage;
	CC17AchievementsSubStats *m_pC17StatsPage;

protected:
	virtual bool OnOK( bool applyOnly );
	virtual void OnClose();
	virtual void OnCommand(const char *command);

	typedef vgui::PropertyDialog BaseClass;


};

#endif // C17_ACHIEVEMENTSDIALOG_H
