#if !defined HUD_HEALTH_H
#define HUD_HEALTH_H 

#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include "hud_numericdisplay.h"

//-----------------------------------------------------------------------------
// Purpose: Shows the health bar
//-----------------------------------------------------------------------------

class CHudHealth : public CHudElement, public vgui::Panel
{

	DECLARE_CLASS_SIMPLE(CHudHealth, vgui::Panel);

public:
	CHudHealth(const char * pElementName);

	void	SetBarTexture( const char *barTextureName );
	void	SetTopTexture( const char *topTextureName );
	void	SetBottomTexture( const char *bottomTextureName );
	void	SetGlowTexture( const char *glowTextureName );
	virtual void Init( void );
	virtual void Reset( void );
	virtual void OnThink( void );
	void MsgFunc_Damage( bf_read &msg );

protected:
	bool ShouldDraw( void );
	virtual void Paint();
	virtual void ApplySchemeSettings(vgui::IScheme *scheme);

private:
	CPanelAnimationVar(Color, m_HealthColor, "HealthColor", "0 0 0 0");
	CPanelAnimationVar(int, m_iHealthDisabledAlpha, "HealthDisabledAlpha", "255");
	CPanelAnimationVar(int, m_iHealthGlowAlpha, "HealthGlowAlpha", "0");
	//CPanelAnimationVarAliasType(float, m_flBarInsetX, "BarInsetX", "20", "proportional_float");
	//CPanelAnimationVarAliasType(float, m_flBarInsetY, "BarInsetY", "0", "proportional_float");
	//CPanelAnimationVarAliasType(float, m_flImageInsetX, "ImageInsetX", "0", "proportional_float");
	//CPanelAnimationVarAliasType(float, m_flImageInsetY, "ImageInsetY", "0", "proportional_float");
	//CPanelAnimationVarAliasType(float, m_flBarWidth, "BarWidth", "356", "proportional_float");
	//CPanelAnimationVarAliasType(float, m_flBarHeight, "BarHeight", "19", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarChunkWidth, "BarChunkWidth", "2", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarChunkGap, "BarChunkGap", "0", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flImageWidth, "ImageWidth", "512", "float");
	CPanelAnimationVarAliasType(float, m_flImageHeight, "ImageHeight", "128", "float");
	CPanelAnimationVarAliasType(float, m_flImageOffsetX, "ImageOffsetX", "0", "float");
	CPanelAnimationVarAliasType(float, m_flImageOffsetY, "ImageOffsetY", "0", "float");

	CPanelAnimationVarAliasType(float, m_flBarOffsetX, "BarOffsetX", "99", "float");
	CPanelAnimationVarAliasType(float, m_flBarOffsetY, "BarOffsetY", "53", "float");
	CPanelAnimationVarAliasType(float, m_flBarWidth, "BarWidth", "356", "float");
	CPanelAnimationVarAliasType(float, m_flBarHeight, "BarHeight", "19", "float");
	//CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "HUDBarText");
	//CPanelAnimationVarAliasType(float, text_xpos, "text_xpos", "2", "proportional_float");
	//CPanelAnimationVarAliasType(float, text_ypos, "text_ypos", "2", "proportional_float");
	//CPanelAnimationVarAliasType(float, text2_xpos, "text2_xpos", "8", "proportional_float");
	//CPanelAnimationVarAliasType(float, text2_ypos, "text2_ypos", "40", "proportional_float");
	//CPanelAnimationVarAliasType(float, text2_gap, "text2_gap", "10", "proportional_float");
	float m_flHealthSize;
	int	m_bitsDamage;
	int m_nHealthLow;

	int m_iBarTextureId;
	int	m_iTopTextureId;
	int	m_iBottomTextureId;
	int m_iGlowTextureId;
	char	m_szBarTextureName[64];
	char	m_szTopTextureName[64];
	char	m_szBottomTextureName[64];
	char	m_szGlowTextureName[64];
};

#endif // HUD_HEALTH_H