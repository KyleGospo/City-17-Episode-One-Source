#include "basehlcombatweapon.h"

#ifndef WEAPON_MEDKIT_H
#define WEAPON_MEDKIT_H
#ifdef _WIN32
#pragma once
#endif

//---------------------
// City 17 Emergency Medical Kit
//---------------------
class CMedKit:public CBaseHLCombatWeapon
{
public:
	DECLARE_CLASS( CMedKit, CBaseHLCombatWeapon );

	DECLARE_SERVERCLASS();

	CMedKit();

	CNetworkVar( float, m_flHeal1Time );
	CNetworkVar( float, m_flHeal2Time );
	CNetworkVar( float, m_flHeal3Time );

	void Precache( void );
	void ItemPostFrame( void );
	void PrimaryAttack( void );
	void SecondaryAttack( void );

	virtual int	GetMinBurst() 
	{ 
		return 1; 
	}

	virtual int	GetMaxBurst() 
	{ 
		return 3; 
	}

	virtual float GetFireRate( void ) 
	{
		return 0.1f; 
	}

private:
	bool CanHolster( void );

	bool m_bNeedHeal1;
	bool m_bNeedHeal2;
	bool m_bNeedHeal3;
	bool m_bIsSmall;
};

#endif // WEAPON_MEDKIT_H