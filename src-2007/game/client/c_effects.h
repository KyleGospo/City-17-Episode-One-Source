//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef C_EFFECTS_H
#define C_EFFECTS_H
#ifdef _WIN32
#pragma once
#endif

#include "env_wind_shared.h"

// Draw rain effects.
void DrawPrecipitation();

//-----------------------------------------------------------------------------
// EnvWind - global wind info
//-----------------------------------------------------------------------------
class C_EnvWind : public C_BaseEntity
{
public:
	C_EnvWind();

	DECLARE_CLIENTCLASS();
	DECLARE_CLASS( C_EnvWind, C_BaseEntity );

	virtual void	OnDataChanged( DataUpdateType_t updateType );
	virtual bool	ShouldDraw( void ) { return false; }

	virtual void	ClientThink( );

	CEnvWindShared m_EnvWindShared;

private:
	C_EnvWind( const C_EnvWind & );
};


#endif // C_EFFECTS_H
