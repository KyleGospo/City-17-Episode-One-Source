#ifndef HAPTICS_DEMO_UI_H
#define HAPTICS_DEMO_UI_H
#pragma once
class CTimerUpPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CTimerUpPanel, vgui::Frame);
public:
	CTimerUpPanel(vgui::VPANEL parent);
	void ExitGame();
protected:
	virtual void OnTick();
private:
	vgui::Label *exitMessage;
};
class CSleepTimer : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CSleepTimer,vgui::Frame);
public:
	CSleepTimer(vgui::VPANEL parent);
protected:
	virtual void OnTick();
private:
	vgui::Label *m_pMinutes;
	vgui::Label *m_pSeconds;
	CTimerUpPanel *timeUp;
};
extern CSleepTimer *g_pSleepTimer;
#endif