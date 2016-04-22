#include "cbase.h"
#include "gravity_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( player, CGravityPlayer );

BEGIN_DATADESC( CGravityPlayer )	
END_DATADESC()


IMPLEMENT_SERVERCLASS_ST(CGravityPlayer, DT_GravityPlayer)	
END_SEND_TABLE()


CGravityPlayer::CGravityPlayer()
{

}

CGravityPlayer::~CGravityPlayer()
{

}

void CGravityPlayer::Spawn()
{
	BaseClass::Spawn();
	RecalculateDownVector( GetAbsOrigin() );
	SetViewOffset( GetViewVector() );
}

void CGravityPlayer::PreThink( void )
{
	BaseClass::PreThink();

	UpdateVerticalDependentValues();
}

extern QAngle GetRotationForYaw(Vector upDir, vec_t yaw);

void CGravityPlayer::PostThink( void )
{
	BaseClass::PostThink();

//	this is here to correct the pitch changing with the mouse only...
//	but may as well do roll & yaw, as its easier than just doing pitch...
	QAngle rotatedAngles = GetRotationForYaw(GetUpVector(),LocalEyeAngles()[YAW]);
	SetAbsAngles(rotatedAngles);
}