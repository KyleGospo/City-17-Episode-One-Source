#include "cbase.h"

#include "baseentity.h"
#include "soundent.h"
#include <boost/python.hpp>
namespace bp = boost::python;
 
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

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
 
void locWarning(const char* msg)
{
	Warning(msg);
}
 
 
 
bool pyIsValidPlayerIndex(int index)
{
	return (UTIL_PlayerByIndex(index) != NULL);
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
	bp::def("Warning", locWarning);
 
	bp::def("GetTime", GetTime);
	bp::def("GetCVarValue", GetCVar);
	bp::def("DistanceBetween", DistanceBetween);
	bp::def("IsValidPlayerIndex", pyIsValidPlayerIndex);
 
 
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