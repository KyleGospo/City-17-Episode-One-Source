#ifndef C17OPTIONSPANEL_H
#define C17OPTIONSPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_helpers.h"
#include "nf_CvarComboBox.h"
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>

class CCvarComboBox;

using namespace vgui;

class CC17OptionsPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(CC17OptionsPanel,Frame);

public:

	CC17OptionsPanel( vgui::VPANEL parent );
	void SetVisible(bool state);

protected:

	virtual void OnApplyChanges( void );
	virtual void OnCommand(const char* pcCommand);
	virtual void SetComboBoxDefaults( void );

private:

	CCvarComboBox		*m_pRadBox;
	CCvarComboBox		*m_pHeadBox;
	CCvarComboBox		*m_pAABox;
	CCvarComboBox		*m_pMCBox;
	CCvarComboBox		*m_pSndBox;
	CCvarComboBox		*m_pMBBox;
	CCvarComboBox		*m_pShadowFixBox;
	CCvarComboBox		*m_pMuzzleFlashProj;
	CCvarComboBox		*m_pProjectedShadows;
	CCvarComboBox		*m_pSunBox;

};

DeclarePanel(CC17Options,CC17OptionsPanel,c17options);
#endif // C17OPTIONSPANEL_H 