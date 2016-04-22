//================================ Copyright © 2009, Team Gear =============================//
//
// Purpose: City 17's achievements dialog sub panel. Built from code released to the VDC by lodle.
//
//==========================================================================================//
#include "cbase.h"
#include "c17_AchievementSubPanel.h"

AchievementSubPanel::AchievementSubPanel( Panel *parent, const char *name, const char *desc ) : PD_BasePanel( parent, name )
{
	m_pAchievementNameLabel = new vgui::Label(this, "NameLabel", "Achievement Name");
	m_pAchievementDescriptionLabel = new vgui::Label(this, "DescLabel", "Achievement Text");

	m_pAchievementNameLabel->SetMouseInputEnabled( false );
	m_pAchievementDescriptionLabel->SetMouseInputEnabled( false );

	Q_strncpy( m_szAchievementName, "Achievement Name", 512 );
	Q_strncpy( m_szAchievementDesc, "Achievement Text", 1024 );
	m_bNeedsUpdate = true;
}

void AchievementSubPanel::OnThink( void )
{
	if (m_bNeedsUpdate)
	{
		m_pAchievementNameLabel->SetText( m_szAchievementName );
		m_pAchievementDescriptionLabel->SetText( m_szAchievementDesc );

		m_pAchievementNameLabel->SetFgColor( Color( 0, 0, 0, 255 ) );
		m_pAchievementDescriptionLabel->SetFgColor( Color( 0, 0, 0, 255 ) );
	}
}

void AchievementSubPanel::PaintBackground( void )
{
	vgui::surface()->DrawSetTexture( vgui::surface()->DrawGetTextureId( "vgui/hud/achievement_subpanel" ) );
	//vgui::surface()->DrawSetColor( 255, 255, 255, 255 );
	vgui::surface()->DrawTexturedRect( 0, 0, 200, 150 );
}