#include "CBase.h"
#include "HapticPages.h"
#include <KeyValues.h>
#include "tier0/memdbgon.h"

static const char *WeaponNameFixer(const char *token, char *out, int size)
{
	//heres some special stuff to use more common names that are inside ()'s
	// (sort of hacky)
	vgui::Label *temp = new vgui::Label(NULL,"TEMP",token);
	temp->GetText(out,size);
	CUtlVector<char *>szCont;
	V_SplitString(out, "(", szCont);
	if(szCont.Count()>1)
	{
		V_StrSubst(szCont[1],")","",out,size);
	}
	temp->DeletePanel();

	return (const char *)out;
}

CHapticsForcesPage::CHapticsForcesPage(vgui::PropertySheet *parent, KeyValues *pageData) :
BaseClass(parent,NULL)
{
	parent->AddPage(this,GetTabTitle());

	CVarSlider *ForceScale = new CVarSlider(this, "FORCESCALE", "#HapForceScale", "hap_recoil_scale", 0.3, 0, 2, 0, 40, 40, 0);

	CVarSliderCombo *WeaponScale = new CVarSliderCombo(this, "WEAPONFORCESCALE", "#HapWeaponRecoil", "hap_weapon_scale_357", "#HL2_357Handgun", 2.5, 0, 4, 0, 40, 40, 0);

	// used for weapon name fixing.
	char temp[80];
	WeaponScale->AddSlider("hap_weapon_scale_ar2", WeaponNameFixer("#HL2_Pulse_Rifle",temp,sizeof(temp)), 0.9);
	WeaponScale->AddSlider("hap_weapon_scale_stunstick", "STUNSTICK", 1.0);
	WeaponScale->AddSlider("hap_weapon_scale_crossbow", "#HL2_Crossbow", 1.0);
	WeaponScale->AddSlider("hap_weapon_scale_crowbar", "#HL2_Crowbar", 1.0);
	WeaponScale->AddSlider("hap_weapon_scale_physcannon", WeaponNameFixer("#HL2_GravityGun",temp,sizeof(temp)), 0.8);
	WeaponScale->AddSlider("hap_weapon_scale_pistol", "#HL2_Pistol", 0.5);
	WeaponScale->AddSlider("hap_weapon_scale_shotgun", "#HL2_Shotgun", 2.5);
	WeaponScale->AddSlider("hap_weapon_scale_machinegun", WeaponNameFixer("#HL2_SMG1",temp,sizeof(temp)), 0.5);

	CVarSlider *DamageForces = new CVarSlider(this, "DAMAGEFORCES", "#HapDamageForces", "hap_damage_scale", 0.5, 0, 4, 0, 40, 40, 0);

	CVarSliderCombo *CarryScale = new CVarSliderCombo(this, "CARRYSCALE", "#HapObjectCarryWeight", "hap_sv_carry_player_scale", "#HapPlayer", 2.0, 0, 4, 0, 40, 40, 0);
	CarryScale->AddSlider("hap_sv_carry_physcannon_scale", WeaponNameFixer("#HL2_GravityGun",temp,sizeof(temp)), 0.5);

	CVarSlider *VehicleForces = new CVarSlider(this, "VEHICLEFORCES", "#HapVehicleForces", "hap_vehicle_force_scale", 1.0, 0, 4, 0, 40, 40, 0);
	
	//CVarSliderCombo *ControlBoxStiffness = new CVarSliderCombo(this, "CONTOLLSTIFF", "#HapControlStiffness", "hap_cb_springk", "#HapPlayer", 300, 0, 300, 0, 30, 30, 0);
	//ControlBoxStiffness->AddSlider("hap_cbvehicle_springk", "#HapVehicle", 250);
	//CVarSlider *AvatarForces  = new CVarSlider(this, "AVATARFORCES", "#HapAvatarForces", "hap_avatar_scale", 1.0, 0.0, 5.0, 0, 40, 40, 0);
	AddCVarControl(ForceScale);
	AddCVarControl(WeaponScale);
	AddCVarControl(DamageForces);
	AddCVarControl(CarryScale);
	AddCVarControl(VehicleForces);
	//AddCVarControl(AvatarForces);
	//AddCVarControl(ControlBoxStiffness);

	LoadControlSettings("resource/UI/FalconForces.res");

	//resize everything
	for(int i = 0;i!=m_controls.Count();i++)
	{
		// currently, this page will only use slider CVControls.
		((CVarSlider *)m_controls[i])->SetSize(m_controls[i]->GetWide(), m_controls[i]->GetTall());
	}
	SetVisible(false);
};

const char *CHapticsForcesPage::GetTabTitle()
{
	return "#HapForces";
}