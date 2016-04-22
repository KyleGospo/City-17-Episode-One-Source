#include <cbase.h>
#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/PropertyDialog.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/button.h>
//#include "keydefs.h"
#include "inputsystem/ButtonCode.h"
#ifdef HAP_DEMO
#include "kiosk_instructionPanel.h"
#endif //HAP_DEMO

#undef INVALID_HANDLE_VALUE
#undef GetCommandLine
#undef ReadConsoleInput
#undef RegCreateKey
#undef RegCreateKeyEx
#undef RegOpenKey
#undef RegOpenKeyEx
#undef RegQueryValue
#undef RegQueryValueEx
#undef RegSetValue
#undef RegSetValueEx
#include <windows.h>

// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
#ifdef HAP_DEMO
CInstructionPanel *instPanel = null;
CInstructionPanel::CInstructionPanel(vgui::VPANEL parent) : BaseClass(null,"instructionPanel")
{
	SetParent(parent);
	SetSize(512-5,512+40-9);//40 is height of the bottom content with ok and cancle button.
	SetPos(	ScreenWidth()/2 - GetWide()/2,
			ScreenHeight()/2 - GetTall()/2 );
	SetVisible(false);
	SetMoveable(false);
	SetSizeable(false);
	SetProportional(false);
	SetTitle("Instructions",true);
	instPanel=this;
	SetOKButtonVisible(false);
	SetCancelButtonVisible(false);
	//SetApplyButtonVisible(true);
	SetCloseButtonVisible(true);
	m_pImagePanel = new vgui::ImagePanel(this,"instructionImage");
	m_pImagePanel->SetSize(490,476);
	m_pImagePanel->SetPos(8,27);
	vgui::Button *okB = new vgui::Button(this,"demoStart","Start",this,"startDemo");
	m_pTimeLabel = new vgui::Label(this,"warningL","#kiosk_press_start");
	m_pTimeLabel->SetSize(416,24);
	m_pTimeLabel->SetPos(8,509);
	m_pTimeLabel->SetVisible(true);
	okB->SetPos(419,509);
	okB->SetSize(72,24);
	timeleft=60;
}
ConVar kiosk_start_time("kiosk_start_time","60",FCVAR_ARCHIVE);
void SimEscapePress( BOOL bState )
{
  BYTE keyState[256];

  GetKeyboardState((LPBYTE)&keyState);
  if( (bState && !(keyState[VK_ESCAPE] & 1)) ||
      (!bState && (keyState[VK_ESCAPE] & 1)) )
  {
  // Simulate a key press
     keybd_event( VK_ESCAPE,
                  0,
                  0,
                  0 );

  // Simulate a key release
     keybd_event( VK_ESCAPE,
                  0,
                  KEYEVENTF_KEYUP,
                  0);
  }
}
CON_COMMAND(toggle_main_menu,"simulates a escape press")
{
	SimEscapePress(true);
	SimEscapePress(false);
}
void CInstructionPanel::ShowInstructions(const char *pImage, const char *pCommand)
{
	if(!engine->IsPaused())
	{
		//SimEscapePress(true);
	}
	V_snprintf(m_cCommand,sizeof(m_cCommand),"exec %s\n",pCommand);
	m_pCommandOK = m_cCommand;
	m_pImagePanel->SetImage(pImage);
	V_snprintf(m_cIngameURL,sizeof(m_cIngameURL),"%s_ingame",pImage);
	SetVisible(true);
	Activate();
	timeleft = kiosk_start_time.GetInt();
	vgui::ivgui()->AddTickSignal(GetVPanel(),1000);
}


void CInstructionPanel::OnCommand(const char *command)
{
	if(!V_stricmp(command,"startDemo"))
	{

		engine->ClientCmd(m_pCommandOK);
		SetVisible(false);
		vgui::ivgui()->RemoveTickSignal(GetVPanel());
	};

}
ConVar kiosk_warnTime("kiosk_warnTime","50");
ConVar kiosk_quitTime("kiosk_quitTime","2");
void CInstructionPanel::OnTick()
{
	if(engine->IsInGame())
		engine->ClientCmd( "gameui_activate\n");
	timeleft--;
	if(timeleft<=kiosk_warnTime.GetInt()&&timeleft>0)
	{
		char message1[80];
		char message[80];
		m_pTimeLabel->SetText(vgui::localize()->Find("#kiosk_press_start_auto"));
		m_pTimeLabel->GetText(message1,sizeof(message1));
		Q_snprintf(message,sizeof(message),"%s%i )",message1,timeleft);
		m_pTimeLabel->SetText(message);
	}else if ( timeleft<=0)
	{
		m_pTimeLabel->SetText("#kiosk_press_start_auto_starting");

	}
	if(timeleft<=-kiosk_quitTime.GetInt())
	{
		engine->ClientCmd("exit\n");
	}
}
CON_COMMAND(kiosk_showpanel,"usage kiosk_showpanel [image] [command on okay button]")
{
	if(engine->Cmd_Argc()>2)
	{
		instPanel->ShowInstructions(engine->Cmd_Argv(1),engine->Cmd_Argv(2));
	}
}
#endif//HAP_DEMO