// ******************************************************
//
// Purpose:
//		-	Connects the shader editor
//		-	Sends data from the main viewsetup
//		-	exposes client callbacks to shaders
// 
// ******************************************************

#include "cbase.h"
#include "client_factorylist.h"
#include "ShaderEditor/IVShaderEditor.h"
#include "ShaderEditor/SEdit_ModelRender.h"
#include "iviewrender.h"
#include "viewrender.h"
#include "view.h"
#include "view_shared.h"
#include "beamdraw.h"
#include "c_sun.h"
#include "c_effects.h"
#include "tier0/icommandline.h"

class CViewSetup_SEdit_Shared
{
public:
	CViewSetup_SEdit_Shared( const CViewSetup &o )
	{
		x = o.x;
		y = o.y;
		width = o.width;
		height = o.height;
		m_bOrtho = o.m_bOrtho;
		m_OrthoLeft = o.m_OrthoLeft;
		m_OrthoTop = o.m_OrthoTop;
		m_OrthoRight = o.m_OrthoRight;
		m_OrthoBottom = o.m_OrthoBottom;
		fov = o.fov;
		origin = o.origin;
		angles = o.angles;
		zNear = o.zNear;
		zFar = o.zFar;
		m_flAspectRatio = o.m_flAspectRatio;
		m_bOffCenter = o.m_bOffCenter;
		m_flOffCenterTop = o.m_flOffCenterTop;
		m_flOffCenterBottom = o.m_flOffCenterBottom;
		m_flOffCenterLeft = o.m_flOffCenterLeft;
		m_flOffCenterRight = o.m_flOffCenterRight;
	}
	int			x;
	int			y;
	int			width;
	int			height;
	bool		m_bOrtho;
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;
	float		fov;
	float		fovViewmodel;
	Vector		origin;
	QAngle		angles;
	float		zNear;
	float		zFar;
	float		zNearViewmodel;
	float		zFarViewmodel;
	bool		m_bRenderToSubrectOfLargerScreen;
	float		m_flAspectRatio;
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	bool		m_bDoBloomAndToneMapping;
	bool		m_bCacheFullSceneState;
};

#if SWARM_DLL
#define Editor_MainViewOrigin MainViewOrigin( 0 )
#define Editor_MainViewForward MainViewForward( 0 )
#else
#define Editor_MainViewOrigin MainViewOrigin()
#define Editor_MainViewForward MainViewForward()
#endif

ShaderEditorHandler __g_ShaderEditorSystem( "ShEditUpdate" );
ShaderEditorHandler *g_ShaderEditorSystem = &__g_ShaderEditorSystem;

CSysModule *shaderEditorModule = NULL;
IVShaderEditor *shaderEdit = NULL;

ShaderEditorHandler::ShaderEditorHandler( char const *name ) : CAutoGameSystemPerFrame( name )
{
	m_bReady = false;
}

ShaderEditorHandler::~ShaderEditorHandler()
{
}

const bool ShaderEditorHandler::IsReady()
{
	return m_bReady;
}

bool ShaderEditorHandler::Init()
{
	factorylist_t factories;
	FactoryList_Retrieve( factories );

	ConVarRef devEnabled( "developer", true );

	bool bCreateEditor = ( CommandLine() != NULL ) && ( CommandLine()->FindParm( "-shaderedit" ) != 0 );
	bool bShowPrimDebug = devEnabled.GetInt() != 0;
	//SEDIT_SKYMASK_MODE iEnableSkymask = SKYMASK_OFF;
	SEDIT_SKYMASK_MODE iEnableSkymask = SKYMASK_QUARTER;

#ifdef SHADEREDITOR_FORCE_ENABLED
	bCreateEditor = true;
	iEnableSkymask = SKYMASK_QUARTER;
#endif

	char modulePath[MAX_PATH*4];
	Q_snprintf( modulePath, sizeof( modulePath ), "%s/bin/shadereditor.dll\0", engine->GetGameDirectory() );
	shaderEditorModule = Sys_LoadModule( modulePath );
	if ( shaderEditorModule )
	{
		CreateInterfaceFn shaderEditorDLLFactory = Sys_GetFactory( shaderEditorModule );
		shaderEdit = shaderEditorDLLFactory ? ((IVShaderEditor *) shaderEditorDLLFactory( SHADEREDIT_INTERFACE_VERSION, NULL )) : NULL;

		if ( !shaderEdit )
			Warning( "Unable to pull IVShaderEditor interface.\n" );
		else if ( !shaderEdit->Init( factories.appSystemFactory, gpGlobals, sEditMRender,
				bCreateEditor, bShowPrimDebug, iEnableSkymask ) )
			Warning( "Cannot initialize IVShaderEditor.\n" );
	}
	else
		Warning( "Cannot load shadereditor.dll from %s!\n", modulePath );

	m_bReady = !!shaderEdit;
	RegisterCallbacks();
	return true;
}

void ShaderEditorHandler::Shutdown()
{
	if ( shaderEdit )
		shaderEdit->Shutdown();
	if ( shaderEditorModule )
		Sys_UnloadModule( shaderEditorModule );
}

void ShaderEditorHandler::Update( float frametime )
{
	if ( IsReady() )
		shaderEdit->OnFrame( frametime );
}

CThreadMutex m_Lock;

void ShaderEditorHandler::PreRender()
{
	if ( IsReady() && view )
	{
		// make sure the class matches
		const CViewSetup *v = view->GetPlayerViewSetup();
		CViewSetup_SEdit_Shared stableVSetup( *v );
		shaderEdit->OnPreRender( &stableVSetup );

		m_Lock.Lock();
		PrepareCallbackData();
		m_Lock.Unlock();
	}
}
void ShaderEditorHandler::PostRender()
{
}
void ShaderEditorHandler::UpdateSkymask( bool bCombineMode )
{
	if ( IsReady() )
		shaderEdit->OnUpdateSkymask( bCombineMode );
}
void ShaderEditorHandler::CustomPostRender()
{
	if ( IsReady() )
		shaderEdit->OnPostRender( true );
}

struct CallbackData_t
{
	void Reset()
	{
		sun_data.Init();
		sun_dir.Init();
		wind_dir.Init();
		fast_wind_dir.Init();
		wind_speed.Init();
		player_speed.Init();
	};
	Vector4D sun_data;
	Vector sun_dir;
	Vector wind_dir;
	Vector fast_wind_dir;
	Vector wind_speed;
	Vector4D player_speed;
};

static CallbackData_t clCallback_data;

void ShaderEditorHandler::PrepareCallbackData()
{
	clCallback_data.Reset();

	float flSunAmt_Goal = 0;
	static float s_flSunAmt_Last = 0;

	float flGust_Goal = 0;
	static float s_flGust_Last = 0;

	C_BaseEntity *pEnt = ClientEntityList().FirstBaseEntity();
	while ( pEnt )
	{
		if ( !Q_stricmp( pEnt->GetClassname(), "class C_Sun" ) )
		{
			C_Sun *pSun = ( C_Sun* )pEnt;
			Vector dir = pSun->m_vDirection;
			dir.NormalizeInPlace();

			Vector screen;

			if ( ScreenTransform( Editor_MainViewOrigin + dir * 512, screen ) )
				ScreenTransform( (Editor_MainViewOrigin - dir * 512), screen );

			screen = screen * Vector( 0.5f, -0.5f, 0 ) + Vector( 0.5f, 0.5f, 0 );

			Q_memcpy( clCallback_data.sun_data.Base(), screen.Base(), sizeof(float) * 2 );
			clCallback_data.sun_data[ 2 ] = DotProduct( dir, Editor_MainViewForward );
			clCallback_data.sun_dir = dir;

			trace_t tr;
			UTIL_TraceLine( Editor_MainViewOrigin, Editor_MainViewOrigin + dir * MAX_TRACE_LENGTH, MASK_SOLID, NULL, COLLISION_GROUP_DEBRIS, &tr );
			if ( !tr.DidHitWorld() )
				break;

			if ( tr.surface.flags & SURF_SKY )
				flSunAmt_Goal = 1;

			break;
		}
		pEnt = ClientEntityList().NextBaseEntity( pEnt );
	}

	pEnt = ClientEntityList().FirstBaseEntity();
	while ( pEnt )
	{
		if ( !Q_stricmp( pEnt->GetClassname(), "class C_EnvWind" ) )
		{
			C_EnvWind *pWind = ( C_EnvWind* )pEnt;

			QAngle windangle( 0, pWind->m_EnvWindShared.m_iInitialWindDir, 0 );
			QAngle fastwindangle( 0, pWind->m_EnvWindShared.m_iWindDir, 0 );
			Vector s_WindVector;
			Vector s_FastWindVector;
			AngleVectors( windangle, &s_WindVector );
			AngleVectors( fastwindangle, &s_FastWindVector );
			float small_windspeed = (pWind->m_EnvWindShared.m_iMinWind+pWind->m_EnvWindShared.m_iMaxWind)/2;
			float large_windspeed = (pWind->m_EnvWindShared.m_iMinGust+pWind->m_EnvWindShared.m_iMaxGust)/2;

			Vector winddir;
			winddir.x = s_WindVector.x;
			winddir.y = s_WindVector.y;
			winddir.z = s_WindVector.z;
			Vector fastwinddir;
			fastwinddir.x = s_FastWindVector.x;
			fastwinddir.y = s_FastWindVector.y;
			fastwinddir.z = s_FastWindVector.z;
			Vector windspeed;
			windspeed.x = small_windspeed;
			windspeed.y = large_windspeed;

			if( pWind->m_EnvWindShared.m_bGusting )
				flGust_Goal = 1;

			Q_memcpy( clCallback_data.wind_dir.Base(), winddir.Base(), sizeof(float) * 3 );
			Q_memcpy( clCallback_data.fast_wind_dir.Base(), fastwinddir.Base(), sizeof(float) * 3 );
			Q_memcpy( clCallback_data.wind_speed.Base(), windspeed.Base(), sizeof(float) * 3 );
			break;
		}
		pEnt = ClientEntityList().NextBaseEntity( pEnt );
	}

	if ( s_flSunAmt_Last != flSunAmt_Goal )
		s_flSunAmt_Last = Approach( flSunAmt_Goal, s_flSunAmt_Last, gpGlobals->frametime * ( (!!flSunAmt_Goal) ? 4.0f : 0.75f ) );

	clCallback_data.sun_data[ 3 ] = s_flSunAmt_Last;

	if ( s_flGust_Last != flGust_Goal )
		s_flGust_Last = Approach( flGust_Goal, s_flGust_Last, gpGlobals->frametime * 0.15f );

	clCallback_data.wind_speed[ 2 ] = s_flGust_Last;

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( pPlayer )
	{
		Vector velo = pPlayer->GetLocalVelocity();
		clCallback_data.player_speed[ 3 ] = velo.NormalizeInPlace();
		Q_memcpy( clCallback_data.player_speed.Base(), velo.Base(), sizeof(float) * 3 );
	}
}

pFnClCallback_Declare( ClCallback_SunData )
{
	m_Lock.Lock();
	Q_memcpy( pfl4, clCallback_data.sun_data.Base(), sizeof(float) * 4 );
	m_Lock.Unlock();
}

pFnClCallback_Declare( ClCallback_SunDirection )
{
	m_Lock.Lock();
	Q_memcpy( pfl4, clCallback_data.sun_dir.Base(), sizeof(float) * 3 );
	m_Lock.Unlock();
}

pFnClCallback_Declare( ClCallback_WindDirection )
{
	m_Lock.Lock();
	Q_memcpy( pfl4, clCallback_data.wind_dir.Base(), sizeof(float) * 3 );
	m_Lock.Unlock();
}

pFnClCallback_Declare( ClCallback_FastWindDirection )
{
	m_Lock.Lock();
	Q_memcpy( pfl4, clCallback_data.fast_wind_dir.Base(), sizeof(float) * 3 );
	m_Lock.Unlock();
}

pFnClCallback_Declare( ClCallback_WindSpeed )
{
	m_Lock.Lock();
	Q_memcpy( pfl4, clCallback_data.wind_speed.Base(), sizeof(float) * 3 );
	m_Lock.Unlock();
}

pFnClCallback_Declare( ClCallback_PlayerVelocity )
{
	m_Lock.Lock();
	Q_memcpy( pfl4, clCallback_data.player_speed.Base(), sizeof(float) * 4 );
	m_Lock.Unlock();
}

void ShaderEditorHandler::RegisterCallbacks()
{
	if ( !IsReady() )
		return;

	// 4 components max
	shaderEdit->RegisterClientCallback( "sun data", ClCallback_SunData, 4 );
	shaderEdit->RegisterClientCallback( "sun dir", ClCallback_SunDirection, 3 );
	shaderEdit->RegisterClientCallback( "wind dir", ClCallback_WindDirection, 3 );
	shaderEdit->RegisterClientCallback( "fast wind dir", ClCallback_FastWindDirection, 3 );
	shaderEdit->RegisterClientCallback( "wind speed", ClCallback_WindSpeed, 3 );
	shaderEdit->RegisterClientCallback( "local player velocity", ClCallback_PlayerVelocity, 4 );

	shaderEdit->LockClientCallbacks();
}
