#include "cbase.h"

#include "convar.h"
#include "player.h"
#include "svr_in_haptics.h"
#include "utlvector.h"

// Recoil
ConVar hap_var_recoil_value("hap_var_recoil_value", "0", FCVAR_REPLICATED );
ConVar hap_var_recoil_strength("hap_var_recoil_strength", "0", FCVAR_REPLICATED );
ConVar hap_var_recoil_pitch("hap_var_recoil_pitch", "0", FCVAR_REPLICATED );
ConVar hap_var_recoil_yaw("hap_var_recoil_yaw", "0", FCVAR_REPLICATED );
ConVar hap_var_recoil_roll("hap_var_recoil_roll", "0", FCVAR_REPLICATED );

// Footsteps
ConVar hap_var_step_type( "hap_var_step_type", "0", FCVAR_REPLICATED );
ConVar hap_var_step_value( "hap_var_step_value", "0.0", FCVAR_REPLICATED );

// Damage vector
ConVar hap_var_damage_on( "hap_var_damage_on", "0", FCVAR_REPLICATED );
ConVar hap_var_damage_angle_pitch( "hap_var_damage_angle_pitch", "0", FCVAR_REPLICATED );
ConVar hap_var_damage_angle_yaw( "hap_var_damage_angle_yaw", "0", FCVAR_REPLICATED );
ConVar hap_var_damage_amount( "hap_var_damage_amount", "0", FCVAR_REPLICATED );

// Physics / Carry
ConVar hap_var_phys_carry_on( "hap_var_phys_carry_on", "0", FCVAR_REPLICATED );
ConVar hap_var_phys_carry_reset( "hap_var_phys_carry_reset", "0", FCVAR_REPLICATED );
ConVar hap_var_phys_carry_weight( "hap_var_phys_carry_weight", "0", FCVAR_REPLICATED );
ConVar hap_carry_physgun_scale( "hap_carry_physgun_scale", "0.5", FCVAR_ARCHIVE );

// Physics / Vehicle
ConVar hap_vehicle_airboat_samples("hap_vehicle_airboat_samples", "30", FCVAR_ARCHIVE);
ConVar hap_vehicle_jeep_samples("hap_vehicle_jeep_samples", "30", FCVAR_ARCHIVE);

//BA CES07
ConVar hap_var_isInVehicle("hap_var_isInVehicle", "0", FCVAR_REPLICATED);

ConVar hap_var_vehicle_force_on("hap_var_vehicle_force_on", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_force_reset("hap_var_vehicle_force_reset", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_force_update("hap_var_vehicle_force_update", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_force_clear("hap_var_vehicle_force_clear", "0", FCVAR_REPLICATED);

ConVar hap_var_vehicle_type("hap_var_vehicle_type", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltav_x("hap_var_vehicle_deltav_x", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltav_y("hap_var_vehicle_deltav_y", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltav_z("hap_var_vehicle_deltav_z", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltat("hap_var_vehicle_deltat", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltaa_x("hap_var_vehicle_deltaa_x", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltaa_y("hap_var_vehicle_deltaa_y", "0", FCVAR_REPLICATED);
ConVar hap_var_vehicle_deltaa_z("hap_var_vehicle_deltaa_z", "0", FCVAR_REPLICATED);
//static bool bDriver = false;

// Physics / Avatar
ConVar hap_avatar_samples("hap_avatar_samples", "30", FCVAR_ARCHIVE);

ConVar hap_var_avatar_force_on("hap_var_avatar_force_on", "0", FCVAR_REPLICATED);

ConVar hap_var_avatar_deltav_x("hap_var_avatar_deltav_x", "0", FCVAR_REPLICATED);
ConVar hap_var_avatar_deltav_y("hap_var_avatar_deltav_y", "0", FCVAR_REPLICATED);
ConVar hap_var_avatar_deltav_z("hap_var_avatar_deltav_z", "0", FCVAR_REPLICATED);
ConVar hap_var_avatar_deltat("hap_var_avatar_deltat", "0", FCVAR_REPLICATED);


CHaptics::CHaptics()
{
	nSampleCount = 0;
	nTotalSamples = 0;
	nEstimatedCycles = 0;
	//nSampleCount_2nd = 0;
	nDiffCount = 0;
	bUpdateVehicle = false;
	bCyclesEstimated = false;
	bTypeInit = false;

	vVelocityQueue = new CUtlVector<Vector>;
}

void CHaptics::Recoil(float fStrength, QAngle fDisplacement)
{
	hap_var_recoil_value.SetValue(1);
	hap_var_recoil_strength.SetValue(fStrength);
	hap_var_recoil_pitch.SetValue(fDisplacement[PITCH]);
	hap_var_recoil_yaw.SetValue(fDisplacement[YAW]);
	hap_var_recoil_roll.SetValue(fDisplacement[ROLL]);
}

void CHaptics::Footsteps(int material_type, float fStrength)
{
	hap_var_step_type.SetValue(material_type);
	hap_var_step_value.SetValue(fStrength);
}

void CHaptics::Damage(CBasePlayer *pPlayer, CBaseEntity *eInflictor, float fDamage, int bitDamageType)
{
	// Player Data
	Vector playerPosition = pPlayer->GetLocalOrigin();
	QAngle playerAngles = pPlayer->GetLocalAngles();
	Vector inflictorPosition = eInflictor->GetLocalOrigin();
	QAngle inflictorAngles = eInflictor->GetLocalAngles();

	// Calculate Yaw Angle
	float yawAngle = playerAngles.y - inflictorAngles.y - 180.0;

	// Calculate Pitch Angle
	Vector diffVector = playerPosition - inflictorPosition;
	float triHypoteneus = (float)(playerPosition.DistTo(inflictorPosition));
	float triAdjacent = sqrt((float)(
		pow((float)(diffVector.x), (int)2) + 
		pow((float)(diffVector.y), (int)2)));
	float pitchAngle = acos(triAdjacent / triHypoteneus);
	pitchAngle *= 180.0 / M_PI; // convert to degrees
	if(diffVector.z > 0)
		pitchAngle *= -1.0;

	// Haptics addition
	hap_var_damage_on.SetValue(1);
	if(bitDamageType & DMG_FALL)
		hap_var_damage_angle_pitch.SetValue((float)-90.0); // coming from beneath
	else
		hap_var_damage_angle_pitch.SetValue(pitchAngle);
	hap_var_damage_angle_yaw.SetValue(-yawAngle); //(goes from -0 to -360?)
	hap_var_damage_amount.SetValue(
		(bitDamageType & ( DMG_CRUSH | DMG_BULLET | DMG_SLASH | DMG_VEHICLE |
		DMG_FALL | DMG_BLAST | DMG_CLUB | DMG_SONIC | DMG_ENERGYBEAM | DMG_PHYSGUN |
		DMG_AIRBOAT | DMG_BLAST_SURFACE | DMG_BUCKSHOT)) ? fDamage : (float)0.0 );
}

void CHaptics::Weight_On( )
{
	hap_var_phys_carry_on.SetValue(1);
}
void CHaptics::Weight_Set(float fMass, float fCarryMax)
{
	if(fCarryMax == 35.0) // player carry
		hap_var_phys_carry_weight.SetValue(fMass / fCarryMax);
	else // physgun carry
		hap_var_phys_carry_weight.SetValue(hap_carry_physgun_scale.GetFloat() * (fMass / fCarryMax));
}

void CHaptics::Weight_Off( )
{
	hap_var_phys_carry_weight.SetValue((float)0.0);
	hap_var_phys_carry_reset.SetValue((int)1);
	hap_var_phys_carry_on.SetValue(0);
}

void CHaptics::Vehicle_On( )
{
	nSampleCount = 0;
	nTotalSamples = 0;
	vVelocityQueue->RemoveAll();
	hap_var_isInVehicle.SetValue((int)1);
	hap_var_vehicle_force_on.SetValue((int)1);
	bUpdateVehicle = true;
	nDiffCount = 0;
}

bool CHaptics::Vehicle_IsOn( void )
{
	return bUpdateVehicle;
}

	
void CHaptics::Vehicle_Off( )
{
	hap_var_isInVehicle.SetValue((int)0);
	hap_var_vehicle_force_on.SetValue((int)0);
	hap_var_vehicle_force_reset.SetValue((int)1);
	hap_var_vehicle_force_clear.SetValue((int)1);
	nSampleCount = 0;
	nTotalSamples = 0;
	bUpdateVehicle = false;
	bTypeInit = false;
	vVelocityQueue->RemoveAll();
	nDiffCount = 0;
}

CON_COMMAND(hap_cc_test, "Testing out concommand ability\n")
{
	Msg("The time per physics update is: %0.8f seconds (approximate deltaT = %0.6f)\n",
		(float)svrHaptics->fEstimatedTime, (float)svrHaptics->fEstimatedTime * (float)svrHaptics->nTotalSamples);
}

ConVar hap_temp_feedback("hap_temp_feedback", "0");
Vector vLocalAcceleration;

void CHaptics::Vehicle_Update( Vector *vVelocityAdr, int nVehicleType )
{
	if(nVehicleType == VEHICLE_TYPE_AIRBOAT)
		nTotalSamples = hap_vehicle_airboat_samples.GetInt();
	if(nVehicleType == VEHICLE_TYPE_JEEP)
		nTotalSamples = hap_vehicle_jeep_samples.GetInt();
	if(!bTypeInit)
	{hap_var_vehicle_type.SetValue((int)nVehicleType);bTypeInit = true;}

	// Should only call once
	if(nSampleCount == 0)
	{
		Vector vLocalVelocity(vVelocityAdr->x, vVelocityAdr->y, vVelocityAdr->z);
		//vOldVelocity = vLocalVelocity;
		vVelocityQueue->AddToTail(vLocalVelocity);
		nSampleCount = 1;
		tVelocityTimer.Start();
	}
	// Create new sample
	else if(nSampleCount >= nTotalSamples)
	{
		tVelocityTimer.End();

		fAverageTime += (float)tVelocityTimer.GetDuration().GetSeconds();
		fAverageTime /= 2.0;

		Vector vLocalVelocity(vVelocityAdr->x, vVelocityAdr->y, vVelocityAdr->z);

		// Queue not full yet
		if(nDiffCount < 2)
		{
			vVelocityQueue->AddToTail(vLocalVelocity);
			nDiffCount++;
		}
		else
		{
			vVelocityQueue->Remove(0); // remove at head
			vVelocityQueue->AddToTail(vLocalVelocity);
			vLocalAcceleration = Vector(
				vVelocityQueue->Element(2).x - vVelocityQueue->Element(0).x,
				vVelocityQueue->Element(2).y - vVelocityQueue->Element(0).y,
				vVelocityQueue->Element(2).z - vVelocityQueue->Element(0).z);

			hap_var_vehicle_deltat.SetValue(fAverageTime);
			
			hap_var_vehicle_deltav_x.SetValue(vLocalAcceleration.x);
			hap_var_vehicle_deltav_y.SetValue(vLocalAcceleration.y);
			hap_var_vehicle_deltav_z.SetValue(vLocalAcceleration.z);

			/*
			Vector vLocalShape = Vector(
				vVelocityQueue->Element(2).x + vVelocityQueue->Element(0).x - (2.0 * vVelocityQueue->Element(1).x),
				vVelocityQueue->Element(2).y + vVelocityQueue->Element(0).y - (2.0 * vVelocityQueue->Element(1).y),
				vVelocityQueue->Element(2).z + vVelocityQueue->Element(0).z - (2.0 * vVelocityQueue->Element(1).z));
			
			hap_var_vehicle_deltaa_x.SetValue(vLocalShape.x);
			hap_var_vehicle_deltaa_y.SetValue(vLocalShape.y);
			hap_var_vehicle_deltaa_z.SetValue(vLocalShape.z);
			*/

			hap_var_vehicle_force_update.SetValue((int)1);
		}

		nSampleCount = 1;
		tVelocityTimer.Start();
	}
	else
	{
		//if(!bCyclesEstimated)
		//{
			//fEstimatedTime += tVelocityTimer.GetDurationInProgress().GetSeconds();
			//fEstimatedTime /= 2.0;
			//nClockSpeed += tVelocityTimer.GetClockSpeed();
			//nClockSpeed /= 2;
			//bCyclesEstimated = true;
		//}
		nSampleCount++;
	}
}

void CHaptics::Avatar_Update( Vector *vVelocityAdr  )
{
	Vector tempVec0;
	Vector tempVec1;
	Vector tempVec2;

	if(hap_temp_feedback.GetInt() == 1)
		Msg("Velocity = %+00.6f, %+00.6f, %+00.6f - Acceleration = %+00.6f, %+00.6f, %+00.6f - dt = %+00.4f\n",
			vVelocityAdr->x * METERS_PER_INCH, vVelocityAdr->y * METERS_PER_INCH, vVelocityAdr->z * METERS_PER_INCH,
			vLocalAcceleration.x * METERS_PER_INCH / (2*fAverageTime),
			vLocalAcceleration.y * METERS_PER_INCH / (2*fAverageTime),
			vLocalAcceleration.z * METERS_PER_INCH / (2*fAverageTime), fAverageTime);

	nTotalSamples = hap_avatar_samples.GetInt();
	hap_var_avatar_force_on.SetValue((int)1);

	// Should only call once
	if(nSampleCount == 0)
	{
		Vector vLocalVelocity(vVelocityAdr->x, vVelocityAdr->y, vVelocityAdr->z);
		vVelocityQueue->AddToTail(vLocalVelocity);
		nSampleCount = 1;
		tVelocityTimer.Start();
	}
	// Create new sample
	else if(nSampleCount >= nTotalSamples)
	{
		tVelocityTimer.End();

		Vector vLocalVelocity(vVelocityAdr->x, vVelocityAdr->y, vVelocityAdr->z);

		fAverageTime += (float)tVelocityTimer.GetDuration().GetSeconds();
		fAverageTime /= 2.0;

		// Queue not full yet
		if(nDiffCount < 2)
		{
			vVelocityQueue->AddToTail(vLocalVelocity);
			nDiffCount++;
		}
		else
		{
			vVelocityQueue->Remove(0); // remove at head
			vVelocityQueue->AddToTail(vLocalVelocity);

			Vector vLocalAcceleration = Vector(
				vVelocityQueue->Element(2).x - vVelocityQueue->Element(0).x,
				vVelocityQueue->Element(2).y - vVelocityQueue->Element(0).y,
				vVelocityQueue->Element(2).z - vVelocityQueue->Element(0).z);

			hap_var_avatar_deltav_x.SetValue(vLocalAcceleration.x);
			hap_var_avatar_deltav_y.SetValue(vLocalAcceleration.y);
			hap_var_avatar_deltav_z.SetValue(vLocalAcceleration.z);

			hap_var_avatar_deltat.SetValue(fAverageTime);
		}

		nSampleCount = 1;
		tVelocityTimer.Start();
	}
	else
		nSampleCount++;
}

void CHaptics::Carry_Update(Vector *vVelocityAdr)
{
	Vector vLocalVelocity(vVelocityAdr->x, vVelocityAdr->y, vVelocityAdr->z);

	vCarryAverage = (vCarryAverage / 2.0) + (vLocalVelocity / 2.0);

	//Msg("Average velocity (PYR) - %+000.2f, %+000.2f, %+000.2f - Length - %+000.2f\n",
	//	vCarryAverage.x, vCarryAverage.y, vCarryAverage.z, vCarryAverage.Length());
}

// Global hook
static CHaptics g_Haptics;
CHaptics *svrHaptics = &g_Haptics;