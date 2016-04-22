//========= Copyright © 2005-2011, GearDev, All rights reserved. ============//
//
// Purpose: The player's juggernaught friend.
//
//===========================================================================//

#ifndef	NPC_MICHAEL_H
#define	NPC_MICHAEL_H
#ifdef _WIN32
#pragma once
#endif

#include "npc_citizen17.h"

class CNPC_Michael : public CNPC_Citizen
{
	DECLARE_CLASS( CNPC_Michael, CNPC_Citizen );

public:
	void			Spawn( void );
	void			Precache();
	Class_T Classify ( void );
	virtual void	SelectModel();
	void			Activate( void );
	WeaponProficiency_t CalcWeaponProficiency( CBaseCombatWeapon *pWeapon );

	//---------------------------------
	string_t 		GetModelName() const;
};

#endif //NPC_MICHAEL_H