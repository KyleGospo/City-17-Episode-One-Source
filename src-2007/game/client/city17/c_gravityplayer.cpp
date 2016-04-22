#include "cbase.h"
#include "c_gravityplayer.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CGravityPlayer
	#undef CGravityPlayer // otherwise this screws with class definitions
#endif

IMPLEMENT_CLIENTCLASS_DT(C_GravityPlayer, DT_GravityPlayer, CGravityPlayer)	
END_RECV_TABLE()

BEGIN_PREDICTION_DATA( C_GravityPlayer )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( player, C_GravityPlayer );

C_GravityPlayer::C_GravityPlayer()
{
	
}

void C_GravityPlayer::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
}

void C_GravityPlayer::CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov )
{
	UpdateVerticalDependentValues();
	BaseClass::CalcView(eyeOrigin, eyeAngles, zNear, zFar, fov);
}