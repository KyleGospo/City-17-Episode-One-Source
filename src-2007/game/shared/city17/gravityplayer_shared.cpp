#include "cbase.h"
#include "city17/gravitywell.h"

#ifdef CLIENT_DLL
	#include "city17/c_gravityplayer.h"
#else
	#include "city17/gravity_player.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define STANDING_VIEW_HEIGHT	64.0f
#define	DUCKING_VIEW_HEIGHT		28.0f

QAngle VectorAnglesRollNoYaw(Vector vecDir)
{

	QAngle upAngle;
	upAngle[YAW] = 0;
	
	if ( vecDir.x == 0 )
		upAngle[PITCH] = 0;//vecDir.y == 0 && vecDir.z < 0 ? 180 : 0;
	else
	{
		upAngle[PITCH] = vecDir.z >= 0 ? RAD2DEG(asin(vecDir.x)) : 180 - RAD2DEG(asin(vecDir.x));
		if ( upAngle[PITCH] < 0 )
			upAngle[PITCH] += 360;
	}

	if ( vecDir.y == 0 )
		upAngle[ROLL] = vecDir.x == 0 && vecDir.z < 0 ? 180 : 0;
	else
	{
		upAngle[ROLL] = vecDir.z >= 0 ? -RAD2DEG(asin(vecDir.y)) : 180 + RAD2DEG(asin(vecDir.y));
		if ( upAngle[ROLL] < 0 )
			upAngle[ROLL] += 360;
	}

//	VectorAngles(vecDir,upAngle);
//	upAngle[PITCH] += 90;

/*
	upAngle[PITCH] = (atan2(-vecDir[2], vecDir[0]) * 180 / M_PI) + 90;
	if (upAngle[PITCH] < 0)
		upAngle[PITCH] += 360;

	upAngle[ROLL] = (atan2(-vecDir[2], vecDir[1]) * 180 / M_PI);
	if (upAngle[ROLL] < 0)
		upAngle[ROLL] += 360;
*/
/*
	if ( vecDir.z >= 0 )
	{
		if ( vecDir.x >= 0 )
			upAngle[PITCH] = RAD2DEG(atan(vecDir.z/vecDir.x));
		else
			upAngle[PITCH] = 90 + RAD2DEG(atan(-vecDir.x/vecDir.z));

		if ( vecDir.y >= 0 )
			upAngle[ROLL] = RAD2DEG(atan(vecDir.z/vecDir.y));
		else
			upAngle[ROLL] = 90 + RAD2DEG(atan(-vecDir.y/vecDir.z));
	}
	else
	{
		if ( vecDir.x >= 0 )
			upAngle[PITCH] = 180 + RAD2DEG(atan(vecDir.z/vecDir.x));
		else
			upAngle[PITCH] = 360 - RAD2DEG(atan(-vecDir.z/vecDir.x));

		if ( vecDir.y >= 0 )
			upAngle[ROLL] = 180 + RAD2DEG(atan(vecDir.z/vecDir.y));
		else
			upAngle[ROLL] = 360 - RAD2DEG(atan(-vecDir.z/vecDir.y));
	}
*/
	return upAngle;
}

QAngle GetRotationForYaw(Vector upDir, vec_t yaw)
{
	matrix3x4_t parentToWorld, eyesToParent, eyesToWorld;
	
	QAngle yawOnly = QAngle(0,yaw,0);
	QAngle upAngle = VectorAnglesRollNoYaw(upDir);
	AngleMatrix(upAngle,parentToWorld);
	AngleMatrix(yawOnly, eyesToParent);
	ConcatTransforms(parentToWorld, eyesToParent, eyesToWorld);

	QAngle rotatedAngles;
	MatrixAngles( eyesToWorld, rotatedAngles );
	return rotatedAngles;
}

// although this is networked, can't hurt to have the client predict the value in case of interruption or w/e
extern void ReplaceComponent(Vector &a, Vector vecDir, float mag, bool normalizeDir=false);

#define TRACE_DOWN_DIST 96
void CGravityPlayer::RecalculateDownVector(Vector origin)
{
	Vector oldUpVector = GetUpVector();

	if ( GetGroundEntity() != NULL )
	{// for now only if on the ground
		Vector center = origin + GetUpVector();
		Vector forward, right;
		EyeVectors(&forward,&right,NULL);
		Vector offsets[5];
		offsets[0] = vec3_origin;
		offsets[1] = (forward - right) * GetWidth()/2;
		offsets[2] = (forward + right) * GetWidth()/2;
		offsets[3] = (-forward - right) * GetWidth()/2;
		offsets[4] = (-forward + right) * GetWidth()/2;
	
		trace_t trs[5];

		for ( int i=0; i<5; i++ )
			UTIL_TraceLine(origin+GetUpVector()+offsets[i],origin+GetDownVector()*TRACE_DOWN_DIST+offsets[i],MASK_PLAYERSOLID_BRUSHONLY,this,COLLISION_GROUP_PLAYER_MOVEMENT,&trs[i]);

		trace_t *tr = NULL;
		for ( int i=0; i<5; i++ )
		{
			if ( trs[i].DidHit() && (tr==NULL || trs[i].fraction > tr->fraction) )
			{
				tr = &trs[i];
				break;
			}
		}
		if ( tr != NULL )
			SetDownVector( -tr->plane.normal );
		
		//trace_t tr;
		//UTIL_TraceLine(origin+GetUpVector(),origin+GetDownVector()*32,MASK_PLAYERSOLID_BRUSHONLY,this,COLLISION_GROUP_PLAYER_MOVEMENT,&tr);
		//if ( tr.DidHit() )
		//	SetDownVector( -tr.plane.normal );
	}

/*
	Vector gravity = Vector(0,0,0);
	for ( int i=0; i<g_GravityWells.Count(); i++ )
		VectorAdd(gravity,g_GravityWells.Element(i)->GetForceOnEntity(this),gravity);
	float gravityStrength = VectorNormalize(gravity);
	
	if ( gravityStrength > 10.0f )
		SetDownVector(gravity);

	// should use gravity magnitude also...
	SetGravity(gravityStrength);
*/
/*
	if ( GetUpVector() != oldUpVector )
	{// add this difference to the punch angle, .. by calculating it as the difference between base up angles, with yaw added in so that pitch/roll are correct way round
		Vector newUpVector = GetUpVector();
		QAngle temp = VectorAnglesRollNoYaw(oldUpVector);
		temp[YAW] = LocalEyeAngles()[YAW];
		AngleVectors(temp,&oldUpVector);

		temp = VectorAnglesRollNoYaw(newUpVector);
		temp[YAW] = LocalEyeAngles()[YAW];
		AngleVectors(temp,&newUpVector);
		QAngle difference = VectorAnglesRollNoYaw(newUpVector) - VectorAnglesRollNoYaw(oldUpVector);
		
		ViewPunch(difference);
	}
*/
}

// all these values are dependent only on the down vector. no use networking them all, just predict like this.
void CGravityPlayer::UpdateVerticalDependentValues()
{
	m_vecStandingViewVector = GetUpVector() * STANDING_VIEW_HEIGHT;
	m_vecDuckingViewVector = GetUpVector() * DUCKING_VIEW_HEIGHT;


	QAngle rotatedAngles = GetRotationForYaw(GetUpVector(),LocalEyeAngles()[YAW]);
#ifdef CLIENT_DLL
	m_angRender = rotatedAngles;
#endif
	SetAbsAngles(rotatedAngles);
}

const QAngle &CGravityPlayer::EyeAngles( )
{
	// NOTE: Viewangles are measured *relative* to the parent's coordinate system
	CBaseEntity *pMoveParent = GetMoveParent();

	if ( !pMoveParent )
	{// not in a vehicle
		matrix3x4_t parentToWorld, eyesToParent, eyesToWorld;
		
		QAngle upAngle = VectorAnglesRollNoYaw(GetUpVector());
		//Msg("upAngle = %.2f %.2f %.2f\n",VectorExpand(upAngle));
		AngleMatrix(upAngle,parentToWorld);
		AngleMatrix( pl.v_angle, eyesToParent );
		ConcatTransforms( parentToWorld, eyesToParent, eyesToWorld );

		static QAngle angEyeWorld;
		MatrixAngles( eyesToWorld, angEyeWorld );
		return angEyeWorld;
	}

	// when in a vehicle...
	// FIXME: Cache off the angles?
	matrix3x4_t eyesToParent, eyesToWorld;
	AngleMatrix( pl.v_angle, eyesToParent );
	ConcatTransforms( pMoveParent->EntityToWorldTransform(), eyesToParent, eyesToWorld );

	static QAngle angEyeWorld;
	MatrixAngles( eyesToWorld, angEyeWorld );
	return angEyeWorld;
}

float CGravityPlayer::GetWidth()
{
	return 32;
}

float CGravityPlayer::GetHeight(bool ducked)
{
	return ducked ? 72 : 36;
}