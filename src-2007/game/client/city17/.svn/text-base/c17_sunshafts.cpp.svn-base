//===== Copyright © 2009, Gear Software, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//==================================================================//
 
#include "cbase.h"
#include "screenspaceeffects.h"
#include "rendertexture.h"
#include "model_types.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterialvar.h"
#include "cdll_client_int.h"
#include "materialsystem/itexture.h"
#include "keyvalues.h"
#include "ClientEffectPrecacheSystem.h"
 
#include "c17_sunshafts.h"
 
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ADD_SCREENSPACE_EFFECT( CSunShaftEffect, c17_sunshafts );
 
void CSunShaftEffect::Init( void ) 
{
	// Initialize render targets
	m_SunQuarterRes.InitRenderTarget( ScreenWidth()/4, ScreenHeight()/4, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_SEPARATE, false, "_rt_SunQuarterRes" );
	m_SunFullRes.InitRenderTarget( ScreenWidth(), ScreenHeight(), RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_SEPARATE, false, "_rt_SunFullRes" );
	
	// Initialize the sunshaft shader that we render our stenciled frame into.
	KeyValues *pVMTKeyValues = new KeyValues( "sun_shafts" );
	pVMTKeyValues->SetString( "$SUNBUFFER", "_rt_SunQuarterRes" );
	pVMTKeyValues->SetInt( "$LIGHTVECTOR", 1 );
	m_SunPassMaterial.Init( "__sunpass", TEXTURE_GROUP_CLIENT_EFFECTS, pVMTKeyValues );
	m_SunPassMaterial->Refresh();

	// This is used to convert m_SunFullRes into a texture we can use on a full screen quad
	KeyValues *pVMTKeyValues2 = new KeyValues( "UnlitGeneric" );
	pVMTKeyValues2->SetString( "$basetexture", "_rt_SunFullRes" );
	pVMTKeyValues2->SetInt( "$additive", 1 );
	m_SunPassFinalMaterial.Init( "__sunpassfinal", TEXTURE_GROUP_CLIENT_EFFECTS, pVMTKeyValues );
	m_SunPassFinalMaterial->Refresh();

	// Initialize the white overlay material to render our model with
	KeyValues *pVMTKeyValues3 = new KeyValues( "UnlitGeneric" );
	pVMTKeyValues3->SetString( "$basetexture", "vgui/black" );
	pVMTKeyValues3->SetInt( "$vertexalpha", 1 );
	pVMTKeyValues3->SetInt( "$model", 1 );
	m_BlackMaterial.Init( "__black", TEXTURE_GROUP_CLIENT_EFFECTS, pVMTKeyValues );
	m_BlackMaterial->Refresh();
}
 
void CSunShaftEffect::Shutdown( void )
{
	// Tell the materials we are done referencing them
	m_SunPassMaterial.Shutdown();
	m_SunPassFinalMaterial.Shutdown();
	
	m_SunQuarterRes.Shutdown();
	m_SunFullRes.Shutdown();
}
 
//-----------------------------------------------------------------------------
// Purpose: Render the effect
//-----------------------------------------------------------------------------
ConVar r_sunshafts( "r_sunshafts", "0", FCVAR_ARCHIVE );

void CSunShaftEffect::Render( int x, int y, int w, int h )
{
	if ( !r_sunshafts.GetBool() )
		return;

	// Don't bother rendering if we have nothing to render!
	if ( !m_vShaftEnts.Count() )
		return;
 
	// Grab the render context
	CMatRenderContextPtr pRenderContext( materials );
 
	pRenderContext->ClearColor4ub( 0, 0, 0, 255 );

	// Clear the stencil buffer in case someone dirtied it this frame
	pRenderContext->ClearStencilBufferRectangle( 0, 0, ScreenWidth(), ScreenHeight(), 0 );
 
	// Iterate over our registered entities and add them to the light buffer
	for ( int i=0; i < m_vShaftEnts.Count(); i++ )
	{
		RenderLightToTexture( i, pRenderContext );
	}
 
	// Setup the renderer to only draw where the stencil is not 1
	pRenderContext->SetStencilEnable( true );
	pRenderContext->SetStencilReferenceValue( 0 );
	pRenderContext->SetStencilTestMask( 1 );
	pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_EQUAL );
	pRenderContext->SetStencilPassOperation( STENCILOPERATION_ZERO );
	//City17: At this point, m_pSunFinalMaterial is just a simple circle with whatever light textures inside of it. Time to apply our stencil.
	pRenderContext->DrawScreenSpaceQuad( m_SunPassFinalMaterial );
	pRenderContext->SetStencilEnable( false );

	//Resize the light texture to one quarter.
	//Render light texture with sun_shafts pass.
	//Copy light texture back to full size

	// Finally draw our result onto the screen.
	pRenderContext->DrawScreenSpaceQuad( m_SunPassFinalMaterial );
}
 
void CSunShaftEffect::RenderToStencil( int idx, IMatRenderContext *pRenderContext )
{
	if ( idx < 0 || idx >= m_vShaftEnts.Count() )
		return;
 
	C_BaseEntity *pEnt = m_vShaftEnts[idx]->m_hEnt.Get();
	if ( !pEnt )
	{
		// We don't exist anymore, remove us!
		delete m_vShaftEnts[idx];
		m_vShaftEnts.Remove(idx);
		return;
	}
 
	pRenderContext->SetStencilEnable( true );
	pRenderContext->SetStencilFailOperation( STENCILOPERATION_KEEP );
	pRenderContext->SetStencilZFailOperation( STENCILOPERATION_KEEP );
	pRenderContext->SetStencilPassOperation( STENCILOPERATION_REPLACE );
	pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_ALWAYS );
	pRenderContext->SetStencilWriteMask( 1 );
	pRenderContext->SetStencilReferenceValue( 1 );
 
	pRenderContext->DepthRange( 0.0f, 0.01f );
	render->SetBlend( 0 );
 
	modelrender->ForcedMaterialOverride( m_BlackMaterial );
		pEnt->DrawModel( STUDIO_RENDER );
	modelrender->ForcedMaterialOverride( NULL );
 
	render->SetBlend( 1 );
	pRenderContext->DepthRange( 0.0f, 1.0f );
 
	pRenderContext->SetStencilEnable( false );
}
 
void CSunShaftEffect::RenderLightToTexture( int idx, IMatRenderContext *pRenderContext )
{
	if ( idx < 0 || idx >= m_vShaftEnts.Count() )
		return;
 
	C_BaseEntity *pEnt = m_vShaftEnts[idx]->m_hEnt.Get();
	if ( !pEnt )
	{
		// We don't exist anymore, remove us!
		delete m_vShaftEnts[idx];
		m_vShaftEnts.Remove(idx);
		return;
	}
 
	pRenderContext->PushRenderTargetAndViewport( m_SunFullRes );
 
	modelrender->SuppressEngineLighting( true );
 
	pEnt->DrawModel( STUDIO_RENDER );

	modelrender->SuppressEngineLighting( false );
 
	pRenderContext->PopRenderTargetAndViewport();
}	
 
void CSunShaftEffect::RegisterEnt( EHANDLE hEnt )
{
	// Don't add duplicates
	if ( FindShaftEnt(hEnt) != -1 || !hEnt.Get() )
		return;
 
	sShaftEnt *newEnt = new sShaftEnt;
	newEnt->m_hEnt = hEnt;
	m_vShaftEnts.AddToTail( newEnt );
}
 
void CSunShaftEffect::DeregisterEnt( EHANDLE hEnt )
{
	int idx = FindShaftEnt(hEnt);
	if ( idx == -1 )
		return;
 
	delete m_vShaftEnts[idx];
	m_vShaftEnts.Remove( idx );
}
 
int CSunShaftEffect::FindShaftEnt( EHANDLE hEnt )
{
	for ( int i=0; i < m_vShaftEnts.Count(); i++ )
	{
		if ( m_vShaftEnts[i]->m_hEnt == hEnt )
			return i;
	}
 
	return -1;
}