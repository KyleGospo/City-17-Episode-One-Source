#include "CBase.h"
#include "HapticPages.h"
#include <vgui_controls/CheckButton.h>
#include <KeyValues.h>
#include "tier0/memdbgon.h"
CHapticsMovementPage::CHapticsMovementPage(vgui::PropertySheet *parent,KeyValues *pageData) :
BaseClass(parent,pageData)
{
	parent->AddPage(this,GetTabTitle());
	//CVarSliderCombo *ControlBox = new CVarSliderCombo(this, "CBoxSize", "#HapControlBoxSize", "hap_cb_left_start", "#HapPlayer", 300, 0, 300, 0, 12, 12, 0);
	//ControlBox->AddConvarMimic("hap_cb_bottom_start");
	//ControlBox->AddConvarMimic("hap_cb_far_start");
	//ControlBox->AddConvarMimic("hap_cb_right_start");
	//ControlBox->AddConvarMimic("hap_cb_top_start");
	//ControlBox->AddConvarMimic("hap_cb_near_start");

	
	
	//CVarSlider *VehicleBox = ControlBox->AddSlider("hap_cb_left_start", "#HapVehicle", 200);
	//VehicleBox->AddConvarMimic("hap_cbvehicle_bottom_start");
	//VehicleBox->AddConvarMimic("hap_cbvehicle_far_start");
	//VehicleBox->AddConvarMimic("hap_cbvehicle_right_start");
	//VehicleBox->AddConvarMimic("hap_cbvehicle_top_start");
	//VehicleBox->AddConvarMimic("hap_cbvehicle_near_start");	

	CVarSlider *pAimYaw    = new CVarSlider(this, "YAWAIM", NULL, "hap_yawsensitivity_posctrl", 120, 0, 360, 0, 360, 36, 0);
	CVarSlider *pAimPitch  = new CVarSlider(this, "PITAIM", NULL, "hap_pitchsensitivity_posctrl", 120, 0, 180, 0, 180, 18, 0);  

	CVarSlider *pTurnYaw   = new CVarSlider(this, "YAWTURN", NULL, "hap_yawsensitivity_velctrl", 2, 0, 4, 0, 16, 16, 0);
	CVarSlider *pTurnPitch = new CVarSlider(this, "PITTURN", NULL, "hap_pitchsensitivity_velctrl", 1, 0, 4, 0, 16, 16, 0);  
	
	CVarSliderCheck *pMove = new CVarSliderCheck(this, "MOVE", "#HapMovementEnabled", "hap_var_avatar_movement_mode", "hap_forwardsensitivity",2,1,0,10,1,2,0,10,10,0); 
	pMove->SetAutoSizeLabels(true);
	
	CVarSlider *pCursorSensitivity = new CVarSlider(this, "CURSORSENS", NULL, "hap_cursor_sensitivity", 2, 0, 10, 0, 10, 10, 0);

	//AddCVarControl( (vgui::Panel*) ControlBox );
	AddCVarControl( (vgui::Panel*) pAimYaw );
	AddCVarControl( (vgui::Panel*) pAimPitch );
	AddCVarControl( (vgui::Panel*) pTurnYaw );
	AddCVarControl( (vgui::Panel*) pTurnPitch );
	AddCVarControl( (vgui::Panel*) pMove );
	AddCVarControl( (vgui::Panel*) pCursorSensitivity );

	// check marks for reversals
	m_pReverseYaw = new vgui::CheckButton( this, "YAWREVERSE", "#HapReverse");
	m_pReversePitch = new vgui::CheckButton( this, "PITCHREVERSE", "#HapReverseUp");

	LoadControlSettings("resource/UI/FalconMovement.res");
	//resize everything
	for(int i = 0;i!=m_controls.Count();i++)
	{
		// currently, this page will only use slider CVControls.
		((CVarSlider *)m_controls[i])->SetSize(m_controls[i]->GetWide(), m_controls[i]->GetTall());
	}
	//SetVisible(false);first page.
}
void CHapticsMovementPage::InitControls()
{
}
const char *CHapticsMovementPage::GetTabTitle()
{
	return "#HapMovement";
}
void CHapticsMovementPage::SaveVars()
{
	BaseClass::SaveVars();
	// now check our checkboxes
	if(m_pReverseYaw->IsSelected())
	{
		// if we are selected keep the number selected as positive for us is reversed.
	}else{
		// negate our yaw based convars
		ConVar *pos = cvar->FindVar("hap_yawsensitivity_posctrl");
		ConVar *vel = cvar->FindVar("hap_yawsensitivity_velctrl");
		pos->SetValue(pos->GetFloat()*-1.0f);
		vel->SetValue(vel->GetFloat()*-1.0f);
	}
	if(m_pReversePitch->IsSelected())
	{
		// if we are selected keep the number selected as positive for us is reversed.
	}else{
		// negate our pitch based convars
		ConVar *pos = cvar->FindVar("hap_pitchsensitivity_posctrl");
		ConVar *vel = cvar->FindVar("hap_pitchsensitivity_velctrl");
		pos->SetValue(pos->GetFloat()*-1.0f);
		vel->SetValue(vel->GetFloat()*-1.0f);
	}
}
void CHapticsMovementPage::LoadDefaultVars()
{
	BaseClass::LoadDefaultVars();
	OnChangedCvars();
}
void CHapticsMovementPage::OnChangedCvars()
{
	const ConVar *yaw = cvar->FindVar("hap_yawsensitivity_velctrl");
	const ConVar *pitch = cvar->FindVar("hap_pitchsensitivity_velctrl");
	m_pReverseYaw->SetSelected(yaw->GetFloat()>0);
	m_pReversePitch->SetSelected(pitch->GetFloat()>0);
}
void CHapticsMovementPage::ResetVars()
{
	BaseClass::ResetVars();
	OnChangedCvars();
}