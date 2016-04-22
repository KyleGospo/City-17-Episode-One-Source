#include "cbase.h"
#include "city17/gravitywell.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sv_gravity_scale( "sv_gravity_scale", "0.005", FCVAR_REPLICATED );

CUtlVector<CGravityWell*> g_GravityWells;

IMPLEMENT_NETWORKCLASS_ALIASED( GravityWell, DT_GravityWell )

BEGIN_NETWORK_TABLE( CGravityWell, DT_GravityWell )
#ifdef CLIENT_DLL
	RecvPropFloat(RECVINFO(m_flStrength)),
	RecvPropInt(RECVINFO(m_iDisabled)),
	RecvPropInt(RECVINFO(m_iNeedsLOS)),
#else
	SendPropFloat(SENDINFO(m_flStrength)),
	SendPropInt(SENDINFO(m_iDisabled), 1, SPROP_UNSIGNED ),
	SendPropInt(SENDINFO(m_iNeedsLOS), 1, SPROP_UNSIGNED ),
#endif
END_NETWORK_TABLE()

#ifndef CLIENT_DLL
BEGIN_DATADESC( CGravityWell )
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", Toggle ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", Enable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", Disable ),

	DEFINE_KEYFIELD( m_flStrength, FIELD_FLOAT, "strength" ),
	DEFINE_KEYFIELD( m_iDisabled, FIELD_INTEGER, "disabled" ),
	DEFINE_KEYFIELD( m_iNeedsLOS, FIELD_INTEGER, "los" ),
END_DATADESC()
#else
BEGIN_PREDICTION_DATA( CGravityWell )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( point_gravity, CGravityWell );

CGravityWell::CGravityWell()
{
	g_GravityWells.AddToTail(this);
	m_iDisabled = false;
}

CGravityWell::~CGravityWell()
{
	g_GravityWells.FindAndRemove(this);
}

void CGravityWell::Spawn( void )
{
	BaseClass::Spawn();
}

void CGravityWell::Toggle( inputdata_t &inputdata )
{
	m_iDisabled = 1-m_iDisabled;
}

void CGravityWell::Enable( inputdata_t &inputdata )
{
	m_iDisabled = 0;
}

void CGravityWell::Disable( inputdata_t &inputdata )
{
	m_iDisabled = 1;
}

bool CGravityWell::ShouldAffectEntity(CBaseEntity *e)
{
	if ( m_iDisabled != 0)
		return false;

	if ( m_iNeedsLOS )
	{
		trace_t tr;
		UTIL_TraceLine(GetAbsOrigin(), e->WorldSpaceCenter(), MASK_SOLID_BRUSHONLY, e, COLLISION_GROUP_NONE, &tr);
		if ( tr.fraction < 1.0f )
			return false;
	}

	return true;
}

float CGravityWell::GetForceOnEntity(CBaseEntity *e)
{
	if ( !ShouldAffectEntity(e) )
		return 0.0f;

	float r_squared = (e->WorldSpaceCenter() - GetAbsOrigin()).LengthSqr();
	return sv_gravity_scale.GetFloat() * m_flStrength / r_squared;
}