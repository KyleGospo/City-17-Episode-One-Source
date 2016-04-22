//========= Copyright © 2005-2011, GearDev, All rights reserved. ============//
//
// Purpose: The player's juggernaught friend.
//
//===========================================================================//

#include "cbase.h"
#include "npc_michael.h"

//-----------------------------------------------------------------------------

LINK_ENTITY_TO_CLASS( npc_michael, CNPC_Michael );

//---------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_Michael::Spawn( void )
{
	BaseClass::Spawn();

	m_iHealth			= 80;
	m_bloodColor		= DONT_BLEED;
}

void CNPC_Michael::Precache()
{
	PrecacheModel( STRING( GetModelName() ) );

	BaseClass::Precache();
}

Class_T	CNPC_Michael::Classify ( void )
{
	return	CLASS_PLAYER_ALLY_VITAL;
}

void CNPC_Michael::SelectModel()
{
	char *szModel = (char *)STRING( GetModelName() );
	if (!szModel || !*szModel)
	{
		szModel = "models/humans/group03/male_07.mdl";
		PrecacheModel( "models/humans/group03/male_07.mdl" );
		SetModelName( AllocPooledString(szModel) );
	}
	SetModel( szModel );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_Michael::Activate( void )
{
	//Kick health back up after loading a save game to prevent players from creating a save where michael dies instantly after load.
	m_iHealth = 80;

	BaseClass::Activate();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WeaponProficiency_t CNPC_Michael::CalcWeaponProficiency( CBaseCombatWeapon *pWeapon )
{
	if( FClassnameIs( pWeapon, "weapon_ar2" ) || FClassnameIs( pWeapon, "weapon_shotgun" ) )
	{
		return WEAPON_PROFICIENCY_PERFECT;
	}

	return WEAPON_PROFICIENCY_VERY_GOOD;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
string_t CNPC_Michael::GetModelName() const
{
	string_t iszModelName = CBaseEntity::GetModelName();
	return iszModelName;
}