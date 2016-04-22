#ifndef GRAVITY_PLAYER_H
#define GRAVITY_PLAYER_H
#pragma once

#include "hl2_player.h"

class CGravityPlayer : public CHL2_Player
{
public:
	DECLARE_CLASS( CGravityPlayer, CHL2_Player );

	CGravityPlayer();
	~CGravityPlayer();
	
	static CGravityPlayer *CreatePlayer( const char *className, edict_t *ed )
	{
		CGravityPlayer::s_PlayerEdict = ed;
		return (CGravityPlayer*)CreateEntityByName( className );
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void	Spawn();

	virtual void	SetDownVector(Vector v) { m_vecDown = v; UpdateVerticalDependentValues(); }
	Vector&			GetViewVector(bool ducking=false) { return ducking ? m_vecDuckingViewVector : m_vecStandingViewVector; }
	virtual const	QAngle &EyeAngles();
	void			RecalculateDownVector(Vector origin);
	void			UpdateVerticalDependentValues();
	float			GetWidth();
	float			GetHeight(bool ducked=false);

protected:
	virtual void	PreThink( void );
	virtual	void	PostThink( void );

private:
	Vector			m_vecStandingViewVector;
	Vector			m_vecDuckingViewVector;
};

inline CGravityPlayer *ToGravityPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CGravityPlayer*>( pEntity );
}

#endif