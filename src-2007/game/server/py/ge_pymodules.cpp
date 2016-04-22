///////////// Copyright © 2009 LodleNet. All rights reserved. /////////////
//
//   Project     : Server
//   File        : ge_pymodules.cpp
//   Description :
//      [TODO: Write the purpose of ge_pymodules.cpp.]
//
//   Created On: 9/1/2009 10:19:52 PM
//   Created By: Mark Chandler <mailto:mark@moddb.com>
////////////////////////////////////////////////////////////////////////////

#include "cbase.h"

#include "baseentity.h"
#include "soundent.h"

#include "hl2_player.h"

#include "basecombatweapon_shared.h"

#include "gamerules.h"

#include "fmod/fmod_manager.h"

#include <boost/python.hpp>
namespace bp = boost::python;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define REG( Name )	extern void init##Name(); PyImport_AppendInittab( #Name , & init##Name );	

void ClientPrintFilter(CBasePlayer *pPlayer, int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );
 
void ClientPrintFilter(CBasePlayer *pPlayer, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	CRecipientFilter filter;
	filter.AddRecipient( pPlayer );
	UTIL_ClientPrintFilter(filter, msg_dest, msg_name, param1, param2, param3, param4);
}
 
float GetTime()
{
	return gpGlobals->curtime;
}
 
const char* GetCVar(const char* name)
{
	const ConVar *cvar = dynamic_cast< const ConVar* >( g_pCVar->FindCommandBase( name ) );
 
	if (!cvar)
		return NULL;
 
	return cvar->GetString();
}
 
 
float DistanceBetween(CBaseEntity *pEntOne, CBaseEntity *pEntTwo)
{
	if (pEntOne && pEntTwo)
		return pEntOne->GetAbsOrigin().DistTo(pEntTwo->GetAbsOrigin());
 
	return 0;
}
 
void locMsg(const char* msg)
{
	Msg(msg);
}
 
void locDevMsg(const char* msg)
{
	DevMsg(msg);
}

void locWarning(const char* msg)
{
	Warning(msg);
}
 
 
 
bool pyIsValidPlayerIndex(int index)
{
	return (UTIL_PlayerByIndex(index) != NULL);
}
 
CBasePlayer* pyGetPlayerByIndex(int index)
{
   return UTIL_PlayerByIndex(index);
}
 
BOOST_PYTHON_FUNCTION_OVERLOADS(UTIL_ClientPrintAll_overloads, UTIL_ClientPrintAll, 2, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(ClientPrintFilter_overloads, ClientPrintFilter, 3, 7);
 
 
 
void QAngle_setitem(QAngle &a, int index, float value)
{
    if (index >= 0 && index < 3) 
	{
        a[index] = value;
    }
    else 
	{
        PyErr_SetString(PyExc_IndexError, "index out of range");
		bp::throw_error_already_set();
    }
}
 
float QAngle_getitem(QAngle &a, int index)
{
    if (index >= 0 && index < 3) 
	{
        return a[index];
    }
    else 
	{
        PyErr_SetString(PyExc_IndexError, "index out of range");
		bp::throw_error_already_set();
    }
}
 
void Vector_setitem(Vector &v, int index, float value)
{
    if (index >= 0 && index < 3) 
	{
        v[index] = value;
    }
    else 
	{
        PyErr_SetString(PyExc_IndexError, "index out of range");
        bp::throw_error_already_set();
    }
}
 
float Vector_getitem(Vector &v, int index)
{
    if (index >= 0 && index < 3) 
	{
        return v[index];
    }
    else 
	{
        PyErr_SetString(PyExc_IndexError, "index out of range");
        bp::throw_error_already_set();
    }
}
 
BOOST_PYTHON_MODULE(HLUtil)
{
	bp::def("ClientPrintAll", UTIL_ClientPrintAll, UTIL_ClientPrintAll_overloads());
	bp::def("ClientPrintFilter", ClientPrintFilter, ClientPrintFilter_overloads());
 
	bp::def("Msg", locMsg);
	bp::def("DevMsg", locDevMsg);
	bp::def("Warning", locWarning);
 
	bp::def("GetTime", GetTime);
	bp::def("GetCVarValue", GetCVar);
	bp::def("DistanceBetween", DistanceBetween);
	bp::def("IsValidPlayerIndex", pyIsValidPlayerIndex);
	bp::def("GetPlayerByIndex", pyGetPlayerByIndex, bp::return_value_policy<bp::reference_existing_object>());
 
	bp::class_< Color >("CColor", bp::init<>())
		.def(bp::init<int, int, int>())
		.def(bp::init<int, int, int, int>())
		.def("SetColor", &Color::SetColor )
		.def("SetRawColor", &Color::SetRawColor )
		.def("GetRawColor", &Color::GetRawColor )
		.def("r", &Color::r )
		.def("g", &Color::g )
		.def("b", &Color::b )
		.def("a", &Color::a );
 
 
	bp::class_<QAngle>("QAngle", bp::init<>())
		.def(bp::init<float, float, float>())
		.def("Random", &QAngle::Random )
		.def("IsValid", &QAngle::IsValid )
		.def("Invalidate", &QAngle::Invalidate )
		.def("Length", &QAngle::Length )
		.def("LengthSqr", &QAngle::LengthSqr )
		.def("__getitem__", &QAngle_getitem)
		.def("__setitem__", &QAngle_setitem)
		.def(bp::self += bp::other<QAngle>())
		.def(bp::self -= bp::other<QAngle>())
		.def(bp::self /= bp::other<float>())
		.def(bp::self *= bp::other<float>());
 
	bp::class_<Vector>("Vector", bp::init<>())
		.def(bp::init<float, float, float>())
		.def("Random", &Vector::Random )
		.def("IsValid", &Vector::IsValid )
		.def("Invalidate", &Vector::Invalidate )
		.def("Length", &Vector::Length )
		.def("LengthSqr", &Vector::LengthSqr )
		.def("Zero", &Vector::Zero )
		.def("Negate", &Vector::Negate )
		.def("Length", &Vector::Length )
		.def("LengthSqr", &Vector::LengthSqr )
		.def("IsZero", &Vector::IsZero )
		.def("NormalizeInPlace", &Vector::NormalizeInPlace )
		.def("IsLengthGreaterThan", &Vector::IsLengthGreaterThan )
		.def("IsLengthLessThan", &Vector::IsLengthLessThan )
		.def("DistTo", &Vector::DistTo )
		.def("DistToSqr", &Vector::DistToSqr )
		.def("MulAdd", &Vector::MulAdd )
		.def("Dot", &Vector::Dot )
		.def("DistTo", &Vector::DistTo )
		.def("Length2D", &Vector::Dot )
		.def("Length2DSqr", &Vector::DistTo )
		.def("__getitem__", &Vector_getitem)
		.def("__setitem__", &Vector_setitem)
		.def(bp::self += bp::other<float>())
		.def(bp::self -= bp::other<float>())
		.def(bp::self /= bp::other<float>())
		.def(bp::self *= bp::other<float>())
		.def(bp::self += bp::other<Vector>())
		.def(bp::self -= bp::other<Vector>())
		.def(bp::self /= bp::other<Vector>())
		.def(bp::self *= bp::other<Vector>());
 
	bp::class_<CSound, boost::noncopyable>("CSound", bp::no_init)
		.def("DoesSoundExpire", &CSound::DoesSoundExpire)
		.def("SoundExpirationTime", &CSound::SoundExpirationTime)
		.def("SetSoundOrigin", &CSound::SetSoundOrigin)
		.def("GetSoundOrigin", &CSound::GetSoundOrigin, bp::return_value_policy<bp::reference_existing_object>())
		.def("GetSoundReactOrigin", &CSound::GetSoundReactOrigin, bp::return_value_policy<bp::reference_existing_object>())
		.def("FIsSound", &CSound::FIsSound)
		.def("FIsScent", &CSound::FIsScent)
		.def("IsSoundType", &CSound::IsSoundType)
		.def("SoundType", &CSound::SoundType)
		.def("SoundContext", &CSound::SoundContext)
		.def("SoundTypeNoContext", &CSound::SoundTypeNoContext)
		.def("Volume", &CSound::Volume)
		.def("OccludedVolume", &CSound::OccludedVolume)
		.def("NextSound", &CSound::NextSound)
		.def("Reset", &CSound::Reset)
		.def("SoundChannel", &CSound::SoundChannel)
		.def("ValidateOwner", &CSound::ValidateOwner);
}

BOOST_PYTHON_MODULE(HLPlayer)
{
	//because CommitSuicide is overloaded we must tell it which one to use
	void (CBasePlayer::*CommitSuicideFP)(bool, bool) = &CBasePlayer::CommitSuicide;
 
	bp::class_<CBasePlayer, bp::bases<CBaseEntity> , boost::noncopyable>("CBasePlayer", bp::no_init)
		.def("DeathCount", &CBasePlayer::DeathCount)
		.def("ResetDeathCount", &CBasePlayer::ResetDeathCount)
		.def("CommitSuicide", CommitSuicideFP)
		.def("GetPlayerName", &CBasePlayer::GetPlayerName)
		.def("GetHealth", &CBasePlayer::GetHealth);
 
	bp::class_<CHL2_Player, bp::bases<CBasePlayer>, boost::noncopyable >("CHL2_Player", bp::no_init)
		.def("Weapon_Switch", &CHL2_Player::Weapon_Switch);
}

BOOST_PYTHON_MODULE(HLGlobal)
{
	bp::scope().attr("WEAPON_NOT_CARRIED") = WEAPON_NOT_CARRIED;
	bp::scope().attr("WEAPON_IS_CARRIED_BY_PLAYER") = WEAPON_IS_CARRIED_BY_PLAYER;
	bp::scope().attr("WEAPON_IS_ACTIVE") = WEAPON_IS_ACTIVE;
 
	bp::scope().attr("SKILL_EASY") = SKILL_EASY;
	bp::scope().attr("SKILL_MEDIUM") = SKILL_MEDIUM;
	bp::scope().attr("SKILL_HARD") = SKILL_HARD;

	bp::scope().attr("HUD_PRINTNOTIFY") = HUD_PRINTNOTIFY;
	bp::scope().attr("HUD_PRINTCONSOLE") = HUD_PRINTCONSOLE;
	bp::scope().attr("HUD_PRINTTALK") = HUD_PRINTTALK;
	bp::scope().attr("HUD_PRINTCENTER") = HUD_PRINTCENTER;
}
 
BOOST_PYTHON_MODULE(HLEntity)
{
	bp::class_< CHandle<CBaseEntity> >("EHANDLE", bp::init<CBaseEntity*>());
 
	bp::class_<CBaseEntity, boost::noncopyable>("CBaseEntity", bp::no_init)
		.def("IsAlive", &CBaseEntity::IsAlive)
		.def("GetFlags", &CBaseEntity::GetFlags)
		.def("GetCollisionGroup", &CBaseEntity::GetCollisionGroup)
		.def("GetId", &CBaseEntity::entindex);
 
	bp::class_<CBaseAnimating, bp::bases<CBaseEntity>, boost::noncopyable>("CBaseAnimating", bp::no_init);
}

BOOST_PYTHON_MODULE(HLWeapon)
{
	bp::class_<CBaseCombatWeapon, bp::bases<BASECOMBATWEAPON_DERIVED_FROM>, boost::noncopyable>("CBaseCombatWeapon", bp::no_init)
		.def("HasAnyAmmo", &CBaseCombatWeapon::HasAnyAmmo)
		.def("HasPrimaryAmmo", &CBaseCombatWeapon::HasPrimaryAmmo)
		.def("HasSecondaryAmmo", &CBaseCombatWeapon::HasSecondaryAmmo)
		.def("UsesPrimaryAmmo", &CBaseCombatWeapon::UsesPrimaryAmmo)
		.def("UsesSecondaryAmmo", &CBaseCombatWeapon::UsesSecondaryAmmo)
		.def("GiveDefaultAmmo", &CBaseCombatWeapon::GiveDefaultAmmo)
 
		.def("CanSight", &CBaseCombatWeapon::CanSight)
		.def("IsIronsighted", &CBaseCombatWeapon::IsIronsighted)
		.def("EnableIronsights", &CBaseCombatWeapon::EnableIronsights)
		.def("DisableIronsights", &CBaseCombatWeapon::DisableIronsights)

		.def("GetViewModel", &CBaseCombatWeapon::GetViewModel)
		.def("GetWorldModel", &CBaseCombatWeapon::GetWorldModel)
		.def("GetAnimPrefix", &CBaseCombatWeapon::GetAnimPrefix)
		.def("GetMaxClip1", &CBaseCombatWeapon::GetMaxClip1)
		.def("GetMaxClip2", &CBaseCombatWeapon::GetMaxClip2)
		.def("GetDefaultClip1", &CBaseCombatWeapon::GetDefaultClip1)
		.def("GetDefaultClip2", &CBaseCombatWeapon::GetDefaultClip2)
		.def("GetWeight", &CBaseCombatWeapon::GetWeight)
		.def("GetWeaponFlags", &CBaseCombatWeapon::GetWeaponFlags)
		.def("GetSlot", &CBaseCombatWeapon::GetSlot)
		.def("GetPosition", &CBaseCombatWeapon::GetPosition)
		.def("GetName", &CBaseCombatWeapon::GetName)
		.def("GetPrintName", &CBaseCombatWeapon::GetPrintName)
		.def("GetShootSound", &CBaseCombatWeapon::GetShootSound)
		.def("UsesClipsForAmmo1", &CBaseCombatWeapon::UsesClipsForAmmo1)
		.def("UsesClipsForAmmo2", &CBaseCombatWeapon::UsesClipsForAmmo2)
		.def("IsMeleeWeapon", &CBaseCombatWeapon::IsMeleeWeapon)
 
		.def("GetPrimaryAmmoCount", &CBaseCombatWeapon::GetPrimaryAmmoCount)
		.def("SetPrimaryAmmoCount", &CBaseCombatWeapon::SetPrimaryAmmoCount)
		.def("GetSecondaryAmmoCount", &CBaseCombatWeapon::GetSecondaryAmmoCount)
		.def("SetSecondaryAmmoCount", &CBaseCombatWeapon::SetSecondaryAmmoCount)
 
		.def("GetClip1", &CBaseCombatWeapon::Clip1)
		.def("GetClip2", &CBaseCombatWeapon::Clip2);
}

BOOST_PYTHON_MODULE(HLGameRules)
{
	bp::class_<CAutoGameSystemPerFrame, boost::noncopyable>("CAutoGameSystemPerFrame", bp::no_init)
		.def("IsSkillLevel", &CGameRules::IsSkillLevel)
		.def("GetSkillLevel", &CGameRules::GetSkillLevel)
		.def("SetSkillLevel", &CGameRules::SetSkillLevel)
		.def("PlayerRelationship", &CGameRules::PlayerRelationship)
		.def("GetAutoAimMode", &CGameRules::GetAutoAimMode)
		.def("CanHaveItem", &CGameRules::CanHaveItem);
}

BOOST_PYTHON_MODULE(FMOD)
{
	bp::class_< CFMODManager, boost::noncopyable >("CFMODManager", bp::no_init)
		.def("IsSoundPlaying", &CFMODManager::IsSoundPlaying)
		.def("IsPaused", &CFMODManager::IsPaused)
		.def("PlayAmbientSound", &CFMODManager::PlayAmbientSound)
		.def("StopAmbientSound", &CFMODManager::StopAmbientSound)
		.def("PauseAmbientSound", &CFMODManager::PauseAmbientSound)
		.def("TransitionAmbientSounds", &CFMODManager::TransitionAmbientSounds);
}

extern "C"
{

void RegisterPythonModules()
{
	REG( HLUtil );
	REG( HLPlayer );
	REG( HLGlobal );
	REG( HLEntity );
	REG( HLWeapon );
	REG( HLGameRules );
	REG( FMOD );
}

}