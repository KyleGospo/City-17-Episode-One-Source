#include "cbase.h" 
#include "hud.h" 
#include "hud_macros.h" 
#include "c_baseplayer.h" 
#include "c17_hud_health.h" 
#include "iclientmode.h" 
#include "vgui/ISurface.h"
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/AnimationController.h>

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT( CHudHealth );
DECLARE_HUD_MESSAGE( CHudHealth, Damage );

# define HEALTH_INIT 80 

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudHealth::CHudHealth (const char * pElementName) : 
	CHudElement (pElementName), BaseClass (NULL, "C17HudHealth")
{
	vgui:: Panel * pParent = g_pClientMode-> GetViewport ();
	SetParent (pParent);

	SetHiddenBits (HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

void CHudHealth::SetBarTexture(const char *barTextureName)
{
	if( barTextureName != NULL && barTextureName[0] )
	{
		Q_strncpy( m_szBarTextureName, barTextureName, 64 );
		m_iBarTextureId = surface()->DrawGetTextureId( barTextureName );
		if( m_iBarTextureId == -1 )
		{
			m_iBarTextureId = surface()->CreateNewTextureID();
			surface()->DrawSetTextureFile( m_iBarTextureId, barTextureName, false, true );
		}
	}
	else
	{
		memset( m_szBarTextureName, 0, sizeof( m_szBarTextureName ) );
		m_iBarTextureId = -1;
	}
}

void CHudHealth::SetTopTexture(const char *topTextureName)
{
	if( topTextureName != NULL && topTextureName[0] )
	{
		Q_strncpy( m_szTopTextureName, topTextureName, 64 );
		m_iTopTextureId = surface()->DrawGetTextureId( topTextureName );
		if( m_iTopTextureId == -1 )
		{
			m_iTopTextureId = surface()->CreateNewTextureID();
			surface()->DrawSetTextureFile( m_iTopTextureId, topTextureName, false, true );
		}
	}
	else
	{
		memset( m_szTopTextureName, 0, sizeof( m_szTopTextureName ) );
		m_iTopTextureId = -1;
	}
}

void CHudHealth::SetBottomTexture(const char *bottomTextureName)
{
	if( bottomTextureName != NULL && bottomTextureName[0] )
	{
		Q_strncpy( m_szBottomTextureName, bottomTextureName, 64 );
		m_iBottomTextureId = surface()->DrawGetTextureId( bottomTextureName );
		if( m_iBottomTextureId == -1 )
		{
			m_iBottomTextureId = surface()->CreateNewTextureID();
			surface()->DrawSetTextureFile( m_iBottomTextureId, bottomTextureName, false, true );
		}
	}
	else
	{
		memset( m_szBottomTextureName, 0, sizeof( m_szBottomTextureName ) );
		m_iBottomTextureId = -1;
	}
}

void CHudHealth::SetGlowTexture(const char *glowTextureName)
{
	if( glowTextureName != NULL && glowTextureName[0] )
	{
		Q_strncpy( m_szGlowTextureName, glowTextureName, 64 );
		m_iGlowTextureId = surface()->DrawGetTextureId( glowTextureName );
		if( m_iGlowTextureId == -1 )
		{
			m_iGlowTextureId = surface()->CreateNewTextureID();
			surface()->DrawSetTextureFile( m_iGlowTextureId, glowTextureName, false, true );
		}
	}
	else
	{
		memset( m_szGlowTextureName, 0, sizeof( m_szGlowTextureName ) );
		m_iGlowTextureId = -1;
	}
}

//-----------------------------------------------------------------------------
// Purpose: sets scheme colors
//-----------------------------------------------------------------------------
void CHudHealth::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings(scheme);

	SetPaintBackgroundEnabled(false);
	SetPaintBorderEnabled(false);
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudHealth::Init()
{
	HOOK_HUD_MESSAGE( CHudHealth, Damage );

	m_flHealthSize = HEALTH_INIT;
	m_bitsDamage	= 0;
	m_nHealthLow = -1;
	m_iHealthGlowAlpha = 0;

	SetBarTexture( "vgui/hud/hud_health_orangebar_divider" );
	SetTopTexture( "vgui/hud/hud_health_orangebar" );
	SetBottomTexture( "vgui/hud/hud_health_bar" );
	SetGlowTexture( "vgui/hud/hud_health_bar_death" );
}

//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------
void CHudHealth::Reset( void )
{
	Init();
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudHealth::OnThink( void )
{
	float newHealth = 0;
	C_BasePlayer * local = C_BasePlayer:: GetLocalPlayer ();

	if (!local)
		return;

	// Never below zero 
	newHealth = max(local->GetHealth(), 0);

	// DevMsg("Sheild at is at: %f\n",newShield);
	// Only update the fade if we've changed health
	if (newHealth == m_flHealthSize)
	{
		return;
	}

	m_flHealthSize = newHealth;

	if ( m_flHealthSize >= 11 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17HealthIncreasedAbove20");
	}
	else if ( m_flHealthSize > 0 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17HealthIncreasedBelow20");
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17HealthLow");
	}
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudHealth::ShouldDraw( void )
{
	bool bNeedsDraw = ( GetAlpha() > 0 );

	return ( bNeedsDraw && CHudElement::ShouldDraw() );
}

//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------
void CHudHealth::Paint()
{
	if( m_iGlowTextureId != -1 )
	{
		surface()->DrawSetTexture( m_iGlowTextureId );
		surface()->DrawSetColor( 255, 255, 255, m_iHealthGlowAlpha );
		surface()->DrawTexturedRect( m_flImageOffsetX, m_flImageOffsetY, m_flImageWidth, m_flImageHeight );
	}

	if( m_iBottomTextureId != -1 )
	{
		surface()->DrawSetTexture( m_iBottomTextureId );
		surface()->DrawSetColor( 255, 255, 255, 255 );
		surface()->DrawTexturedRect( m_flImageOffsetX, m_flImageOffsetY, m_flImageWidth, m_flImageHeight );
	}

	if( m_iTopTextureId != -1 )
	{
		surface()->DrawSetTexture( m_iTopTextureId );
		surface()->DrawSetColor( 255, 255, 255, 255 );
		surface()->DrawTexturedRect( m_flImageOffsetX, m_flImageOffsetY, m_flImageWidth, m_flImageHeight );
	}

	// Get bar chunks
	int chunkCount = m_flBarWidth / (m_flBarChunkWidth + m_flBarChunkGap);
	int enabledChunks = (int)((float)chunkCount * (m_flHealthSize / 100.0f) + 0.5f );

	// Draw the suit power bar
	surface()->DrawSetColor (m_HealthColor);

	int xpos = m_flBarOffsetX, ypos = m_flBarOffsetY;

	for (int i = 0; i < enabledChunks; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}

	// Draw the exhausted portion of the bar.
	surface()->DrawSetColor(Color(m_HealthColor [0], m_HealthColor [1], m_HealthColor [2], m_iHealthDisabledAlpha));

	for (int i = enabledChunks; i < chunkCount; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + 19.0f /*Height*/);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}

	if( m_iBarTextureId != -1 )
	{
		surface()->DrawSetTexture( m_iBarTextureId );
		surface()->DrawSetColor( 255, 255, 255, 255 );
		surface()->DrawTexturedRect( m_flImageOffsetX, m_flImageOffsetY, m_flImageWidth, m_flImageHeight );
	}

	// Draw our name
	//surface()->DrawSetTextFont(m_hTextFont);
	//surface()->DrawSetTextColor(m_HealthColor);
	//surface()->DrawSetTextPos(text_xpos, text_ypos);

	//wchar_t *tempString = vgui::localize()->Find("#Valve_Hud_AUX_POWER");
	//surface()->DrawPrintText(L"HEALTH", wcslen(L"HEALTH"));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::MsgFunc_Damage( bf_read &msg )
{

	int armor = msg.ReadByte();	// armor
	int damageTaken = msg.ReadByte();	// health
	long bitsDamage = msg.ReadLong(); // damage bits
	bitsDamage; // variable still sent but not used

	Vector vecFrom;

	vecFrom.x = msg.ReadBitCoord();
	vecFrom.y = msg.ReadBitCoord();
	vecFrom.z = msg.ReadBitCoord();

	// Actually took damage?
	if ( damageTaken > 0 || armor > 0 )
	{
		if ( damageTaken > 0 )
		{
			// start the animation
			if( m_flHealthSize > 20 )
			{
				g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17HealthDamageTaken");
			}
		}
	}
}