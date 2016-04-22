//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "c_basehlcombatweapon.h"
#include "iviewrender_beams.h"
#include "beam_shared.h"
#include "c_weapon__stubs.h"
#include "ClientEffectPrecacheSystem.h"
#include "beamdraw.h"
#include "materialsystem/IMaterial.h"
	#include "iviewrender_beams.h"
	#include "beam_shared.h"
	#include "model_types.h"
	#include "c_te_effect_dispatch.h"
	#include "fx_quad.h"
	#include "fx.h"

	extern void DrawHalo( IMaterial* pMaterial, const Vector &source, float scale, float const *color, float flHDRColorScale );
	extern void FormatViewModelAttachment( Vector &vOrigin, bool bInverse );

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CLIENTEFFECT_REGISTER_BEGIN( PrecacheEffectStunbaton )
CLIENTEFFECT_MATERIAL( "effects/stunstick" )
CLIENTEFFECT_REGISTER_END()

#define	STUNSTICK_BEAM_MATERIAL		"sprites/lgtning.vmt"
#define STUNSTICK_GLOW_MATERIAL		"sprites/light_glow02_add.vmt"
#define STUNSTICK_GLOW_MATERIAL2	"effects/blueflare1.vmt"
#define STUNSTICK_GLOW_MATERIAL_NOZ	"sprites/light_glow02_add_noz.vmt"
#define CWeaponStunStick C_WeaponStunStick


class C_WeaponStunBaton : public C_BaseHLBludgeonWeapon
{
	DECLARE_CLASS( C_WeaponStunBaton, C_BaseHLBludgeonWeapon );
public:
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	int			WeaponMeleeAttack1Condition( float flDot, float flDist );

	virtual int				DrawModel( int flags );
	virtual void			ClientThink( void );
	virtual void			OnDataChanged( DataUpdateType_t updateType );
	virtual RenderGroup_t	GetRenderGroup( void );
	virtual void			ViewModelDrawn( C_BaseViewModel *pBaseViewModel );
	virtual void Precache();

		#define	NUM_BEAM_ATTACHMENTS	9

	struct stunstickBeamInfo_t
	{
		int IDs[2];		// 0 - top, 1 - bottom
	};

	stunstickBeamInfo_t		m_BeamAttachments[NUM_BEAM_ATTACHMENTS];	// Lookup for arc attachment points on the head of the stick
	int						m_BeamCenterAttachment;						// "Core" of the effect (center of the head)

	void	SetupAttachmentPoints( void );
	void	DrawFirstPersonEffects( void );
	void	DrawThirdPersonEffects( void );
	void	DrawEffects( void );
	bool	InSwing( void );

	bool	m_bSwungLastFrame;

	#define	FADE_DURATION	0.25f

	float	m_flFadeTime;

};


//-----------------------------------------------------------------------------
// Purpose: Get the attachment point on a viewmodel that a base weapon is using
//-----------------------------------------------------------------------------
bool UTIL_GetWeaponAttachment( C_BaseCombatWeapon *pWeapon, int attachmentID, Vector &absOrigin, QAngle &absAngles )
{
	// This is already correct in third-person
	if ( pWeapon && pWeapon->IsCarriedByLocalPlayer() == false )
	{
		return pWeapon->GetAttachment( attachmentID, absOrigin, absAngles );
	}

	// Otherwise we need to translate the attachment to the viewmodel's version and reformat it
	CBasePlayer *pOwner = ToBasePlayer( pWeapon->GetOwner() );
	
	if ( pOwner != NULL )
	{
		int ret = pOwner->GetViewModel()->GetAttachment( attachmentID, absOrigin, absAngles );
		FormatViewModelAttachment( absOrigin, true );

		return ret;
	}

	// Wasn't found
	return false;
}

#define	BEAM_ATTACH_CORE_NAME	"sparkrear"

//-----------------------------------------------------------------------------
// Purpose: Sets up the attachment point lookup for the model
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::SetupAttachmentPoints( void )
{
	// Setup points for both types of views
	if ( IsCarriedByLocalPlayer() )
	{
		const char *szBeamAttachNamesTop[NUM_BEAM_ATTACHMENTS] =
		{
			"spark1a","spark2a","spark3a","spark4a",
			"spark5a","spark6a","spark7a","spark8a",
			"spark9a",
		};

		const char *szBeamAttachNamesBottom[NUM_BEAM_ATTACHMENTS] =
		{
			"spark1b","spark2b","spark3b","spark4b",
			"spark5b","spark6b","spark7b","spark8b",
			"spark9b",
		};
		
		// Lookup and store all connections
		for ( int i = 0; i < NUM_BEAM_ATTACHMENTS; i++ )
		{
			m_BeamAttachments[i].IDs[0] = LookupAttachment( szBeamAttachNamesTop[i] );
			m_BeamAttachments[i].IDs[1] = LookupAttachment( szBeamAttachNamesBottom[i] );
		}

		// Setup the center beam point
		m_BeamCenterAttachment = LookupAttachment( BEAM_ATTACH_CORE_NAME );
	}
	else
	{
		// Setup the center beam point
		m_BeamCenterAttachment = 1;
	}
}


void C_WeaponStunBaton::Precache()
{
	BaseClass::Precache();

	PrecacheModel( STUNSTICK_BEAM_MATERIAL );
	PrecacheModel( STUNSTICK_GLOW_MATERIAL );
	PrecacheModel( STUNSTICK_GLOW_MATERIAL2 );
	PrecacheModel( STUNSTICK_GLOW_MATERIAL_NOZ );
}

//-----------------------------------------------------------------------------
// Purpose: Draws the stunstick model (with extra effects)
//-----------------------------------------------------------------------------
int C_WeaponStunBaton::DrawModel( int flags )
{
	if ( ShouldDraw() == false )
		return 0;

	// Only render these on the transparent pass
	if ( flags & STUDIO_TRANSPARENCY )
	{
		DrawEffects();
		return 1;
	}

	return BaseClass::DrawModel( flags );
}

//-----------------------------------------------------------------------------
// Purpose: Randomly adds extra effects
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::ClientThink( void )
{
	if ( InSwing() == false )
	{
		if ( m_bSwungLastFrame )
		{
			// Start fading
			m_flFadeTime = gpGlobals->curtime;
			m_bSwungLastFrame = false;
		}

		return;
	}

	// Remember if we were swinging last frame
	m_bSwungLastFrame = InSwing();

	if ( IsEffectActive( EF_NODRAW ) )
		return;

	if ( IsCarriedByLocalPlayer() )
	{
		// Update our effects
		if ( gpGlobals->frametime != 0.0f && ( random->RandomInt( 0, 3 ) == 0 ) )
		{		
			Vector	vecOrigin;
			QAngle	vecAngles;

			// Inner beams
			BeamInfo_t beamInfo;

			int attachment = random->RandomInt( 0, 15 );

			UTIL_GetWeaponAttachment( this, attachment, vecOrigin, vecAngles );
			::FormatViewModelAttachment( vecOrigin, false );

			CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
			CBaseEntity *pBeamEnt = pOwner->GetViewModel();

			beamInfo.m_vecStart = vec3_origin;
			beamInfo.m_pStartEnt= pBeamEnt;
			beamInfo.m_nStartAttachment = attachment;

			beamInfo.m_pEndEnt	= NULL;
			beamInfo.m_nEndAttachment = -1;
			beamInfo.m_vecEnd = vecOrigin + RandomVector( -8, 8 );

			beamInfo.m_pszModelName = STUNSTICK_BEAM_MATERIAL;
			beamInfo.m_flHaloScale = 0.0f;
			beamInfo.m_flLife = 0.05f;
			beamInfo.m_flWidth = random->RandomFloat( 1.0f, 2.0f );
			beamInfo.m_flEndWidth = 0;
			beamInfo.m_flFadeLength = 0.0f;
			beamInfo.m_flAmplitude = random->RandomFloat( 16, 32 );
			beamInfo.m_flBrightness = 255.0;
			beamInfo.m_flSpeed = 0.0;
			beamInfo.m_nStartFrame = 0.0;
			beamInfo.m_flFrameRate = 1.0f;
			beamInfo.m_flRed = 255.0f;;
			beamInfo.m_flGreen = 255.0f;
			beamInfo.m_flBlue = 255.0f;
			beamInfo.m_nSegments = 16;
			beamInfo.m_bRenderable = true;
			beamInfo.m_nFlags = 0;
			
			beams->CreateBeamEntPoint( beamInfo );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Starts the client-side version thinking
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
	if ( updateType == DATA_UPDATE_CREATED )
	{
		SetNextClientThink( CLIENT_THINK_ALWAYS );
		SetupAttachmentPoints();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Tells us we're always a translucent entity
//-----------------------------------------------------------------------------
RenderGroup_t C_WeaponStunBaton::GetRenderGroup( void )
{
	return RENDER_GROUP_TWOPASS;
}

//-----------------------------------------------------------------------------
// Purpose: Tells us we're always a translucent entity
//-----------------------------------------------------------------------------
bool C_WeaponStunBaton::InSwing( void )
{
	int activity = GetActivity();

	// FIXME: This is needed until the actual animation works
	if ( IsCarriedByLocalPlayer() == false )
		return true;

	// These are the swing activities this weapon can play
	if ( activity == GetPrimaryAttackActivity() || 
		 activity == GetSecondaryAttackActivity() ||
		 activity == ACT_VM_MISSCENTER ||
		 activity == ACT_VM_MISSCENTER2 )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Draw our special effects
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::DrawThirdPersonEffects( void )
	{
			CMatRenderContextPtr pRenderContext( materials );
	Vector	vecOrigin;
	QAngle	vecAngles;
	float	color[3];
	float	scale;

	IMaterial *pMaterial = materials->FindMaterial( STUNSTICK_GLOW_MATERIAL, NULL, false );
		pRenderContext->Bind( pMaterial );

	// Get bright when swung
	if ( InSwing() )
	{
		color[0] = color[1] = color[2] = 0.4f;
		scale = 22.0f;
	}
	else
	{
		color[0] = color[1] = color[2] = 0.1f;
		scale = 20.0f;
	}
	
	// Draw an all encompassing glow around the entire head
	UTIL_GetWeaponAttachment( this, m_BeamCenterAttachment, vecOrigin, vecAngles );
	DrawHalo( pMaterial, vecOrigin, scale, color );

	if ( InSwing() )
	{
		pMaterial = materials->FindMaterial( STUNSTICK_GLOW_MATERIAL2, NULL, false );
		pRenderContext->Bind( pMaterial );

		color[0] = color[1] = color[2] = random->RandomFloat( 0.6f, 0.8f );
		scale = random->RandomFloat( 4.0f, 6.0f );

		// Draw an all encompassing glow around the entire head
		UTIL_GetWeaponAttachment( this, m_BeamCenterAttachment, vecOrigin, vecAngles );
		DrawHalo( pMaterial, vecOrigin, scale, color );

		// Update our effects
		if ( gpGlobals->frametime != 0.0f && ( random->RandomInt( 0, 5 ) == 0 ) )
		{
			Vector	vecOrigin;
			QAngle	vecAngles;

			GetAttachment( 1, vecOrigin, vecAngles );

			Vector	vForward;
			AngleVectors( vecAngles, &vForward );

			Vector vEnd = vecOrigin - vForward * 1.0f;

			// Inner beams
			BeamInfo_t beamInfo;

			beamInfo.m_vecStart = vEnd;
			Vector	offset = RandomVector( -12, 8 );

			offset += Vector(4,4,4);
			beamInfo.m_vecEnd = vecOrigin + offset;

			beamInfo.m_pStartEnt= cl_entitylist->GetEnt( BEAMENT_ENTITY( entindex() ) );
			beamInfo.m_pEndEnt	= cl_entitylist->GetEnt( BEAMENT_ENTITY( entindex() ) );
			beamInfo.m_nStartAttachment = 1;
			beamInfo.m_nEndAttachment = -1;
			
			beamInfo.m_nType = TE_BEAMTESLA;
			beamInfo.m_pszModelName = STUNSTICK_BEAM_MATERIAL;
			beamInfo.m_flHaloScale = 0.0f;
			beamInfo.m_flLife = 0.01f;
			beamInfo.m_flWidth = random->RandomFloat( 1.0f, 3.0f );
			beamInfo.m_flEndWidth = 0;
			beamInfo.m_flFadeLength = 0.0f;
			beamInfo.m_flAmplitude = random->RandomFloat( 1, 2 );
			beamInfo.m_flBrightness = 255.0;
			beamInfo.m_flSpeed = 0.0;
			beamInfo.m_nStartFrame = 0.0;
			beamInfo.m_flFrameRate = 1.0f;
			beamInfo.m_flRed = 255.0f;;
			beamInfo.m_flGreen = 255.0f;
			beamInfo.m_flBlue = 255.0f;
			beamInfo.m_nSegments = 16;
			beamInfo.m_bRenderable = true;
			beamInfo.m_nFlags = FBEAM_SHADEOUT;
			
			beams->CreateBeamPoints( beamInfo );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Draw our special effects
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::DrawFirstPersonEffects( void )
{
	CMatRenderContextPtr pRenderContext( materials );
	Vector	vecOrigin;
	QAngle	vecAngles;
	float	color[3];
	float	scale;

	IMaterial *pMaterial = materials->FindMaterial( STUNSTICK_GLOW_MATERIAL_NOZ, NULL, false );
		pRenderContext->Bind( pMaterial );

	// Find where we are in the fade
	float fadeAmount = RemapValClamped( gpGlobals->curtime, m_flFadeTime, m_flFadeTime + FADE_DURATION, 1.0f, 0.1f );

	// Get bright when swung
	if ( InSwing() )
	{
		color[0] = color[1] = color[2] = 0.4f;
		scale = 22.0f;
	}
	else
	{
		color[0] = color[1] = color[2] = 0.4f * fadeAmount;
		scale = 20.0f;
	}
	
	if ( color[0] > 0.0f )
	{
		// Draw an all encompassing glow around the entire head
		UTIL_GetWeaponAttachment( this, m_BeamCenterAttachment, vecOrigin, vecAngles );
		DrawHalo( pMaterial, vecOrigin, scale, color );
	}

	// Draw bright points at each attachment location
	for ( int i = 0; i < (NUM_BEAM_ATTACHMENTS*2)+1; i++ )
	{
		if ( InSwing() )
		{
			color[0] = color[1] = color[2] = random->RandomFloat( 0.05f, 0.5f );
			scale = random->RandomFloat( 4.0f, 5.0f );
		}
		else
		{
			color[0] = color[1] = color[2] = random->RandomFloat( 0.05f, 0.5f ) * fadeAmount;
			scale = random->RandomFloat( 4.0f, 5.0f ) * fadeAmount;
		}

		if ( color[0] > 0.0f )
		{
			UTIL_GetWeaponAttachment( this, i, vecOrigin, vecAngles );
			DrawHalo( pMaterial, vecOrigin, scale, color );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Draw our special effects
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::DrawEffects( void )
{
	if ( IsCarriedByLocalPlayer() )
	{
		DrawFirstPersonEffects();
	}
	else
	{
		DrawThirdPersonEffects();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Viewmodel was drawn
//-----------------------------------------------------------------------------
void C_WeaponStunBaton::ViewModelDrawn( C_BaseViewModel *pBaseViewModel )
{
	// Don't bother when we're not deployed
	if ( IsWeaponVisible() )
	{
		// Do all our special effects
		DrawEffects();
	}

	BaseClass::ViewModelDrawn( pBaseViewModel );
}

//-----------------------------------------------------------------------------
// Purpose: Draw a cheap glow quad at our impact point (with sparks)
//-----------------------------------------------------------------------------
void StunstickImpactCallback( const CEffectData &data )
{
	float scale = random->RandomFloat( 16, 32 );

	FX_AddQuad( data.m_vOrigin, 
				data.m_vNormal, 
				scale,
				scale*2.0f,
				1.0f, 
				1.0f,
				0.0f,
				0.0f,
				random->RandomInt( 0, 360 ), 
				0,
				Vector( 1.0f, 1.0f, 1.0f ), 
				0.1f, 
				"sprites/light_glow02_add",
				0 );

	FX_Sparks( data.m_vOrigin, 1, 2, data.m_vNormal, 6, 64, 256 );
}

DECLARE_CLIENT_EFFECT( "StunstickImpact", StunstickImpactCallback );

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pData - 
//			*pStruct - 
//			*pOut - 
//-----------------------------------------------------------------------------
STUB_WEAPON_CLASS_IMPLEMENT( weapon_stunbaton, C_WeaponStunBaton );

IMPLEMENT_CLIENTCLASS_DT( C_WeaponStunBaton, DT_WeaponStunBaton, CWeaponStunBaton )
END_RECV_TABLE()