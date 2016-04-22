/*---------------------------------------------------------------------------*
/ File Name: vgui_falconpanel.h
/ Author: Anthony Iacono
/ Ending Date: April 19, 2007
/ Ending Time: 5:51 PM - Pacific Time (GMT -08:00)
/ Ideal File Location: src\cl_dll\VGUIFalcon\vgui_falconpanel.h
/ Description: Declare the necessary class declarations
\*---------------------------------------------------------------------------*/

#ifndef FALCONPANEL_H
#define FALCONPANEL_H
#ifdef _WIN32
#pragma once
// disabling macro redefinition warning from protected things due to windows include.
#pragma warning(disable:4005)
#endif
#include "vgui_helpers.h"
#include "vgui_falconpanel_macros.h"
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/PropertyDialog.h>
#include "HapticPages.h"
#include <KeyValues.h>
using namespace vgui;

// this should be changed to a static lookup of how many buttons are available once more support for seperate attachments.
class CFalconPanel : public PropertyDialog
{
	DECLARE_CLASS_SIMPLE(CFalconPanel,PropertyDialog);
public:
	CFalconPanel( vgui::VPANEL parent );

	// virtual overrides
	//virtual void Think();
	void Trickery();
	
	// our functions
	void CheckandCreateElements();

	// one function to set to the first page
	void SetToDefaultPage();
	void OnDefault();
protected:
	// only for cursor emulation.
	virtual void OnTick();
	virtual void OnCommand(const char *command);
	virtual void OnCancel();
	virtual bool OnOK(bool applyOnly);
private:
	// true once elements are created.
	bool m_bCreatedElements;
	vgui::Button *m_pButtonDefault;

	// all the available binds from kb_act.lst
	CUtlVector<CHapticsPage*>HapPages;
};
class IFalconPanel
{
public:
	void Create(vgui::VPANEL parent)
	{
		panel = new CFalconPanel(parent);
		DevMsg("Created Falcon Panel!\n");
	}
	void Destroy()
	{
		panel->DeletePanel();
	}
private:
	CFalconPanel *panel;
};
static IFalconPanel g_iFalcon;
static IFalconPanel *falconpanel = &g_iFalcon;
#endif // FALCONPANEL_H 