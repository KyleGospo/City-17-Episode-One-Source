//The following include files are necessary to allow your HapticsPanel.cpp to compile.
#include "cbase.h"
#include "haptics_panel.h"

using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/PropertyDialog.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/ComboBox.h>

//CHapticsPanel class: Tutorial example class
class CHapticsPanel : public vgui::PropertyDialog
{
	DECLARE_CLASS_SIMPLE(CHapticsPanel, vgui::PropertyDialog); 
	//CHapticsPanel : This Class / vgui::Frame : BaseClass

	CHapticsPanel(vgui::VPANEL parent); 	// Constructor
	~CHapticsPanel(){};				// Destructor

protected:
	//VGUI overrides:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);

private:
	//Other used VGUI control Elements:
	vgui::TextEntry* m_pXPos; // Panel class declaration, private section
	vgui::TextEntry* m_pYPos; // Panel class declaration, private section
	vgui::TextEntry* m_pZPos; // Panel class declaration, private section

	// Movement Model UI components
	vgui::Panel* m_pMovementModel;

	vgui::Label*	m_lInner;
	vgui::Label*	m_lInnerType;
	vgui::ComboBox*	m_cInnerType;
	vgui::Label*	m_lInnerExtents;
	vgui::Label*	m_lInnerXMin;
	vgui::Label*	m_lInnerYMin;
	vgui::Label*	m_lInnerZMin;
	vgui::Label*	m_lInnerXMax;
	vgui::Label*	m_lInnerYMax;
	vgui::Label*	m_lInnerZMax;
	vgui::Slider*	m_sInnerXMin;
	vgui::Slider*	m_sInnerYMin;
	vgui::Slider*	m_sInnerZMin;
	vgui::Slider*	m_sInnerXMax;
	vgui::Slider*	m_sInnerYMax;
	vgui::Slider*	m_sInnerZMax;

	vgui::Label*	m_lOuter;
	vgui::Label*	m_lOuterType;
	vgui::ComboBox*	m_cOuterType;
	vgui::Label*	m_lOuterExtents;
	vgui::Label*	m_lOuterXMin;
	vgui::Label*	m_lOuterYMin;
	vgui::Label*	m_lOuterZMin;
	vgui::Label*	m_lOuterXMax;
	vgui::Label*	m_lOuterYMax;
	vgui::Label*	m_lOuterZMax;
	vgui::Slider*	m_sOuterXMin;
	vgui::Slider*	m_sOuterYMin;
	vgui::Slider*	m_sOuterZMin;
	vgui::Slider*	m_sOuterXMax;
	vgui::Slider*	m_sOuterYMax;
	vgui::Slider*	m_sOuterZMax;

	vgui::Label*	m_lSensitivity;
	vgui::Label*	m_lForward;
	vgui::Label*	m_lSide;
	vgui::Label*	m_lPitch;
	vgui::Label*	m_lYaw;
	vgui::Slider*	m_sForward;
	vgui::Slider*	m_sSide;
	vgui::Slider*	m_sPitch;
	vgui::Slider*	m_sYaw;

	vgui::Label*	m_lAxis;
	vgui::ComboBox*	m_cAxis;

	// Force Effects UI components
	vgui::Panel* m_pForceEffects;

	vgui::Label*	m_lDamage;
	vgui::Label*	m_lRecoil;
	vgui::Label*	m_lSteps;
	vgui::Label*	m_lCarry;
	vgui::Label*	m_lVehicle;
	vgui::Label*	m_lAvatar;

	vgui::CheckButton*	m_bDamage;
	vgui::CheckButton*	m_bRecoil;
	vgui::CheckButton*	m_bSteps;
	vgui::CheckButton*	m_bCarry;
	vgui::CheckButton*	m_bVehicle;
	vgui::CheckButton*	m_bAvatar;

	vgui::ComboBox*	m_cWeapon;
	vgui::ComboBox* m_cVehicle;

	vgui::Slider*	m_sDamage;
	vgui::Slider*	m_sRecoil;
	vgui::Slider*	m_sSteps;
	vgui::Slider*	m_sCarry;

	vgui::Slider*	m_sVehicleX;
	vgui::Slider*	m_sVehicleY;
	vgui::Slider*	m_sVehicleZ;
	vgui::Slider*	m_sAvatarX;
	vgui::Slider*	m_sAvatarY;
	vgui::Slider*	m_sAvatarZ;

	// Advanced UI components
	vgui::Panel*	m_pAdvanced;

	vgui::Label*	m_lFeedback;
	vgui::Label*	m_lFeedbackDamage;
	vgui::Label*	m_lFeedbackRecoil;
	vgui::Label*	m_lFeedbackSteps;
	vgui::Label*	m_lFeedbackCarry;
	vgui::Label*	m_lFeedbackVehicle;
	vgui::Label*	m_lFeedbackAvatar;

	vgui::CheckButton*	m_bFeedbackDamage;
	vgui::CheckButton*	m_bFeedbackRecoil;
	vgui::CheckButton*	m_bFeedbackSteps;
	vgui::CheckButton*	m_bFeedbackCarry;
	vgui::CheckButton*	m_bFeedbackVehicle;
	vgui::CheckButton*	m_bFeedbackAvatar;

	vgui::Label*	m_lDeviceExtents;
	vgui::Label*	m_lDeviceXMin;
	vgui::Label*	m_lDeviceYMin;
	vgui::Label*	m_lDeviceZMin;
	vgui::Label*	m_lDeviceXMax;
	vgui::Label*	m_lDeviceYMax;
	vgui::Label*	m_lDeviceZMax;
	vgui::Slider*	m_sDeviceXMin;
	vgui::Slider*	m_sDeviceYMin;
	vgui::Slider*	m_sDeviceZMin;
	vgui::Slider*	m_sDeviceXMax;
	vgui::Slider*	m_sDeviceYMax;
	vgui::Slider*	m_sDeviceZMax;

	vgui::Label*	m_lCoordinateType;
	vgui::ComboBox*	m_cCoordinateType;
};

// Constuctor: Initializes the Frame
CHapticsPanel::CHapticsPanel(vgui::VPANEL parent)
: BaseClass(NULL, "HapticsFrame")
{
	SetParent( parent );
	
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
	
	SetTitle("HAPTICS OPTIONS", false);
	
	SetPos(160, 140);
	SetSize(480, 480);

	SetProportional( false );
	SetTitleBarVisible( true );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( true );
	SetSizeable( true );
	SetMoveable( true );
	SetVisible( false );

	SetOKButtonVisible(true);
	SetCancelButtonVisible(true);
	SetApplyButtonVisible(true);

	m_pMovementModel = new vgui::Panel(this);
	m_pForceEffects = new vgui::Panel(this);
	m_pAdvanced = new vgui::Panel(this);

	GetPropertySheet()->AddPage(m_pMovementModel, "Movement Model");
	GetPropertySheet()->AddPage(m_pForceEffects, "Force Effects");
	GetPropertySheet()->AddPage(m_pAdvanced, "Advanced");
	GetPropertySheet()->SetTabWidth(40);

	// Initializing Movement Model property page

	m_lInner = new vgui::Label(m_pMovementModel, "LabelInner", "Inner");					m_lInner->SetPos(20, 40);
	m_lInnerType = new vgui::Label(m_pMovementModel, "LabelInnerType", "Type");				m_lInnerType->SetPos(20, 60);
	m_lInnerExtents = new vgui::Label(m_pMovementModel, "LabelInnerExtents", "Extents");	m_lInnerExtents->SetPos(20, 80);

	m_lInnerXMin = new vgui::Label(m_pMovementModel, "LabelInnerXMin", "X (min) = ");		m_lInnerXMin->SetPos(20, 100);
	m_lInnerXMax = new vgui::Label(m_pMovementModel, "LabelInnerXMax", "X (max) = ");		m_lInnerXMax->SetPos(20, 120);
	m_lInnerYMin = new vgui::Label(m_pMovementModel, "LabelInnerYMin", "Y (min) = ");		m_lInnerYMin->SetPos(20, 140);
	m_lInnerYMax = new vgui::Label(m_pMovementModel, "LabelInnerYMax", "Y (max) = ");		m_lInnerYMax->SetPos(20, 160);
	m_lInnerZMin = new vgui::Label(m_pMovementModel, "LabelInnerZMin", "Z (min) = ");		m_lInnerZMin->SetPos(20, 180);
	m_lInnerZMax = new vgui::Label(m_pMovementModel, "LabelInnerZMax", "Z (max) = ");		m_lInnerZMax->SetPos(20, 200);

	m_sInnerXMin = new vgui::Slider(m_pMovementModel, "SliderInnerXMin");					m_sInnerXMin->SetPos(100, 100);
	m_sInnerXMax = new vgui::Slider(m_pMovementModel, "SliderInnerXMax");					m_sInnerXMax->SetPos(100, 120);
	m_sInnerYMin = new vgui::Slider(m_pMovementModel, "SliderInnerYMin");					m_sInnerYMin->SetPos(100, 140);
	m_sInnerYMax = new vgui::Slider(m_pMovementModel, "SliderInnerYMax");					m_sInnerYMax->SetPos(100, 160);
	m_sInnerZMin = new vgui::Slider(m_pMovementModel, "SliderInnerZMin");					m_sInnerZMin->SetPos(100, 180);
	m_sInnerZMax = new vgui::Slider(m_pMovementModel, "SliderInnerZMax");					m_sInnerZMax->SetPos(100, 200);

	m_lOuter = new vgui::Label(m_pMovementModel, "LabelOuter", "Outer");					m_lOuter->SetPos(220, 40);
	m_lOuterType = new vgui::Label(m_pMovementModel, "LabelOuterType", "Type");				m_lOuterType->SetPos(220, 60);
	m_lOuterExtents = new vgui::Label(m_pMovementModel, "LabelOuterExtents", "Extents");	m_lOuterExtents->SetPos(220, 80);

	m_lOuterXMin = new vgui::Label(m_pMovementModel, "LabelOuterXMin", "X (min) = ");		m_lOuterXMin->SetPos(220, 100);
	m_lOuterXMax = new vgui::Label(m_pMovementModel, "LabelOuterXMax", "X (max) = ");		m_lOuterXMax->SetPos(220, 120);
	m_lOuterYMin = new vgui::Label(m_pMovementModel, "LabelOuterYMin", "Y (min) = ");		m_lOuterYMin->SetPos(220, 140);
	m_lOuterYMax = new vgui::Label(m_pMovementModel, "LabelOuterYMax", "Y (max) = ");		m_lOuterYMax->SetPos(220, 160);
	m_lOuterZMin = new vgui::Label(m_pMovementModel, "LabelOuterZMin", "Z (min) = ");		m_lOuterZMin->SetPos(220, 180);
	m_lOuterZMax = new vgui::Label(m_pMovementModel, "LabelOuterZMax", "Z (max) = ");		m_lOuterZMax->SetPos(220, 200);

	m_sOuterXMin = new vgui::Slider(m_pMovementModel, "SliderOuterXMin");					m_sOuterXMin->SetPos(320, 100);
	m_sOuterXMax = new vgui::Slider(m_pMovementModel, "SliderOuterXMax");					m_sOuterXMax->SetPos(320, 120);
	m_sOuterYMin = new vgui::Slider(m_pMovementModel, "SliderOuterYMin");					m_sOuterYMin->SetPos(320, 140);
	m_sOuterYMax = new vgui::Slider(m_pMovementModel, "SliderOuterYMax");					m_sOuterYMax->SetPos(320, 160);
	m_sOuterZMin = new vgui::Slider(m_pMovementModel, "SliderOuterZMin");					m_sOuterZMin->SetPos(320, 180);
	m_sOuterZMax = new vgui::Slider(m_pMovementModel, "SliderOuterZMax");					m_sOuterZMax->SetPos(320, 200);

	m_lSensitivity = new vgui::Label(m_pMovementModel, "LabelSensitivity", "Sensitivity");	m_lSensitivity->SetPos(20, 220);

	m_lForward = new vgui::Label(m_pMovementModel, "LabelForward", "Forward");				m_lForward->SetPos(20, 240);
	m_lSide = new vgui::Label(m_pMovementModel, "LabelSide", "Side");						m_lSide->SetPos(20, 260);
	m_lPitch = new vgui::Label(m_pMovementModel, "LabelPitch", "Pitch");					m_lPitch->SetPos(20, 280);
	m_lYaw = new vgui::Label(m_pMovementModel, "LabelYaw", "Yaw");							m_lYaw->SetPos(20, 300);

	m_sForward = new vgui::Slider(m_pMovementModel, "SliderForward");						m_sForward->SetPos(100, 240);
	m_sForward->SetRange(-10, 10);
	m_sSide = new vgui::Slider(m_pMovementModel, "SliderSide");								m_sSide->SetPos(100, 260);
	m_sPitch = new vgui::Slider(m_pMovementModel, "SliderPitch");							m_sPitch->SetPos(100, 280);
	m_sYaw = new vgui::Slider(m_pMovementModel, "SliderYaw");								m_sYaw->SetPos(100, 300);

	m_lAxis = new vgui::Label(m_pMovementModel, "LabelAxis", "Axis");						m_lAxis->SetPos(220, 240);
	m_cAxis = new vgui::ComboBox(m_pMovementModel,"ComboAxis", 2, false);					m_cAxis->SetPos(220, 260);

	// Force effects property sheet
	m_lDamage = new vgui::Label(m_pForceEffects, "LabelDamage", "Damage strength");		m_lDamage->SetPos(20, 20);
	m_lSteps = new vgui::Label(m_pForceEffects, "LabelSteps", "Footstep scale");		m_lSteps->SetPos(20, 40);
	m_lVehicle = new vgui::Label(m_pForceEffects, "LabelVehicle", "Vehicle Physics");	m_lVehicle->SetPos(20, 60);
	m_lCarry = new vgui::Label(m_pForceEffects, "LabelCarry", "Weight lifting scale");	m_lCarry->SetPos(80, 20);
	m_lRecoil = new vgui::Label(m_pForceEffects, "LabelRecoil", "Recoil strength");		m_lRecoil->SetPos(80, 40);
	m_lAvatar = new vgui::Label(m_pForceEffects, "LabelAvatar", "Avatar Physics");		m_lAvatar->SetPos(80, 60);

	m_bDamage = new vgui::CheckButton(m_pForceEffects, "CheckButtonDamage", "Damage strength");		m_bDamage->SetPos(10, 20);
	m_bSteps = new vgui::CheckButton(m_pForceEffects, "CheckButtonSteps", "Footstep scale");		m_bSteps->SetPos(10, 40);
	m_bVehicle = new vgui::CheckButton(m_pForceEffects, "CheckButtonVehicle", "Vehicle Physics");	m_bVehicle->SetPos(10, 60);
	m_bCarry = new vgui::CheckButton(m_pForceEffects, "CheckButtonCarry", "Weight lifting scale");	m_bCarry->SetPos(70, 20);
	m_bRecoil = new vgui::CheckButton(m_pForceEffects, "CheckButtonRecoil", "Recoil strength");		m_bRecoil->SetPos(70, 40);
	m_bAvatar = new vgui::CheckButton(m_pForceEffects, "CheckButtonAvatar", "Avatar Physics");		m_bAvatar->SetPos(70, 60);

	m_sDamage = new vgui::Slider(m_pForceEffects, "SliderDamage");	m_sDamage->SetPos(30, 20);
	m_sSteps = new vgui::Slider(m_pForceEffects, "SliderSteps");	m_sSteps->SetPos(30, 40);
	m_sCarry = new vgui::Slider(m_pForceEffects, "SliderCarry");	m_sCarry->SetPos(90, 20);
	m_sRecoil = new vgui::Slider(m_pForceEffects, "SliderRecoil");	m_sRecoil->SetPos(90, 40);

	m_sVehicleX = new vgui::Slider(m_pForceEffects, "SliderVehicleX");	m_sVehicleX->SetPos(30, 60);
	m_sVehicleY = new vgui::Slider(m_pForceEffects, "SliderVehicleY");	m_sVehicleY->SetPos(30, 70);
	m_sVehicleZ = new vgui::Slider(m_pForceEffects, "SliderVehicleZ");	m_sVehicleZ->SetPos(30, 80);

	m_sAvatarX = new vgui::Slider(m_pForceEffects, "SliderAvatarX");	m_sAvatarX->SetPos(30, 60);
	m_sAvatarY = new vgui::Slider(m_pForceEffects, "SliderAvatarY");	m_sAvatarY->SetPos(30, 70);
	m_sAvatarZ = new vgui::Slider(m_pForceEffects, "SliderAvatarZ");	m_sAvatarZ->SetPos(30, 80);

	m_cWeapon = new vgui::ComboBox(m_pForceEffects, "ComboWeapon", 9, false);
	m_cVehicle = new vgui::ComboBox(m_pForceEffects, "ComboVehicle", 2, false);

	/*
	m_pXPos = new vgui::TextEntry(m_pChild1, "HapticsXPos");
	m_pXPos->SetPos(40, 50);
	m_pXPos->SetSize(86, 30); // 26, 10
	//m_pXPos->SetAutoResize(PIN_TOPLEFT, AUTORESIZE_DOWNANDRIGHT, 
	m_pYPos = new vgui::TextEntry(m_pChild1, "HapticsYPos");
	m_pYPos->SetPos(40, 80);
	m_pYPos->SetSize(86, 30);
	m_pZPos = new vgui::TextEntry(m_pChild1, "HapticsZPos");
	m_pZPos->SetPos(40, 110);
	m_pZPos->SetSize(86, 30);

	m_pXPos->SetEnabled(true);
	m_pYPos->SetEnabled(true);
	m_pZPos->SetEnabled(true);
	*/

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	LoadControlSettings("resource/UI/HapticsPanel.res");

	vgui::ivgui()->AddTickSignal( GetVPanel(), 100 );

	DevMsg("HapticsPanel has been constructed\n");
}

ConVar cl_showhapticspanel("cl_showhapticspanel", "1", FCVAR_CLIENTDLL, "Sets the state of hapticsPanel <state>");

extern ConVar hap_panel_xpos;
extern ConVar hap_panel_ypos;
extern ConVar hap_panel_zpos;

void CHapticsPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_showhapticspanel.GetBool()); //CL_SHOWHAPTICSPANEL / 1 BY DEFAULT

	char buf[20];

	/*
	sprintf(buf, "%+0.4f", hap_panel_xpos.GetFloat());
	m_pXPos->SetText(buf);
	sprintf(buf, "%+0.4f", hap_panel_ypos.GetFloat());
	m_pYPos->SetText(buf);
	sprintf(buf, "%+0.4f", hap_panel_zpos.GetFloat());
	m_pZPos->SetText(buf);
	*/
}

CON_COMMAND(ToggleHapticsPanel, "Toggles hapticsPanel on or off")
{
	cl_showhapticspanel.SetValue(!cl_showhapticspanel.GetBool());
};

void CHapticsPanel::OnCommand(const char* pcCommand)
{
	if(!Q_stricmp(pcCommand, "turnoff"))
		cl_showhapticspanel.SetValue(0);
}

//Class: CHapticsPanelInterface Class. Used for construction.
class CHapticsPanelInterface : public IHapticsPanel
{
private:
	CHapticsPanel *HapticsPanel;
public:
	CHapticsPanelInterface()
	{
		HapticsPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		HapticsPanel = new CHapticsPanel(parent);
	}
	void Destroy()
	{
		if (HapticsPanel)
		{
			HapticsPanel->SetParent( (vgui::Panel *)NULL);
			delete HapticsPanel;
		}
	}
};

static CHapticsPanelInterface g_HapticsPanel;
IHapticsPanel* hapticspanel = (IHapticsPanel*)&g_HapticsPanel;