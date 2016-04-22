//========= Copyright © 2009, Team Gear, All rights reserved. =================//
//
// Purpose:		Binoculars - Replacement for suit zoom.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "NPCEvent.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "AI_BaseNPC.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// CWeaponBinoculars
//-----------------------------------------------------------------------------

class CWeaponBinoculars : public CBaseHLCombatWeapon
{
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CWeaponBinoculars, CBaseHLCombatWeapon );

	CWeaponBinoculars(void);

	DECLARE_SERVERCLASS();

	void	Precache( void );
	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	void	ItemPostFrame( void );
	void	ItemBusyFrame( void );

	int		CapabilitiesGet( void ) { return bits_CAP_INNATE_MELEE_ATTACK1; }

	DECLARE_ACTTABLE();

private:
	void	CheckZoomToggle( void );
	void	ToggleZoom( void );

	bool	m_bInZoom;
};


IMPLEMENT_SERVERCLASS_ST(CWeaponBinoculars, DT_WeaponBinoculars)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_binoculars, CWeaponBinoculars );
PRECACHE_WEAPON_REGISTER( weapon_binoculars );

BEGIN_DATADESC( CWeaponBinoculars )
END_DATADESC()

acttable_t	CWeaponBinoculars::m_acttable[] = 
{
	{ ACT_IDLE,						ACT_IDLE_PISTOL,				true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_PISTOL,			true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_PISTOL,		true },
	{ ACT_RELOAD,					ACT_RELOAD_PISTOL,				true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_PISTOL,			true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_PISTOL,				true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_PISTOL,true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_PISTOL_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_PISTOL_LOW,	false },
	{ ACT_COVER_LOW,				ACT_COVER_PISTOL_LOW,			false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_PISTOL_LOW,		false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_WALK,						ACT_WALK_PISTOL,				false },
	{ ACT_RUN,						ACT_RUN_PISTOL,					false },
};


IMPLEMENT_ACTTABLE( CWeaponBinoculars );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponBinoculars::CWeaponBinoculars( void )
{

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponBinoculars::Precache( void )
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponBinoculars::ItemBusyFrame( void )
{
	// Allow zoom toggling even when we're reloading
	CheckZoomToggle();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponBinoculars::ItemPostFrame( void )
{
	// Allow zoom toggling
	CheckZoomToggle();

	BaseClass::ItemPostFrame();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponBinoculars::ToggleZoom( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	if( m_bIsIronsighted )
	{
		DisableIronsights();
		m_bInZoom = false;
	}
	else
	{
		EnableIronsights();
		m_bInZoom = true;
	}

	m_flReIronsightTime = gpGlobals->curtime + 0.4f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponBinoculars::CheckZoomToggle( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( (pPlayer->m_afButtonPressed & IN_ATTACK || pPlayer->m_afButtonPressed & IN_ATTACK2) && m_flReIronsightTime < gpGlobals->curtime )
	{
			ToggleZoom();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponBinoculars::PrimaryAttack( void )
{
	//Handled in Item Pre/Post Frame.
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponBinoculars::SecondaryAttack( void )
{
	//Handled in Item Pre/Post Frame.
}