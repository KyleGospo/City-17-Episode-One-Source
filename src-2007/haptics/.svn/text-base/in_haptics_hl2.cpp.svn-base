#include "cbase.h"
#include "in_haptics_hl2.h"
#include "ThresholdInfo.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Airboat threshold data.
ConVar hap_vehicle_airboat_scale_x("hap_vehicle_airboat_scale_x", "0.18", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_scale_y("hap_vehicle_airboat_scale_y", "0.18", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_scale_z("hap_vehicle_airboat_scale_z", "0.18", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_force_threshold_lower("hap_vehicle_airboat_force_threshold_lower", "5.0", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_force_threshold_upper("hap_vehicle_airboat_force_threshold_upper", "12.0", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_scale_low("hap_vehicle_airboat_scale_low", "1.6", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_scale_medium("hap_vehicle_airboat_scale_medium", "1.2", FCVAR_ARCHIVE);
ConVar hap_vehicle_airboat_scale_high("hap_vehicle_airboat_scale_high", "0.80", FCVAR_ARCHIVE);
// Airboat Threshold Helper 
static const ThresholdInfo s_ThresholdAirboat(	&hap_vehicle_airboat_force_threshold_lower, &hap_vehicle_airboat_scale_low,
												&hap_vehicle_airboat_scale_medium,
												&hap_vehicle_airboat_force_threshold_upper, &hap_vehicle_airboat_scale_high,
												&hap_vehicle_airboat_scale_x,
												&hap_vehicle_airboat_scale_y,
												&hap_vehicle_airboat_scale_z);

// Jeep threshold data.
ConVar hap_vehicle_jeep_scale_x("hap_vehicle_jeep_scale_x", "0.1", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_scale_y("hap_vehicle_jeep_scale_y", "0.1", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_scale_z("hap_vehicle_jeep_scale_z", "0.1", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_force_threshold_lower("hap_vehicle_jeep_force_threshold_lower", "3.0", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_force_threshold_upper("hap_vehicle_jeep_force_threshold_upper", "12.0", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_scale_low("hap_vehicle_jeep_scale_low", "2.0", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_scale_medium("hap_vehicle_jeep_scale_medium", "1.0", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_scale_high("hap_vehicle_jeep_scale_high", "1.0", FCVAR_ARCHIVE);
// Jeep Threshold Helper 
static const ThresholdInfo s_ThresholdJeep(	&hap_vehicle_jeep_force_threshold_lower, &hap_vehicle_jeep_scale_low,
											&hap_vehicle_jeep_scale_medium,
											&hap_vehicle_jeep_force_threshold_upper, &hap_vehicle_jeep_scale_high,
											&hap_vehicle_jeep_scale_x,
											&hap_vehicle_jeep_scale_y,
											&hap_vehicle_jeep_scale_z);
C_HapticsHL2::C_HapticsHL2():BaseClass()
{
	VehicleJeepThreshold	=(ThresholdInfo *)&s_ThresholdJeep;
	VehicleAirboatThreshold =(ThresholdInfo *)&s_ThresholdAirboat;
}
extern ConVar hap_debug_vehicle;
ConVar hap_vehicle_force_insensitive("hap_vehicle_force_insensitive","0");
// Haptics --------------------------------------------------------------------
// Purpose: Sets us ready for vehicle velocity sampling effect and informs us the type of vehicle.
//-----------------------------------------------------------------------------
void C_HapticsHL2::SetVehicleMode(byte mode)
{

	// if we are mode sensitive
	if(hap_vehicle_force_insensitive.GetInt()==0)
	{
		//check the mode and set our ForceThreshold to the related vehicle type. 
		if( mode & VEHICLE_TYPE_CAR_WHEELS )
			VehicleForceThreshold = VehicleJeepThreshold;
		else if( mode & VEHICLE_TYPE_AIRBOAT_RAYCAST )
			VehicleForceThreshold = VehicleAirboatThreshold;
	}
	
	ResetSamples();
	ClearMoveButtons();

	m_bUpdateVehicle = true;
	if(hap_debug_vehicle.GetInt() > 0)// if set to 2 it will only show this info.
	{
		const char *spacer = "    ";
		Msg("Entered Vehicle. Type = \n{\n");
		if(mode & VEHICLE_TYPE_CAR_WHEELS)
		{
			Msg( "$sCar (Wheels)\n",spacer );
		}
		if(mode & VEHICLE_TYPE_CAR_RAYCAST)
		{
			Msg( "%sCar (Raycast)\n",spacer );
		}
		if(mode & VEHICLE_TYPE_JETSKI_RAYCAST)
		{
			Msg( "%sJetSki (Raycast)\n",spacer );
		}
		if(mode & VEHICLE_TYPE_AIRBOAT_RAYCAST)
		{
			Msg( "%sAirBoat (Raycast)\n",spacer );
		}
		Msg("}\nEnd\n");
	}
}

//global
static C_HapticsHL2 g_Haptics;
//external access.
C_Haptics *cliHaptics = &g_Haptics;