///////////  Copyright © 2007, Mark Chandler. All rights reserved.  ///////////
//
// Description:
//		Header file for the panel list.
//		Also includes the base panel class for the sub panels
//
//
//	Mark Chandler 2007 admin@lodle.net
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PD_PANELLIST_H
#define PD_PANELLIST_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/panel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/ScrollBar.h>
#include <vgui_controls/Label.h>
#include <vgui/VGUI.h>

#include "Keyvalues.h"

#include <vgui/ISurface.h>
#include <vgui/IScheme.h>

using namespace vgui;

#define TITLEHIEGHT 25


class PD_BasePanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( PD_BasePanel, vgui::Panel );

public:
	Color* bgColor;

	PD_BasePanel(Panel *parent, const char *name) : Panel(parent, name)
	{
		this->SetMouseInputEnabled(true);
		this->SetKeyBoardInputEnabled(false);
		bgColor = new Color(200,200,200,255);
	}

	virtual void OnMousePressed(MouseCode code)
	{
		if (code == MOUSE_LEFT)
		{
			PostActionSignal(new KeyValues("ItemClick","name", this->GetName()));
		}
	}
	virtual void OnMouseDoublePressed(MouseCode code)
	{
		//Msg("XXX Code is: %d \n", code);
		if (code == MOUSE_LEFT)
		{
			PostActionSignal(new KeyValues("ItemDoubleClick","name", this->GetName()));
		}
	}

	void PaintBackground()
	{
		int wide, tall;
		GetSize(wide, tall);
		vgui::surface()->DrawSetColor(*bgColor);
		vgui::surface()->DrawFilledRect(0, 0, wide, tall);
	}
};

#define HEADERIMG "menu/sub_title"
class PD_HeaderPanel : public PD_BasePanel
{
	DECLARE_CLASS_SIMPLE( PD_HeaderPanel, PD_BasePanel );

	PD_HeaderPanel(Panel *parent, const char *name);

	void SetTitleText(char * text){;};
	void OnSizeChanged(int newWide, int newTall);
	void OnThink( void );

protected:
	void PaintBackground();

private:
	vgui::Label *m_pTitle;

	vgui::ImagePanel *m_pTitleBG;
	vgui::ImagePanel *m_pBodyBG;

	bool m_bNeedsUpdate;
	char m_szTitle[256];
};


struct panelInfo
{
	int	iHeader;
	vgui::Panel* pPanel;
	bool isSelectable;
};


//struct headerInfo
//{
//	char* szTitle;
//	vgui::Panel* pPanel;
//};

class PD_PanelList : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( PD_PanelList, vgui::Panel );

public:
	PD_PanelList(Panel *parent, const char *name);
	~PD_PanelList();

//	void RemovePanel( int index );
	int AddPanel( Panel* pPanel, bool isSelectable = true);
	int AddPanel( Panel* pPanel, char* header, bool isSelectable = true);
	Panel * GetPanel( int index )
	{
		if (index <0 || index > m_pPanelList.Count()-1)
			return NULL;
		else
			return m_pPanelList[index]->pPanel;
	}

	int GetSelected( void ){return m_iSelected;}
	void SetSelected( int index );
	void ShowSelected(bool val){m_bShowSelected=val;};


	void SetMargen( int amount );
	void SetHeight( int amount );

	void PurgeAll( void );
	int Count( void ){ return  m_pPanelList.Count(); }

	void Reset();
	void ResetAllChildPos();

	void SetHeaderEnabled(bool state){m_bHeaderDisabled=!state;}
	void SetFixedHeight(bool state){m_bFixedSize = state;}

	virtual void OnSizeChanged(int newWide, int newTall);
	virtual void OnThink( void );
	virtual void Update( void );

	MESSAGE_FUNC_INT( OnSliderMoved, "ScrollBarSliderMoved", position );

	
	MESSAGE_FUNC_CHARPTR( OnSetSelected, "SetSelected", name );
	MESSAGE_FUNC_CHARPTR( OnRequestUpdate, "NeedsUpdate", name );
	MESSAGE_FUNC_CHARPTR( OnItemClicked, "ItemClick", name );
	MESSAGE_FUNC_CHARPTR( OnItemDoubleClicked, "ItemDoubleClick", name );
	MESSAGE_FUNC_INT_INT( OnCursorMoved, "OnCursorMoved", x, y );

	//void OnMouseDoublePressed(MouseCode code);
	void OnKeyCodeTyped(KeyCode code);

	void PrintLoc();

	void OnMouseWheeled(int delta);
	void OnCommand(char* command);
	void UpdateSelected();

	virtual void SetPaintBackgroundEnabled( bool state ){ m_bPaintBg = state; }

protected:
	virtual void PaintBackground();
	virtual void Paint();
	virtual void ApplySchemeSettings(IScheme *pScheme);
	//virtual void PaintBorder();

	void updateSize( void );
	int FindItemUsingIdent(const char *pszIdent);
	int FindHeaderUsingIdent(const char *pszIdent);
	void MoveBy( int amount,  bool slider = false );

	void ResetSelected();

private:
	int m_iMargen;
	int m_iGroupMargen;
	int m_iHeight;
	int m_iMaxSliderVal;
	int m_iSelected;

	bool m_bHeaderDisabled;
	bool m_bShowSelected;
	bool m_bFixedSize;
	bool m_bRejectSliderMove;

	CUtlVector<panelInfo *> m_pPanelList;
	CUtlVector<vgui::Panel *> m_pHeaderList;
	vgui::ScrollBar *m_pScrollBar;
	PD_BasePanel *m_pSelImage;

	bool m_bPaintBg;
};



#endif