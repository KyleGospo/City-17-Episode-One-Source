//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "cbase.h"
#include "hud.h"
#include "in_buttons.h"
#include "beamdraw.h"
#include "c_weapon__stubs.h"
#include "ClientEffectPrecacheSystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CLIENTEFFECT_REGISTER_BEGIN( PrecacheEffectGravityGun )
CLIENTEFFECT_MATERIAL( "sprites/physbeam" )
CLIENTEFFECT_REGISTER_END()

class C_BeamQuadratic : public CDefaultClientRenderable
{
public:

	C_BeamQuadratic();

	void Update( C_BaseEntity *pOwner );

	virtual const Vector& GetRenderOrigin( void ) { return m_worldPosition; }
	virtual const QAngle& GetRenderAngles( void ) { return vec3_angle; }
	virtual bool ShouldDraw( void ) { return true; }
	virtual bool IsTransparent( void ) { return true; }
	virtual bool ShouldReceiveProjectedTextures( int flags ) { return false; }
	virtual int	DrawModel( int flags );

	virtual void GetRenderBounds( Vector& mins, Vector& maxs )
	{
		mins.Init( -32, -32, -32 );
		maxs.Init( 32, 32, 32 );
	}

	matrix3x4_t z;

	const matrix3x4_t & RenderableToWorldTransform()
	{
		return z;
	}

	C_BaseEntity *m_pOwner;
	Vector m_targetPosition;
	Vector m_worldPosition;
	int	m_active;
	int	m_glueTouching;
	int	m_viewModelIndex;
};

class C_WeaponGravityGun : public C_BaseCombatWeapon
{
	DECLARE_CLASS( C_WeaponGravityGun, C_BaseCombatWeapon );
public:

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	C_WeaponGravityGun();

	int KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding );

	void OnDataChanged( DataUpdateType_t updateType );

private:

	C_WeaponGravityGun( const C_WeaponGravityGun & );

	C_BeamQuadratic	m_beam;
};

STUB_WEAPON_CLASS_IMPLEMENT( weapon_physgun, C_WeaponGravityGun );

IMPLEMENT_CLIENTCLASS_DT( C_WeaponGravityGun, DT_WeaponGravityGun, CWeaponGravityGun )

	RecvPropVector( RECVINFO_NAME( m_beam.m_targetPosition, m_targetPosition ) ),
	RecvPropVector( RECVINFO_NAME( m_beam.m_worldPosition, m_worldPosition ) ),
	RecvPropInt( RECVINFO_NAME( m_beam.m_active, m_active ) ),
	RecvPropInt( RECVINFO_NAME( m_beam.m_glueTouching, m_glueTouching ) ),
	RecvPropInt( RECVINFO_NAME( m_beam.m_viewModelIndex, m_viewModelIndex ) ),

END_RECV_TABLE()


C_BeamQuadratic::C_BeamQuadratic()
{
	m_pOwner = NULL;
	m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;
}

void C_BeamQuadratic::Update( C_BaseEntity *pOwner )
{
	m_pOwner = pOwner;
	if ( m_active )
	{
		if ( m_hRenderHandle == INVALID_CLIENT_RENDER_HANDLE )
		{
			ClientLeafSystem() -> AddRenderable( this, RENDER_GROUP_TRANSLUCENT_ENTITY );
		}
		else
		{
			ClientLeafSystem() -> RenderableChanged( m_hRenderHandle );
		}
	}
	else if ( !m_active && m_hRenderHandle != INVALID_CLIENT_RENDER_HANDLE )
	{
		ClientLeafSystem() -> RemoveRenderable( m_hRenderHandle );
		m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;
	}
}

int	C_BeamQuadratic::DrawModel( int )
{
	Vector points[3];
	QAngle tmpAngle;

	if ( !m_active )
		return 0;

	C_BaseEntity *pEnt = cl_entitylist->GetEnt( m_viewModelIndex );
	if ( !pEnt )
		return 0;
	pEnt->GetAttachment( 1, points[ 0 ], tmpAngle );

	points[ 1 ] = 0.5 * ( m_targetPosition + points[ 0 ] );
	
	points[ 2 ] = m_worldPosition;

	IMaterial *pMat = materials->FindMaterial( "sprites/physbeam", TEXTURE_GROUP_CLIENT_EFFECTS );
	Vector color;
	if ( m_glueTouching )
	{
		color.Init( 1, 0, 0 );
	}
	else
	{
		color.Init( 1, 1, 1 );
	}

	float scrollOffset = gpGlobals->curtime - ( int )gpGlobals->curtime;

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->Bind( pMat );

	DrawBeamQuadratic( points[ 0 ], points[ 1 ], points[ 2 ], 13, color, scrollOffset );

	return 1;
}

C_WeaponGravityGun::C_WeaponGravityGun()
{
}

int C_WeaponGravityGun::KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	if ( gHUD.m_iKeyBits & IN_ATTACK )
	{
		switch ( keynum )
		{
		case MOUSE_WHEEL_UP:
			gHUD.m_iKeyBits |= IN_WEAPON1 ;
			return 0;

		case MOUSE_WHEEL_DOWN:
			gHUD.m_iKeyBits |= IN_WEAPON2 ;
			return 0;
		}
	}

	return BaseClass::KeyInput( down, keynum, pszCurrentBinding );
}

void C_WeaponGravityGun::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
	m_beam.Update( this );
}