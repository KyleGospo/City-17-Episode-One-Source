#ifndef IN_HAPTICS_HL2_H
#define IN_HAPTICS_HL2_H
#ifdef _WIN32
#pragma once
#endif



#include "in_haptics.h"

// DO NOT MODIFY (values must match vehicle.h on server)
// START
#define VEHICLE_TYPE_CAR_WHEELS			(1<<0)
#define VEHICLE_TYPE_CAR_RAYCAST		(1<<1)
#define VEHICLE_TYPE_JETSKI_RAYCAST		(1<<2)
#define VEHICLE_TYPE_AIRBOAT_RAYCAST	(1<<3)
// END

// this is a example of how to extend the C_Haptics class for game specific settings
// in this case mainly we are doing this class to treat the half life 2 airboat and 
// jeep handled different.
class C_HapticsHL2 : public C_Haptics
{
public:
	DECLARE_CLASS_GAMEROOT( C_HapticsHL2, C_Haptics );
	C_HapticsHL2();

	// Starts velocity sampling and informs us of what type of vehicle we are using.
	void		SetVehicleMode(byte mode);
private:
	ThresholdInfo	   *VehicleJeepThreshold;
	ThresholdInfo	   *VehicleAirboatThreshold;

};
#endif