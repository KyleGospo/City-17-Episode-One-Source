//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Projectile shot from the AR3 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef	WEAPONAR3_H
#define	WEAPONAR3_H

#include "basegrenade_shared.h"
#include "basehlcombatweapon.h"

class CWeaponAR3 : public CHLMachineGun
{
public:
	DECLARE_CLASS( CWeaponAR3, CHLMachineGun );

	CWeaponAR3();

	DECLARE_SERVERCLASS();

	void	ItemPostFrame( void );
	void	Precache( void );
	void	WeaponIdle( void );
	
	void	SecondaryAttack( void );
	void	PrimaryAttack( void );
	void	DelayedAttack( void );

	const char *GetTracerType( void ) { return "gunfire_ar3_beams"; }

	void	AddViewKick( void );

	void	FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles );
	void	FireNPCSecondaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles );
	void	Operator_ForceNPCFire( CBaseCombatCharacter  *pOperator, bool bSecondary );
	void	Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	bool	Deploy( void );
	//bool	Holster( CBaseCombatWeapon *pSwitchingTo );

	int		GetMinBurst( void ) { return 2; }
	int		GetMaxBurst( void ) { return 5; }
	float	GetFireRate( void ) { return 0.1f; }

	bool	CanHolster( void );
	bool	Reload( void ) { return false; }

	virtual bool			CanBePickedUpByNPCs( void ) { return false;	}

	int		CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	Activity	GetPrimaryAttackActivity( void );
	
	void	DoImpactEffect( trace_t &tr, int nDamageType );

	/*virtual const Vector& GetBulletSpread( void )
	{
		static Vector cone;

		if ( m_bIsIronsighted )
		{
			cone = VECTOR_CONE_4DEGREES;
		}
		else
		{
			cone = VECTOR_CONE_5DEGREES;
		}

		return cone;
	}*/

	//City17: Normal bullet vectors.
	virtual const Vector& GetBulletSpread_Normal( void )
	{
		if ( m_bIsIronsighted )
		{
			static const Vector cone = VECTOR_CONE_4DEGREES;
			return cone;
		}
		else
		{
			static const Vector cone = VECTOR_CONE_5DEGREES;
			return cone;
		}
	}
	
	//City17: Bullet vectors while ducking.
	virtual const Vector& GetBulletSpread_Ducked( void )
	{
		if ( m_bIsIronsighted )
		{
			static const Vector cone = VECTOR_CONE_3DEGREES;
			return cone;
		}
		else
		{
			static const Vector cone = VECTOR_CONE_4DEGREES;
			return cone;
		}
	}

	//City17: Bullet vectors while in the air.
	virtual const Vector& GetBulletSpread_InAir( void )
	{
		static const Vector cone = VECTOR_CONE_8DEGREES;
		return cone;
	}
	
	//City17: Bullet vectors while running.
	virtual const Vector& GetBulletSpread_Running( void )
	{
		static const Vector cone = VECTOR_CONE_7DEGREES;
		return cone;
	}

	//City17: Determine bullet vectors.
	virtual const Vector& GetBulletSpread( void )
	{
		if ( !GetOwner() )
		{
			static const Vector cone = VECTOR_CONE_5DEGREES;
			return cone;
		}
		
		CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

		//If we don't have a player, just return the old default vector.
		//Interestingly (Though not really) NPC's will resolve to this.
		if ( !pPlayer )
		{
			static const Vector cone = VECTOR_CONE_5DEGREES;
			return cone;
		}

		//If our player is ducking, give him a large accuracy boost.
		if ( pPlayer->GetFlags() & FL_DUCKING )
			return GetBulletSpread_Ducked();
		
		//If our player is in the air, lower his accuacy severely.
		if ( !( pPlayer->GetFlags() & FL_ONGROUND ) )
			return GetBulletSpread_InAir();

		//If our player is running, slightly lower his accuracy as a penalty.
		float speed;
		speed = pPlayer->GetAbsVelocity().Length2D();

		if ( speed >= 320 && pPlayer->GetFlags() & FL_ONGROUND )
			return GetBulletSpread_Running();

		//If none of the above conditions are true, return normal accuracy.
		return GetBulletSpread_Normal();
	}

	const WeaponProficiencyInfo_t *GetProficiencyValues();

protected:

	float					m_flDelayedFire;
	bool					m_bShotDelayed;
	int						m_nVentPose;
	
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();
};


#endif	//WEAPONAR3_H
