#ifndef HAPTICPAGES_H
#define HAPTICPAGES_H
#ifdef _WIN32
#pragma once
#endif

#include "haptic_vgui.h"
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/PropertyPage.h>
#define HAPTIC_UI_FILE "resource/ui/falcon.hap"

// used for lookup
class CHapticsPage : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE(CHapticsPage,vgui::PropertyPage);
public:
	CHapticsPage(vgui::PropertySheet *parent, KeyValues* pageData);
	virtual void OnPageShow();
	virtual void OnCommand(const char *command);
protected:
	virtual void SaveVars();
	virtual void LoadDefaultVars();
	virtual void ResetVars();
	virtual void InitControls();
	virtual const char *GetTabTitle();
	// each CVar enabled types should have a vector here.
	KeyValues *m_pKeys;
	void AddCVarControl( Panel *controll );
	CUtlVector<Panel*>m_controls;
};

// button config page
class CHapticsButtonPage : public CHapticsPage
{
	DECLARE_CLASS_SIMPLE(CHapticsButtonPage,CHapticsPage);
public:
	CHapticsButtonPage(vgui::PropertySheet *parent, KeyValues* pageData);
protected:
	virtual void InitControls();
	virtual const char *GetTabTitle();
private:
	vgui::Label *m_pInstructionLable;
	CUtlVector< ButtonBindCombo *> ButtonCombos;
};
// movement config page
class CHapticsMovementPage : public CHapticsPage
{
	DECLARE_CLASS_SIMPLE(CHapticsMovementPage,CHapticsPage);
public:
	
	CHapticsMovementPage(vgui::PropertySheet *parent, KeyValues* pageData);
	
	virtual const char *GetTabTitle();
protected:
	// sets check buttons to correct value when new cvar values are in ( from default or reset )
	void OnChangedCvars();
	// unused.
	virtual void InitControls();
	// calls OnChangedCvars ( this could maybe be moved to base class? )
	virtual void ResetVars();
	// calls OnChangedCvars ( this could maybe be moved to base class? )
	virtual void LoadDefaultVars();
	// second pass of variables related to checkbuttons.
	virtual void SaveVars();

	vgui::Label * m_pLabel;
	vgui::CheckButton * m_pReverseYaw;
	vgui::CheckButton * m_pReversePitch;
};
class CHapticsControlBoxPage : public CHapticsPage
{
	DECLARE_CLASS_SIMPLE(CHapticsControlBoxPage,CHapticsPage);
public:
	CHapticsControlBoxPage(vgui::PropertySheet *parent, KeyValues *pagedata);
	virtual const char *GetTabTitle();
protected:
	vgui::PropertySheet *m_pSheet;
};
class CHapticsForcesPage : public CHapticsPage
{
	DECLARE_CLASS_SIMPLE(CHapticsForcesPage,CHapticsPage);
public:
	CHapticsForcesPage(vgui::PropertySheet *parent, KeyValues* pageData);
	virtual const char *GetTabTitle();
};
#endif // HAPTICPAGES_H