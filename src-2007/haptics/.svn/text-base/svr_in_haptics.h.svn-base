#ifndef SVR_IN_HAPTICS_H
#define SVR_IN_HAPTICS_H

#ifdef _WIN32
#pragma once
#endif

#include "../dlls/player.h"
#include "utlvector.h"

enum {
	VEHICLE_TYPE_NONE,
	VEHICLE_TYPE_AIRBOAT,
	VEHICLE_TYPE_JEEP };

enum {
	STEPTYPE_SOLID,
	STEPTYPE_SPLASHING,
	STEPTYPE_WADING,
	STEPTYPE_SWIMMING };

class CHaptics
{
	friend void hap_cc_test(void);
public:
	CHaptics();
	
	void	Recoil(float fStrength, QAngle fDisplacement); // Automatically start off with a vector of (0,0,1)
	void	Footsteps(int material_type, float fStrength);
	void	Damage(CBasePlayer *pPlayer, CBaseEntity *eInflictor, float fDamage, int bitDamageType);
	void	Weight_On( void );
	void	Weight_Set(float fMass, float fCarryMax);
	void	Weight_Off( void );
	void	Vehicle_On( void );
	bool	Vehicle_IsOn( void );
	void	Vehicle_Off( void );
	void	Vehicle_Update( Vector *vVelocityAdr, int nVehicleType );
	void	Avatar_Update( Vector *vVelocityAdr );
	void	Carry_Update( Vector *vVelocityAdr );

private:
	int					nVelocitySmooth;
	int					nSampleCount;
	int					nTotalSamples;
	//int					nSampleCount_2nd;
	int					nDiffCount;
	Vector				vOldVelocity;
	Vector				vOldAcceleration;
	CFastTimer			tVelocityTimer;
	CFastTimer			tAccelerationTimer;
	bool				bUpdateVehicle;
	CUtlVector<Vector>	*vVelocityQueue;
	float				fAverageTime;
	int					nEstimatedCycles;
	float				fEstimatedTime;
	bool				bCyclesEstimated;
	int					nClockSpeed;
	bool				bTypeInit;
	Vector				vCarryAverage;
};

extern ::CHaptics *svrHaptics;

#endif // SVR_IN_HAPTICS_H