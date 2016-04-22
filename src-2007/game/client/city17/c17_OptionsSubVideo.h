//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's options dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Video Options submenu.
//
//==========================================================================================//

#ifndef C17_OPTIONS_SUB_VIDEO_H
#define C17_OPTIONS_SUB_VIDEO_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>

class CCvarComboBox;

namespace vgui
{
    class Label;
    class Panel;
}


class CC17OptionsSubVideo : public vgui::PropertyPage
{
public:
	CC17OptionsSubVideo( vgui::Panel *parent, const char *name );
	~CC17OptionsSubVideo();

	virtual void SetComboBoxDefaults();
	virtual void OnResetData();
	virtual void OnApplyChanges();

	void SetupTooltips( void );

protected:
    virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:

	CCvarComboBox		*m_pRadBox;
	//CCvarComboBox		*m_pAABox;
	CCvarComboBox		*m_pMCBox;
	CCvarComboBox		*m_pShadowFixBox;
	CCvarComboBox		*m_pMuzzleFlashProj;
	CCvarComboBox		*m_pProjectedShadows;
	CCvarComboBox		*m_pSunBox;
	CCvarComboBox		*m_pPhongBox;
	CCvarComboBox		*m_pImpactBox;
	CCvarComboBox		*m_pWaterBox;
	CCvarComboBox		*m_pPPBox;
	CCvarComboBox		*m_pFXAABox;

	typedef vgui::PropertyPage BaseClass;
};



#endif // C17_OPTIONS_SUB_VIDEO_H
