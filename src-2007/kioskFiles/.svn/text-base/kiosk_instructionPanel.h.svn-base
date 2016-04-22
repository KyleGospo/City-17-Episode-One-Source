/*---------------------------------------------------------------------------*
/ File Name: vgui_falconpanel.h
/ Author: Anthony Iacono
/ Ending Date: April 19, 2007
/ Ending Time: 5:51 PM - Pacific Time (GMT -08:00)
/ Ideal File Location: src\cl_dll\VGUIFalcon\vgui_falconpanel.h
/ Description: Declare the necessary class declarations
\*---------------------------------------------------------------------------*/
//KIOSK ONLY!
#ifdef HAP_DEMO
#ifndef INSTRUCTIONPANEL_H
#define INSTRUCTIONPANEL_H
#ifdef _WIN32
#pragma once
// disabling macro redefinition warning from protected things due to windows include.
#pragma warning(disable:4005)
#endif
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/PropertyDialog.h>
using namespace vgui;

// this should be changed to a static lookup of how many buttons are available once more support for seperate attachments.
class CInstructionPanel : public PropertyDialog
{
	DECLARE_CLASS_SIMPLE(CInstructionPanel,PropertyDialog);
public:
	CInstructionPanel( vgui::VPANEL parent );
	void ShowInstructions(const char *pImage, const char *pCommand);
	virtual void OnTick();
	const char *GetIngameImageURL(){return m_cIngameURL;};
protected:
	virtual void OnCommand(const char *command);
private:
	vgui::ImagePanel *m_pImagePanel;
	vgui::Label *m_pTimeLabel;
	const char *m_pCommandOK;
	char m_cCommand[256];
	char m_cIngameURL[256];
	int timeleft;

};
class IInstructionPanel
{
public:
	void Create(vgui::VPANEL parent)
	{
		panel = new CInstructionPanel(parent);
	}
	void Destroy()
	{
		panel->DeletePanel();
	}
private:
	CInstructionPanel *panel;
};
static IInstructionPanel g_iInstr;
static IInstructionPanel *instructionpanel = &g_iInstr;
extern CInstructionPanel *instPanel;
#endif // INSTRUCTIONPANEL_H 
#endif // HAP_DEMO