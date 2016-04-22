#include "cbase.h" 
#include "hud.h" 
#include "hud_macros.h" 
#include "c_baseplayer.h" 
#include "c17_hud_battery.h" 
#include "iclientmode.h" 
#include "vgui/ISurface.h"
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/AnimationController.h>

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT( CHudBattery );
DECLARE_HUD_MESSAGE( CHudBattery, Battery );

# define BATTERY_INIT 80 

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudBattery::CHudBattery (const char * pElementName) : 
	CHudElement (pElementName), BaseClass (NULL, "C17HudBattery")
{
	vgui:: Panel * pParent = g_pClientMode-> GetViewport ();
	SetParent (pParent);

	SetHiddenBits ( HIDEHUD_HEALTH | HIDEHUD_NEEDSUIT | HIDEHUD_PLAYERDEAD );

	/*ImagePanel* m_pBackgroundImage = new ImagePanel(this, "BatteryBackgroundImage");
	if( m_pBackgroundImage )
	{
		m_pBackgroundImage->SetPos( 0, 0 );
		m_pBackgroundImage->SetSize( 512, 128 );
		m_pBackgroundImage->SetImage(scheme()->GetImage("hud/hud_battery_bar", false));
	}

	ImagePanel* m_pBarImage = new ImagePanel(this, "BatteryBarImage");
	if( m_pBarImage )
	{
		m_pBarImage->SetPos( 0, 0 );
		m_pBarImage->SetSize( 512, 128 );
		m_pBarImage->SetImage(scheme()->GetImage("hud/hud_battery_orangebar", false));
	}*/
}

void CHudBattery::SetTopTexture(const char *topTextureName)
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

void CHudBattery::SetBottomTexture(const char *bottomTextureName)
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

/*void CHudBattery::SetGlowTexture(const char *glowTextureName)
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
}*/

//-----------------------------------------------------------------------------
// Purpose: sets scheme colors
//-----------------------------------------------------------------------------
void CHudBattery::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings(scheme);

	SetPaintBackgroundEnabled(false);
	SetPaintBorderEnabled(false);
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudBattery::Init()
{
	HOOK_HUD_MESSAGE( CHudBattery, Battery);

	m_flBatterySize = BATTERY_INIT;
	m_iBatteryGlowAlpha = 0;
	newBattery = 0;
	SetAlpha(0);
	SetTopTexture( "vgui/hud/hud_armor_bar" );
	SetBottomTexture( "vgui/hud/hud_armor_bg" );
	//SetGlowTexture( "vgui/hud/hud_battery_bar_death" );

	//int img_xpos = m_flImageInsetX, img_ypos = m_flImageInsetY;

	/*ImagePanel* imagePanel = new ImagePanel(this, "BatteryCross");
	imagePanel->SetPos( img_xpos, img_ypos );
	imagePanel->SetImage(scheme()->GetImage("batterycross", false));*/
	//SetBgColor (Color (0,0,0,0));
}

//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------
void CHudBattery::Reset( void )
{
	Init();
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudBattery::OnThink( void )
{
	float newBattery = 0;
	C_BasePlayer * local = C_BasePlayer:: GetLocalPlayer ();

	if (!local)
		return;

	// DevMsg("Sheild at is at: %f\n",newShield);
	// Only update the fade if we've changed battery
	if (newBattery == m_flBatterySize)
	{
		return;
	}

	m_flBatterySize = newBattery;

	/*if ( m_flBatterySize >= 20 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17BatteryIncreasedAbove20");
	}
	else if ( m_flBatterySize > 0 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17BatteryIncreasedBelow20");
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17BatteryLow");
	}*/
	
	if( m_flBatterySize == 0 && GetAlpha() == 100 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17BatteryZero");
	}
	else if( m_flBatterySize > 0 && GetAlpha() == 0 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("C17BatteryAboveZero");
	}
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudBattery::ShouldDraw( void )
{
	bool bNeedsDraw = ( GetAlpha() > 0 );

	return ( bNeedsDraw && CHudElement::ShouldDraw() );
}

//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------
void CHudBattery::Paint()
{
	/*if( m_iGlowTextureId != -1 )
	{
		surface()->DrawSetTexture( m_iGlowTextureId );
		surface()->DrawSetColor( 255, 255, 255, m_iBatteryGlowAlpha );
		surface()->DrawTexturedRect( m_flImageOffsetX, m_flImageOffsetY, m_flImageWidth, m_flImageHeight );
	}*/

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
		surface()->DrawTexturedRect( m_flBarImageOffsetX, m_flBarImageOffsetY, m_flBarImageWidth, m_flBarImageHeight );
	}

	// Get bar chunks
	int chunkCount = m_flBarWidth / (m_flBarChunkWidth + m_flBarChunkGap);
	int enabledChunks = (int)((float)chunkCount * (m_flBatterySize / 100.0f) + 0.5f );

	// Draw the suit power bar
	surface()->DrawSetColor (m_BatteryColor);

	int xpos = m_flBarOffsetX, ypos = m_flBarOffsetY;

	for (int i = 0; i < enabledChunks; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}

	// Draw the exhausted portion of the bar.
	surface()->DrawSetColor(Color(m_BatteryColor [0], m_BatteryColor [1], m_BatteryColor [2], m_iBatteryDisabledAlpha));

	for (int i = enabledChunks; i < chunkCount; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + 19.0f /*Height*/);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudBattery::MsgFunc_Battery( bf_read &msg )
{
	newBattery = max(msg.ReadShort(), 0);
}
