//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_sun.h"

//City17:
#include "view.h"
#include "view_scene.h"
#include "viewrender.h"
#include "materialsystem/imaterialvar.h"
#include "ScreenSpaceEffects.h"
#include "city17/c17_screeneffects.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar	r_post_sunshaft;
extern ConVar	r_drawsprites;

static void RecvProxy_HDRColorScale( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	C_Sun *pSun = ( C_Sun * )pStruct;

	pSun->m_Overlay.m_flHDRColorScale = pData->m_Value.m_Float;
	pSun->m_GlowOverlay.m_flHDRColorScale = pData->m_Value.m_Float;
}

IMPLEMENT_CLIENTCLASS_DT_NOBASE( C_Sun, DT_Sun, CSun )
	
	RecvPropInt( RECVINFO(m_clrRender), 0, RecvProxy_IntToColor32 ),
	RecvPropInt( RECVINFO(m_clrOverlay), 0, RecvProxy_IntToColor32 ),
	RecvPropVector( RECVINFO( m_vDirection ) ),
	RecvPropInt( RECVINFO( m_bOn ) ),
	RecvPropInt( RECVINFO( m_nSize ) ),
	RecvPropInt( RECVINFO( m_nOverlaySize ) ),
	RecvPropInt( RECVINFO( m_nMaterial ) ),
	RecvPropInt( RECVINFO( m_nOverlayMaterial ) ),
	RecvPropFloat("HDRColorScale", 0, SIZEOF_IGNORE, 0, RecvProxy_HDRColorScale),
	
END_RECV_TABLE()

C_Sun::C_Sun()
{
	m_Overlay.m_bDirectional = true;
	m_Overlay.m_bInSky = true;

	m_GlowOverlay.m_bDirectional = true;
	m_GlowOverlay.m_bInSky = true;
}

void C_Sun::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	// We have to do special setup on our colors because we're tinting an additive material.
	// If we don't have at least one component at full strength, the luminosity of the material
	// will change and that will cause the material to become more translucent  This would be incorrect
	// for the sun, which should always be completely opaque at its core.  Here, we renormalize the
	// components to make sure only hue is altered.

	float maxComponent = max ( m_clrRender->r, max ( m_clrRender->g, m_clrRender->b ) );

	Vector vOverlayColor;
	Vector vMainColor;

	// Re-normalize the color ranges
	if ( maxComponent <= 0.0f )
	{
		// This is an error, set to pure white
		vMainColor.Init( 1.0f, 1.0f, 1.0f );
	}
	else
	{
		vMainColor.x = m_clrRender->r / maxComponent;
		vMainColor.y = m_clrRender->g / maxComponent;
		vMainColor.z = m_clrRender->b / maxComponent;
	}
	
	// If we're non-zero, use the value (otherwise use the value we calculated above)
	if ( m_clrOverlay.r != 0 || m_clrOverlay.g != 0 || m_clrOverlay.b != 0 )
	{
		// Get our overlay color
		vOverlayColor.x = m_clrOverlay.r / 255.0f;
		vOverlayColor.y = m_clrOverlay.g / 255.0f;
		vOverlayColor.z = m_clrOverlay.b / 255.0f;
	}
	else
	{
		vOverlayColor = vMainColor;
	}

	// 
	// Setup the core overlay
	//

	m_Overlay.m_vDirection = m_vDirection;
	m_Overlay.m_nSprites = 1;

	m_Overlay.m_Sprites[0].m_vColor = vMainColor;
	m_Overlay.m_Sprites[0].m_flHorzSize = m_nSize;
	m_Overlay.m_Sprites[0].m_flVertSize = m_nSize;

	const model_t* pModel = (m_nMaterial != 0) ? modelinfo->GetModel( m_nMaterial ) : NULL;
	const char *pModelName = pModel ? modelinfo->GetModelName( pModel ) : "";
	m_Overlay.m_Sprites[0].m_pMaterial = materials->FindMaterial( pModelName, TEXTURE_GROUP_OTHER );
	m_Overlay.m_flProxyRadius = 0.05f; // about 1/20th of the screen

	//
	// Setup the external glow overlay
	//

	m_GlowOverlay.m_vDirection = m_vDirection;
	m_GlowOverlay.m_nSprites = 1;

	m_GlowOverlay.m_Sprites[0].m_vColor = vOverlayColor;
	m_GlowOverlay.m_Sprites[0].m_flHorzSize = m_nOverlaySize;
	m_GlowOverlay.m_Sprites[0].m_flVertSize = m_nOverlaySize;

	pModel = (m_nOverlayMaterial != 0) ? modelinfo->GetModel( m_nOverlayMaterial ) : NULL;
	pModelName = pModel ? modelinfo->GetModelName( pModel ) : "";
	m_GlowOverlay.m_Sprites[0].m_pMaterial = materials->FindMaterial( pModelName, TEXTURE_GROUP_OTHER );

	// This texture will fade away as the dot between camera and sun changes
	m_GlowOverlay.SetModulateByDot();
	m_GlowOverlay.m_flProxyRadius = 0.05f; // about 1/20th of the screen


	// Either activate or deactivate.
	if ( m_bOn )
	{
		m_Overlay.Activate();
		m_GlowOverlay.Activate();
	}
	else
	{
		m_Overlay.Deactivate();
		m_GlowOverlay.Deactivate();
	}
}

void C_SunGlowOverlay::Activate()
{
	CGlowOverlay::Activate();
}

void C_SunGlowOverlay::Deactivate()
{
	CGlowOverlay::Deactivate();
}

void C_SunGlowOverlay::Draw( bool bCacheFullSceneState )
{	
	// Get the vector to the sun.
	Vector vToGlow;

	if( m_bDirectional )
		vToGlow = m_vDirection;
	else
		vToGlow = m_vPos - CurrentViewOrigin();

	if( !r_drawsprites.GetBool() )
		return;

	VectorNormalize( vToGlow );

	//float flDot = vToGlow.Dot( CurrentViewForward() );
	float flDot = 0.0f;

	//Normal sun behavior. Check for occlusion.
	UpdateGlowObstruction( vToGlow, bCacheFullSceneState, true );
	//if( m_flGlowObstructionScale == 0 )
		//return;

	bool bWireframe = ShouldDrawInWireFrameMode() || (r_drawsprites.GetInt() == 2);
	
	CMatRenderContextPtr pRenderContext( materials );

	for( int iSprite=0; iSprite < m_nSprites; iSprite++ )
	{
		CGlowSprite *pSprite = &m_Sprites[iSprite];
 
		// Figure out the color and size to draw it.
		float flHorzSize, flVertSize;
		Vector vColor;
		CalcSpriteColorAndSize( flDot, pSprite, &flHorzSize, &flVertSize, &vColor );
	
		// If we're alpha'd out, then don't bother
		//if ( vColor.LengthSqr() < 0.00001f )
			//continue;
		
		// Setup the basis to draw the sprite.
		Vector vBasePt, vUp, vRight;
		CalcBasis( vToGlow, flHorzSize, flVertSize, vBasePt, vUp, vRight );

		//Get our diagonal radius
		//float radius = (vRight+vUp).Length();
		//if ( R_CullSphere( view->GetFrustum(), 5, &vBasePt, radius ) )
			//continue;

		// Get our material (deferred default load)
		if ( m_Sprites[iSprite].m_pMaterial == NULL )
		{
			m_Sprites[iSprite].m_pMaterial = materials->FindMaterial( "sprites/light_glow02_add_noz", TEXTURE_GROUP_CLIENT_EFFECTS );
		}

		Assert( m_Sprites[iSprite].m_pMaterial );
		static unsigned int		nHDRColorScaleCache = 0;
		IMaterialVar *pHDRColorScaleVar = m_Sprites[iSprite].m_pMaterial->FindVarFast( "$hdrcolorscale", &nHDRColorScaleCache );
		if( pHDRColorScaleVar )
		{
			pHDRColorScaleVar->SetFloatValue( m_flHDRColorScale );
		}
		IMaterialVar *pColorVar = m_Sprites[iSprite].m_pMaterial->FindVar( "$mutable_01", NULL );
		if( pColorVar )
		{
			pColorVar->SetVecValue( m_Sprites[iSprite].m_vColor.x, m_Sprites[iSprite].m_vColor.y, m_Sprites[iSprite].m_vColor.z, 1.0f );
		}

		// Draw the sprite.
		IMesh *pMesh = pRenderContext->GetDynamicMesh( false, 0, 0, m_Sprites[iSprite].m_pMaterial );

		CMeshBuilder builder;
		builder.Begin( pMesh, MATERIAL_QUADS, 1 );
		
		Vector vPt;
		
		vPt = vBasePt - vRight + vUp;
		builder.Position3fv( vPt.Base() );
		builder.Color4f( VectorExpand(vColor), 1 );
		builder.TexCoord2f( 0, 0, 1 );
		builder.AdvanceVertex();
		
		vPt = vBasePt + vRight + vUp;
		builder.Position3fv( vPt.Base() );
		builder.Color4f( VectorExpand(vColor), 1 );
		builder.TexCoord2f( 0, 1, 1 );
		builder.AdvanceVertex();
		
		vPt = vBasePt + vRight - vUp;
		builder.Position3fv( vPt.Base() );
		builder.Color4f( VectorExpand(vColor), 1 );
		builder.TexCoord2f( 0, 1, 0 );
		builder.AdvanceVertex();
		
		vPt = vBasePt - vRight - vUp;
		builder.Position3fv( vPt.Base() );
		builder.Color4f( VectorExpand(vColor), 1 );
		builder.TexCoord2f( 0, 0, 0 );
		builder.AdvanceVertex();
		
		builder.End( false, true );

		if( bWireframe )
		{
			IMaterial *pWireframeMaterial = materials->FindMaterial( "debug/debugwireframevertexcolor", TEXTURE_GROUP_OTHER );
			pRenderContext->Bind( pWireframeMaterial );
			
			// Draw the sprite.
			IMesh *pMesh = pRenderContext->GetDynamicMesh( false, 0, 0, pWireframeMaterial );
			
			CMeshBuilder builder;
			builder.Begin( pMesh, MATERIAL_QUADS, 1 );
			
			Vector vPt;
			
			vPt = vBasePt - vRight + vUp;
			builder.Position3fv( vPt.Base() );
			builder.Color3f( 1.0f, 0.0f, 0.0f );
			builder.AdvanceVertex();
			
			vPt = vBasePt + vRight + vUp;
			builder.Position3fv( vPt.Base() );
			builder.Color3f( 1.0f, 0.0f, 0.0f );
			builder.AdvanceVertex();
			
			vPt = vBasePt + vRight - vUp;
			builder.Position3fv( vPt.Base() );
			builder.Color3f( 1.0f, 0.0f, 0.0f );
			builder.AdvanceVertex();
			
			vPt = vBasePt - vRight - vUp;
			builder.Position3fv( vPt.Base() );
			builder.Color3f( 1.0f, 0.0f, 0.0f );
			builder.AdvanceVertex();
			
			builder.End( false, true );
		}
	}
}