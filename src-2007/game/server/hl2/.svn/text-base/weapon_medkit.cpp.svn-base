//========= Copyright © 2007-2009, Gear Developer Team, All rights reserved. ============//
//
// Purpose:		Medical Kit (AUGH OH GAWD, MY ARM IS BROKEN IN 12 PLACES. LET ME JUST POP IT BACK SO IT MAGICALLY HEALS)
//
// $NoKeywords: $
//=======================================================================================//

#include "cbase.h"
#include "player.h"
#include "basehlcombatweapon.h"
#include "weapon_medkit.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_SERVERCLASS_ST(CMedKit, DT_MedKit)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_medkit, CMedKit );
PRECACHE_WEAPON_REGISTER( weapon_medkit );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMedKit::CMedKit( )
{
	m_flHeal1Time = gpGlobals->curtime;
	m_flHeal2Time = gpGlobals->curtime;
	m_flHeal3Time = gpGlobals->curtime;
	m_bNeedHeal1 = false;
	m_bNeedHeal2 = false;
	m_bNeedHeal3 = false;
}

//-----------------------------------------------------------------------------
// Purpose: Precache
//-----------------------------------------------------------------------------
void CMedKit::Precache( void )
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Handle grenade detonate in-air (even when no ammo is left)
//-----------------------------------------------------------------------------
void CMedKit::ItemPostFrame( void )
{
	BaseClass::ItemPostFrame();

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if( m_flHeal1Time <= gpGlobals->curtime && m_bNeedHeal1 )
	{
		//Heal 10 HP if large, 5 if small
		if( !m_bIsSmall )
			pOwner->TakeHealth( 10.0f, DMG_GENERIC );
		else
			pOwner->TakeHealth( 5.0f, DMG_GENERIC );

		m_bNeedHeal1 = false;
	}

	if( m_flHeal2Time <= gpGlobals->curtime && m_bNeedHeal2 )
	{
		//Heal 10 HP if large, 5 if small
		if( !m_bIsSmall )
			pOwner->TakeHealth( 10.0f, DMG_GENERIC );
		else
			pOwner->TakeHealth( 5.0f, DMG_GENERIC );

		m_bNeedHeal2 = false;
	}

	//Never called by the small healthkit heal.
	if( m_flHeal3Time <= gpGlobals->curtime && m_bNeedHeal3 )
	{
		//Heal 5 HP
		pOwner->TakeHealth( 5.0f, DMG_GENERIC );
		m_bNeedHeal3 = false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Override if we're waiting to release a shot
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CMedKit::CanHolster( void )
{
	return BaseClass::CanHolster();
}

//-----------------------------------------------------------------------------
// Purpose: Large MedKit Heal
//-----------------------------------------------------------------------------
void CMedKit::PrimaryAttack( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	//if ( m_iClip1 <= 0 )
	//{
	//	pOwner->m_flNextAttack = gpGlobals->curtime + 1;
	//	return;
	//}

	m_iClip1 = m_iClip1 - 1;

	m_bIsSmall = false;

	m_bNeedHeal1 = true;
	m_bNeedHeal2 = true;
	m_bNeedHeal3 = true;

	m_flHeal1Time = gpGlobals->curtime + 1;
	m_flHeal2Time = gpGlobals->curtime + 2;
	m_flHeal3Time = gpGlobals->curtime + 3;

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pOwner->m_flNextAttack = gpGlobals->curtime + SequenceDuration();

	Msg( "Healed Successfully!" );
}

//-----------------------------------------------------------------------------
// Purpose: Small MedKit Heal
//-----------------------------------------------------------------------------
void CMedKit::SecondaryAttack( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	if ( m_iClip2 <= 0 )
	{
		pOwner->m_flNextAttack = gpGlobals->curtime + 1;
		return;
	}

	m_iClip2 = m_iClip2 - 1;

	m_bIsSmall = true;

	m_bNeedHeal1 = true;
	m_bNeedHeal2 = true;
	m_bNeedHeal3 = false;

	m_flHeal1Time = gpGlobals->curtime + 1.5;
	m_flHeal2Time = gpGlobals->curtime + 3;

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pOwner->m_flNextAttack = gpGlobals->curtime + SequenceDuration();
}
