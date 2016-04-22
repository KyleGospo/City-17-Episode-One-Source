//============ Copyright © 2009, Team Gear, All rights reserved. ==============//
//
// Purpose: DOF Blur for ironsights.
//
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "hud_numericdisplay.h"
#include "iclientmode.h"
#include "c_basehlplayer.h"
#include "vguimatsurface/IMatSystemSurface.h"
#include "materialsystem/IMaterial.h"
#include "materialsystem/IMesh.h"
#include "materialsystem/imaterialvar.h"

#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/AnimationController.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Draws the Ironight Blur
//-----------------------------------------------------------------------------
class CHudBlur : public vgui::Panel, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudBlur, vgui::Panel );

public:
	CHudBlur( const char *pElementName );
	
	bool	ShouldDraw( void );
	void	Init( void );
	void	LevelInit( void );

protected:
	virtual void ApplySchemeSettings(vgui::IScheme *scheme);
	virtual void Paint( void );

private:
	bool	m_bPainted;

	CMaterialReference m_BlurMaterial;
	CMaterialReference m_LowHealthMaterial;
	CMaterialReference m_ConstMaterial;
};

DECLARE_HUDELEMENT( CHudBlur );

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudBlur::CHudBlur( const char *pElementName ) : CHudElement(pElementName), BaseClass(NULL, "HudBlur")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
	
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

//-----------------------------------------------------------------------------
// Purpose: standard hud element init function
//-----------------------------------------------------------------------------
void CHudBlur::Init( void )
{
	m_bPainted = false;
	m_BlurMaterial.Init( "vgui/hudblur", TEXTURE_GROUP_VGUI );
	m_LowHealthMaterial.Init( "vgui/lowhealth", TEXTURE_GROUP_VGUI );
	m_ConstMaterial.Init( "vgui/vignette", TEXTURE_GROUP_VGUI );
}

//-----------------------------------------------------------------------------
// Purpose: standard hud element init function
//-----------------------------------------------------------------------------
void CHudBlur::LevelInit( void )
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: sets scheme colors
//-----------------------------------------------------------------------------
void CHudBlur::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings(scheme);

	SetPaintBackgroundEnabled(false);
	SetPaintBorderEnabled(false);
	//SetFgColor(scheme->GetColor("ZoomReticleColor", GetFgColor()));

	int screenWide, screenTall;
	GetHudSize(screenWide, screenTall);
	SetBounds(0, 0, screenWide, screenTall);
	//SetZPos(99);
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudBlur::ShouldDraw( void )
{
	bool bNeedsDraw = false;

	if( engine->GetDXSupportLevel() < 80 )
		return false;

	C_BaseHLPlayer *pPlayer = dynamic_cast<C_BaseHLPlayer *>(C_BasePlayer::GetLocalPlayer());
	if( !pPlayer )
		return false;

	if ( pPlayer->GetHealth() > 0 )
	{
		// need to paint
		bNeedsDraw = true;
	}
	else if ( m_bPainted )
	{
		// keep painting until state is finished
		bNeedsDraw = true;
	}

	return ( bNeedsDraw && CHudElement::ShouldDraw() );
}

#define	BLUR_FADE_TIME	0.4f
//-----------------------------------------------------------------------------
// Purpose: draws the blur effect
//-----------------------------------------------------------------------------
void CHudBlur::Paint( void )
{
	m_bPainted = false;

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if( !pPlayer )
		return;

	C_BaseCombatWeapon *wpn = pPlayer->GetActiveWeapon();
	
	// draw blur circles
	int wide, tall;
	GetSize(wide, tall);

	Color col = GetFgColor();
	col[3] = 255;

	surface()->DrawSetColor( col );

	// draw the darkened edges, with a rotated texture in the four corners
	CMatRenderContextPtr pRenderContext( materials );
	if( pPlayer->GetHealth() <= 20 || pPlayer->InSmokeVolume() )
	{
		pRenderContext->Bind( m_LowHealthMaterial );
	}
	else if( wpn && wpn->IsIronsighted() )
	{
		pRenderContext->Bind( m_BlurMaterial );
	}
	else
	{
		pRenderContext->Bind( m_ConstMaterial );
	}

	IMesh *pMesh = pRenderContext->GetDynamicMesh( true, NULL, NULL, NULL );

	float x0 = 0.0f, x1 = wide / 2.0f, x2 = wide;
	float y0 = 0.0f, y1 = tall / 2.0f, y2 = tall;

	float uv1 = 1.0f - (1.0f / 255.0f);
	float uv2 = 0.0f + (1.0f / 255.0f);

	struct coord_t
	{
		float x, y;
		float u, v;
	};
	coord_t coords[16] = 
	{
		// top-left
		{ x0, y0, uv1, uv2 },
		{ x1, y0, uv2, uv2 },
		{ x1, y1, uv2, uv1 },
		{ x0, y1, uv1, uv1 },

		// top-right
		{ x1, y0, uv2, uv2 },
		{ x2, y0, uv1, uv2 },
		{ x2, y1, uv1, uv1 },
		{ x1, y1, uv2, uv1 },

		// bottom-right
		{ x1, y1, uv2, uv1 },
		{ x2, y1, uv1, uv1 },
		{ x2, y2, uv1, uv2 },
		{ x1, y2, uv2, uv2 },

		// bottom-left
		{ x0, y1, uv1, uv1 },
		{ x1, y1, uv2, uv1 },
		{ x1, y2, uv2, uv2 },
		{ x0, y2, uv1, uv2 },
	};

	CMeshBuilder meshBuilder;
	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 4 );

	for (int i = 0; i < 16; i++)
	{
		meshBuilder.Color4f( 0.0, 0.0, 0.0, 1.0 );
		meshBuilder.TexCoord2f( 0, coords[i].u, coords[i].v );
		meshBuilder.Position3f( coords[i].x, coords[i].y, 0.0f );
		meshBuilder.AdvanceVertex();
	}

	meshBuilder.End();
	pMesh->Draw();

	m_bPainted = true;
}