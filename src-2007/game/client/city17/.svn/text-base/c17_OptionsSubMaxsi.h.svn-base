//========= Copyright © 2009, Team Gear & Nigredo Studios, All rights reserved. ============//
//
// Purpose: City 17's options dialog, built from NightFall code, originally created by Matt Stafford.
// Implements the Maxsi Options submenu.
//
//==========================================================================================//

#ifndef C17_OPTIONS_SUB_MAXSI_H
#define C17_OPTIONS_SUB_MAXSI_H
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


class CC17OptionsSubMaxsi : public vgui::PropertyPage
{
public:
	CC17OptionsSubMaxsi( vgui::Panel *parent, const char *name );
	~CC17OptionsSubMaxsi();

	virtual void SetComboBoxDefaults();
	virtual void OnResetData();
	virtual void OnApplyChanges();

	void SetupTooltips( void );

protected:
    virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:

	CCvarComboBox		*m_pMDBox;

	typedef vgui::PropertyPage BaseClass;
};



#endif // C17_OPTIONS_SUB_MAXSI_H
