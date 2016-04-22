#include "CBase.h"
#include "HapticPages.h"
#include <vgui_controls/CheckButton.h>
#include <KeyValues.h>
#include "tier0/memdbgon.h"



CHapticsControlBoxPage::CHapticsControlBoxPage(vgui::PropertySheet *parent, KeyValues *pageData) :
BaseClass(parent,pageData)
{
	parent->AddPage(this,GetTabTitle());

	//// okay making sub tabs for each c-box ( vehicle and player )
	m_pSheet = new vgui::PropertySheet(this,"CBOXTABS");
	m_pSheet->SetWide(474);
	m_pSheet->SetTall(200);

	vgui::EditablePanel *pPropertyPlayer  = new vgui::EditablePanel(this, "PLAYERCBOX");
	vgui::EditablePanel *pPropertyVehicle = new vgui::EditablePanel(this, "VEHICLECBOX");

	m_pSheet->AddPage(pPropertyPlayer, "#HapPlayerTab");
	m_pSheet->AddPage(pPropertyVehicle, "#HapVehicleTab");
	
	
	// match names!
	CVarSlider *nearP  = new CVarSlider(pPropertyPlayer , "NEARSLIDER",  "#HapNear" , "hap_cb_near_start"  , 0.02,  0, 0.1, 0, 40, 40, 0);
	CVarSlider *farP   = new CVarSlider(pPropertyPlayer , "FARSLIDER",   "#HapFar"  , "hap_cb_far_start"   , 0.06,  0, 0.1, 0, 40, 40, 0, 0, -1);//negative
	CVarSlider *leftP  = new CVarSlider(pPropertyPlayer , "LEFTSLIDER",  "#HapLeft" , "hap_cb_left_start"  , 0.035, 0, 0.1, 0, 40, 40, 0, 0, -1);//negative
	CVarSlider *rightP = new CVarSlider(pPropertyPlayer , "RIGHTSLIDER", "#HapRight", "hap_cb_right_start" , 0.035, 0, 0.1, 0, 40, 40, 0);
	CVarSlider *upP    = new CVarSlider(pPropertyPlayer , "UPSLIDER",    "#HapUp"   , "hap_cb_top_start"   , 0.035, 0, 0.1, 0, 40, 40, 0);
	CVarSlider *downP  = new CVarSlider(pPropertyPlayer , "DOWNSLIDER",  "#HapDown" , "hap_cb_bottom_start", 0.035, 0, 0.1, 0, 40, 40, 0, 0, -1);//negative 
	CVarSlider *powerP = new CVarSlider(pPropertyPlayer , "POWERSLIDER", "#HapControlStiffness", "hap_cb_springk", 300,	0, 400, 0, 40, 40, 0);

	CVarSlider *nearV  = new CVarSlider(pPropertyVehicle, "NEARSLIDER",  "#HapNear" , "hap_cbvehicle_near_start"  , 0.035, 0, 0.1, 0, 40, 40, 0);
	CVarSlider *farV   = new CVarSlider(pPropertyVehicle, "FARSLIDER",   "#HapFar"  , "hap_cbvehicle_far_start"   , 0.035, 0, 0.1, 0, 40, 40, 0, 0, -1);//negative
	CVarSlider *leftV  = new CVarSlider(pPropertyVehicle, "LEFTSLIDER",  "#HapLeft" , "hap_cbvehicle_left_start"  , 0.003, 0, 0.1, 0, 40, 40, 0, 0, -1);//negative
	CVarSlider *rightV = new CVarSlider(pPropertyVehicle, "RIGHTSLIDER", "#HapRight", "hap_cbvehicle_right_start" , 0.003, 0, 0.1, 0, 40, 40, 0);
	CVarSlider *upV    = new CVarSlider(pPropertyVehicle, "UPSLIDER",    "#HapUp"   , "hap_cbvehicle_top_start"   , 0.1,   0, 0.1, 0, 40, 40, 0);
	CVarSlider *downV  = new CVarSlider(pPropertyVehicle, "DOWNSLIDER",  "#HapDown" , "hap_cbvehicle_bottom_start", 0.1,   0, 0.1, 0, 40, 40, 0, 0, -1);//negative 
	CVarSlider *powerV = new CVarSlider(pPropertyVehicle, "POWERSLIDER", "#HapControlStiffness", "hap_cbvehicle_springk", 250,	0, 400, 0, 40, 40, 0);
	
	new ControlBoxVisual(pPropertyPlayer, nearP, rightP, upP, farP, leftP, downP);
	new ControlBoxVisual(pPropertyVehicle, nearV, rightV, upV, farV, leftV, downV);
	

	//register all our sliders for saving and restoring.
	AddCVarControl( (vgui::Panel*) nearP);
	AddCVarControl( (vgui::Panel*) farP);
	AddCVarControl( (vgui::Panel*) leftP);
	AddCVarControl( (vgui::Panel*) rightP);
	AddCVarControl( (vgui::Panel*) upP);
	AddCVarControl( (vgui::Panel*) downP);

	AddCVarControl( (vgui::Panel*) nearV);
	AddCVarControl( (vgui::Panel*) farV);
	AddCVarControl( (vgui::Panel*) leftV);
	AddCVarControl( (vgui::Panel*) rightV);
	AddCVarControl( (vgui::Panel*) upV);
	AddCVarControl( (vgui::Panel*) downV);
	// now resize the label each of these currently added controls.
	for(int i = 0;i!=m_controls.Count();i++)
	{
		// currently, this page will only use slider CVControls.
		((CVarSlider *)m_controls[i])->SetLabelWidth(53);
	}

	AddCVarControl( (vgui::Panel*) powerP);
	AddCVarControl( (vgui::Panel*) powerV);

	pPropertyPlayer->LoadControlSettings("resource/UI/FalconControlBoxTab.res");
	pPropertyVehicle->LoadControlSettings("resource/UI/FalconControlBoxTab.res");
	//((vgui::Panel *)pPropertyVehicle)->SetVisible(false);
	parent->SetActivePage(pPropertyVehicle);
	LoadControlSettings("resource/UI/FalconControlBox.res");

	//resize everything
	for(int i = 0;i!=m_controls.Count();i++)
	{
		// currently, this page will only use slider CVControls.
		((CVarSlider *)m_controls[i])->SetSize(m_controls[i]->GetWide(), m_controls[i]->GetTall());
	}
	SetVisible(false);
	m_pSheet->SetActivePage(pPropertyPlayer);pPropertyVehicle->SetVisible(false);pPropertyPlayer->SetVisible(true);
};

const char *CHapticsControlBoxPage::GetTabTitle()
{
	return "#HapControlBox";
}