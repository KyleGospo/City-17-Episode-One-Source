#if !defined( C_GRAVITYPLAYER_H )
#define C_GRAVITYPLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_basehlplayer.h"

class C_GravityPlayer : public C_BaseHLPlayer
{
public:
	DECLARE_CLASS( C_GravityPlayer, C_BaseHLPlayer );
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	C_GravityPlayer();

	virtual void	OnDataChanged( DataUpdateType_t updateType );
	virtual void	CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	void			RecalculateDownVector(Vector origin);
	void			SetDownVector(Vector v) { m_vecDown = v; UpdateVerticalDependentValues(); }

	Vector&			GetViewVector(bool ducking=false) { return ducking ? m_vecDuckingViewVector : m_vecStandingViewVector; }
	virtual const	QAngle &EyeAngles();
	virtual const	QAngle &GetRenderAngles() { return m_angRender; }
	void			UpdateVerticalDependentValues();
	float			GetWidth();
	float			GetHeight(bool ducked=false);

private:
	Vector			m_vecStandingViewVector;
	Vector			m_vecDuckingViewVector;

	QAngle			m_angRender;
};

#define CGravityPlayer C_GravityPlayer


inline CGravityPlayer *ToGravityPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CGravityPlayer*>( pEntity );
}

#endif