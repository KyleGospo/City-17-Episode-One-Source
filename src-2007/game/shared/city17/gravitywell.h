#ifndef GRAVITYWELL_H
#define GRAVITYWELL_H
#ifdef _WIN32
#pragma once
#endif

#ifdef CLIENT_DLL
	#define CGravityWell C_GravityWell
#endif

class CGravityWell : public CBaseEntity
{
public:
	DECLARE_CLASS( CGravityWell, CBaseEntity );

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
#if !defined( CLIENT_DLL )
	DECLARE_DATADESC();
#endif
	CGravityWell();
	~CGravityWell();

	void	Spawn( void );
	float	GetStrength() { return m_flStrength; }
	void	SetStrength(float f) { m_flStrength = f; }

	bool	IsInactive() { return m_flStrength == 0.0f; }
	bool	IsAttractive() { return m_flStrength > 0.0f; }
	bool	IsRepulsive() { return m_flStrength < 0.0f; }

	virtual bool ShouldAffectEntity(CBaseEntity *e);
	virtual float GetForceOnEntity(CBaseEntity *e); // does NOT check ShouldAffectEntity first

	void Toggle( inputdata_t &inputdata );
	void Enable( inputdata_t &inputdata );
	void Disable( inputdata_t &inputdata );

	bool IsEnabled() { return m_iDisabled == 0; }

private:
	CNetworkVar(float, m_flStrength);
	CNetworkVar(int, m_iDisabled);
	CNetworkVar(int, m_iNeedsLOS);
};

extern CUtlVector<CGravityWell*> g_GravityWells;

#endif // GRAVITYWELL_H