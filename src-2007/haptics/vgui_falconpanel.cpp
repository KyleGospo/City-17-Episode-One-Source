/*---------------------------------------------------------------------------*
/ File Name: vgui_falconpanel.cpp
/ Author: Anthony Iacono
/ Ending Date: April 19, 2007
/ Ending Time: 5:51 PM - Pacific Time (GMT -08:00)
/ Ideal File Location: src\cl_dll\VGUIFalcon\vgui_falconpanel.cpp
/ Description: Code to create a working and usable options UI for the Falcon
\*---------------------------------------------------------------------------*/

#include "cbase.h"
#include "vgui_falconpanel.h"
#include "vgui_falconpanel_macros.h"
#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/PropertyPage.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/ComboBox.h>
#include "vgui_helpers.h"
#include "filesystem.h"
#include <KeyValues.h>
//#include <keydefs.h>
#include "inputsystem/ButtonCode.h"
#include <kbutton.h>
#include <IGameUIFuncs.h>
#include <vgui/isurface.h>
#include <vgui/IInputInternal.h>
// normal haptics access
#include "../haptics/client_haptics.h"
// low level haptics access
#include "../haptics/in_haptics.h"
// graph class
#include "haptic_graph.h"

// for HWND and GetForgroundWindow 
#include <windows.h>
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"


void SetVGUICursorPos( int x, int y );

/*---------------------------------------------------------------------------*
/ Type: Global Pointer
/ Description: In order to access panel functions outside the class, we need
/ to store a pointer, the actual value is NULL until set in the constructer.
\*---------------------------------------------------------------------------*/
CFalconPanel *panelpointer;

/*---------------------------------------------------------------------------*
/ Type: Console Command
/ Description: Every time the button on the main menu is pressed, this function
/ is called, it does some important stuff for the setting of sliders and stuff.
\*---------------------------------------------------------------------------*/
CON_COMMAND(ToggleFalconPanel,NULL)
{
	if(panelpointer)
	{
		panelpointer->SetToDefaultPage();
		panelpointer->SetVisible(!panelpointer->IsVisible());
		panelpointer->MoveToFront();
		panelpointer->OnSetFocus();
	}

}

CON_COMMAND(hap_debug_tab,NULL)
{
	if(panelpointer)
	{
		vgui::PropertyPage *dbpage = new vgui::PropertyPage(panelpointer->GetPropertySheet(),"DEBUGPAGE");
		new HapticsGraph(dbpage->GetVPanel());
		panelpointer->AddPage(dbpage,"Debug");

	}
}
/*---------------------------------------------------------------------------*
/ Type: Constructer
/ Description: When the panel is first created, this is called, does some basic
/ yet important stuff to get basic initiation.
\*---------------------------------------------------------------------------*/
CFalconPanel::CFalconPanel( vgui::VPANEL parent ) : BaseClass( NULL, "falconpanel" )
{
	SetParent(parent);
	m_bCreatedElements=false;
	m_pButtonDefault = new vgui::Button(this,"DEFAULTBUTTON", "#GameUI_UseDefaults");
	m_pButtonDefault->SetCommand("DefaultPressed");
	// Load the localization.
	//vgui::localize()->AddFileEx(vgui::filesystem(),"resource/nethaptics_%language%.txt","MOD",true);// HL2 ( for weapon names )
	//vgui::localize()->AddFileEx(vgui::filesystem(),"resource/hl2_%language%.txt","MOD",true);// HL2 ( for weapon names )
	//vgui::localize()->AddFileEx(vgui::filesystem(),"resource/haptics_%language%.txt","MOD",true);// Custom Haptics Panel localization.
	g_pVGuiLocalize->AddFile("resource/city17_%language%.txt", "MOD", true);
	g_pVGuiLocalize->AddFile("resource/haptics_%language%.txt", "MOD", true);

	// Load the control settings.
	LoadControlSettings("resource/UI/FalconOptions.res");

	SetSize(512,404);
	SetPos(	ScreenWidth()/2 - GetWide()/2,
			ScreenHeight()/2 - GetTall()/2 );
	SetVisible(false);
	SetMoveable(true);
	SetSizeable(false);
	SetProportional(false);
	panelpointer=this;
	SetOKButtonVisible(true);
	SetCancelButtonVisible(true);
	//SetApplyButtonVisible(true);
	SetCloseButtonVisible(true);
	//For cursor emulation.
	vgui::ivgui()->AddTickSignal(GetVPanel());

	// Add our pages! (first page added will be the default page! tabs left to right)
	HapPages.AddToTail( (CHapticsPage*) new CHapticsMovementPage(GetPropertySheet(),NULL));
	HapPages.AddToTail( (CHapticsPage*) new CHapticsForcesPage(GetPropertySheet(),NULL));
	HapPages.AddToTail( (CHapticsPage*) new CHapticsButtonPage(GetPropertySheet(),NULL));
	HapPages.AddToTail( (CHapticsPage*) new CHapticsControlBoxPage(GetPropertySheet(),NULL));

	int x,y;
	GetPropertySheet()->GetPos(x,y);
	m_pButtonDefault->SetPos(18,370);
	m_pButtonDefault->SetWide(128);
	MakeReadyForUse();
	panelpointer = this;
	EnableApplyButton(true);
}
//cursor convars.
extern ConVar hap_cursor;
extern ConVar hap_debug_cursor;
ConVar hap_cursor_alwayson("hap_cursor_alwayson","1",FCVAR_ARCHIVE);
static bool s_bInGameWindow = false;

//declared in In_Haptics.h
bool WindowIsFocused()
{
	return ( (HWND) vgui::input()->GetIMEWindow() == GetForegroundWindow() );
}

void CFalconPanel::OnCommand(const char *command)
{
	if(!V_stricmp(command, "DefaultPressed"))
	{
		OnDefault();
	}else
		BaseClass::OnCommand(command);
};
void CFalconPanel::OnTick()
{
	if(!InUIMode())
		return;

	if( hap_cursor.GetInt()==1 && cliHaptics )
	{
		int mx,my;
		if(cliHaptics->GetCursorPosition(&mx,&my)||hap_cursor_alwayson.GetInt()==1)
		{
			if(hap_debug_cursor.GetInt()==1)
				Msg("Cursor [ X=%i Y=%i ]\n",mx,my);

			if(WindowIsFocused())
			{
				if(!s_bInGameWindow)
				{
					s_bInGameWindow=true;
					//Pat: 8/23/07 this reinit is no longer neccisary but left in case connection is lost.
					//this is neccisary because unlike in the game input thread that checks connection
					//and reconnects when not, when no map is loaded connection is not tested or reconnected.

					//Also the code is ran here first tick. s_bInGameWindow should be false before this panel is ticked at all.
					
					//so this is really also the first initial connect unless a background map is loaded and inits the game input.
					cliHaptics->Init_Haptics();
				}
				else
					SetVGUICursorPos(mx,my);
			}else{
				if(s_bInGameWindow)
				{
					//Pat: 8/23/07 removing shutdown to haptics on minimize.
					//cliHaptics->Shutdown_Haptics();
					s_bInGameWindow = false;
				}

			}
		}
	}
}

void CFalconPanel::OnCancel()
{
	for(int i = 0;i!=HapPages.Count();i++)
	{
		HapPages[i]->OnCommand("cancel");
	};
	BaseClass::OnCancel();
}

bool CFalconPanel::OnOK(bool applyOnly)
{
	for(int i = 0;i!=HapPages.Count();i++)
	{
		HapPages[i]->OnCommand("ok");
	};
	bool bRet =  BaseClass::OnOK(applyOnly);
	return bRet;
}

void CFalconPanel::OnDefault()// only applies to current tab.
{
	GetPropertySheet()->GetActivePage()->OnCommand("default");
	//SetApplyButtonVisible(true);

};
/*---------------------------------------------------------------------------*
/ Type: void
/ Description: Every time the user opens the options menu, go to the first tab.
\*---------------------------------------------------------------------------*/
void CFalconPanel::SetToDefaultPage()
{
	GetPropertySheet()->SetActivePage(HapPages[0]);
	// then issue reads to all panels.
	for(int i = 0;i!= HapPages.Size(); i++)
	{
		HapPages[i]->OnCommand("cancel");
	}

}
