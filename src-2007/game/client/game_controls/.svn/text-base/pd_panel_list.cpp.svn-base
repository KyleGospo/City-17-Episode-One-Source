///////////  Copyright © 2007, Mark Chandler. All rights reserved.  ///////////
//
// Description:
//
//
//	Mark Chandler 2007 admin@lodle.net
//
////////////////////////////////////////////////////////////////////////////////


#include "cbase.h"
#include "pd_panel_list.h"

////////////////////////////////////////////////////////////////////////////////
//	Constructor
////////////////////////////////////////////////////////////////////////////////
PD_PanelList::PD_PanelList(Panel *parent, const char *name) : Panel(parent, name)
{
	this->SetPaintBackgroundEnabled( false );
	this->SetPaintBackgroundType( 0 );
	this->SetPaintBorderEnabled(true);

	this->SetKeyBoardInputEnabled( true );

	m_pScrollBar = new vgui::ScrollBar( this, "Scrollbar", true);
	m_pScrollBar->AddActionSignalTarget(this);

	m_pSelImage = new PD_BasePanel( this, "SelectImage");

	m_iMargen = 5;
	m_iGroupMargen = 20;
	m_iSelected =-1;
	m_iHeight = 20;
	m_iMaxSliderVal = 0;

	m_pSelImage->SetVisible( false );
	m_pSelImage->bgColor = new Color(50, 100, 255, 125) ;
	m_pSelImage->SetZPos(100);
	m_pSelImage->AddActionSignalTarget(this);
	updateSize();

	m_bHeaderDisabled = false;
	m_bShowSelected = true;
	m_bFixedSize = true;
	m_bRejectSliderMove = false;
	m_bPaintBg = true;
}

////////////////////////////////////////////////////////////////////////////////
//	Destructor
////////////////////////////////////////////////////////////////////////////////
PD_PanelList::~PD_PanelList()
{

}


int PD_PanelList::AddPanel( Panel* pPanel, bool isSelectable)
{
	return AddPanel(pPanel, NULL, isSelectable);
}


int PD_PanelList::AddPanel( Panel* pPanel, char* header,  bool isSelectable)
{
	int headId = -1;

	if (header!=NULL)
	{
		headId = FindHeaderUsingIdent(header);
		if (headId == -1)	//if header doesnt exsist make it
		{
			vgui::Panel* pPanel=new PD_HeaderPanel(this, header);
			pPanel->SetZPos(25);

			headId = m_pHeaderList.AddToTail(pPanel);
		}
	}


	int id = FindItemUsingIdent(pPanel->GetName());

	if (id != -1)
		return -1;


	pPanel->SetParent( this );

	pPanel->SetVisible(true);
	pPanel->SetPaintBackgroundEnabled( true );
	pPanel->SetPaintBackgroundType( 0 );
	pPanel->SetPaintBorderEnabled(true);
	pPanel->AddActionSignalTarget(this);
	pPanel->SetZPos(50);

	panelInfo* newPanel= new panelInfo;
	newPanel->iHeader = headId;
	newPanel->pPanel = pPanel;
	newPanel->isSelectable = isSelectable;

	int index = m_pPanelList.AddToTail( newPanel );

	ResetAllChildPos();


	//Msg("Scroll bar range is: %d\n", range);
	return index;
}

void PD_PanelList::ResetAllChildPos(  )
{
	m_iSelected = 0;

	if (m_bHeaderDisabled)
	{	
		int heightCount = 0;
		int iLastHeight=0;

		for(int i = 0; i < m_pPanelList.Count();i++)
		{
			vgui::Panel *pItem = m_pPanelList[i]->pPanel;
			int x,y;
			int tall=0;
	
			if (i <= 0)
			{
				if (m_bFixedSize)
					y=-1*(m_iHeight + m_iMargen);
				else
					y=-1*m_iMargen;
			}
			else
			{
				m_pPanelList[i-1]->pPanel->GetPos(x,y);
			}

			int ypos=y + m_iMargen +m_iMargen;

			if (m_bFixedSize)
			{
				ypos += m_iHeight;
				tall = m_iHeight;
			}
			else
			{
				ypos += iLastHeight;
				tall = m_pPanelList[i]->pPanel->GetTall();
				iLastHeight = tall;
				heightCount += tall;
			}

			pItem->SetBounds(m_iMargen, ypos, GetWide()-m_iMargen-m_iMargen-m_pScrollBar->GetWide(), tall);
			
		}

		for(int i = 0; i < m_pHeaderList.Count();i++)
		{
			m_pHeaderList[i]->SetVisible(false);
		}

		if (m_bFixedSize)
			m_iMaxSliderVal = m_pPanelList.Count()*m_iHeight+m_pPanelList.Count()*m_iMargen+m_iMargen;
		else
			m_iMaxSliderVal = heightCount + m_pPanelList.Count()*m_iMargen + m_iMargen;

		m_pScrollBar->SetRange(0, m_iMaxSliderVal);

	}
	else
	{
		int maxGroups = m_pHeaderList.Count();

		//Msg("maxGroups is %d\n", maxGroups);

		if (maxGroups==0)
			return;

		for (int i=0; i<maxGroups; i++)
		{
			//Msg("I is %d\n", i);

			int count=0;
			int heightCount=0;
			for (int j=0; j<m_pPanelList.Count(); j++ )
			{
				//Msg("%d: [%d] == [%d]\n",j, m_pPanelList[j]->iHeader , i);
				if (m_pPanelList[j]->iHeader == i)
				{
					heightCount += m_pPanelList[j]->pPanel->GetTall();
					count++;
				}
			}

			//Msg("I is %d, Count is %d\n", i, count);

			int x,y;
			if (i <= 0)
			{
				x=0;
				y=-1*m_iGroupMargen + m_iMargen;
			}
			else
			{
				m_pHeaderList[i-1]->GetPos(x,y);
				y += m_pHeaderList[i-1]->GetTall();
			}

			//Msg("I is %d\n", i);

			int tall =0;

			if (m_bFixedSize)
				tall = (count+1)*m_iMargen + count*m_iHeight + TITLEHIEGHT;
			else
				tall = (count+1)*m_iMargen + heightCount + TITLEHIEGHT;


			int ypos = y + m_iGroupMargen;

	
			if (m_pHeaderList[i])
			{
				m_pHeaderList[i]->SetVisible(true);
				m_pHeaderList[i]->SetBounds(m_iMargen, ypos, GetWide()-(m_iMargen*2)-m_pScrollBar->GetWide(), tall);
			}
	
			int lastypos =0;
			int iLastHeight=0;

			if (m_bFixedSize)
				lastypos = ypos + -1*(m_iHeight) + TITLEHIEGHT;
			else
				lastypos = ypos + TITLEHIEGHT;

			for (int j=0; j<m_pPanelList.Count(); j++ )
			{
				if (m_pPanelList[j]->iHeader == i)
				{
					vgui::Panel *pItem = m_pPanelList[j]->pPanel;

					int ypos = 0;
					if (m_bFixedSize)
						ypos = lastypos + m_iMargen + m_iHeight;
					else
						ypos = lastypos + m_iMargen + iLastHeight;

					lastypos = ypos;
					

					if (m_bFixedSize)
						pItem->SetBounds(m_iMargen*2, ypos, GetWide()-(m_iMargen*4)-m_pScrollBar->GetWide(), m_iHeight);
					else
					{
						iLastHeight = pItem->GetTall();
						pItem->SetBounds(m_iMargen*2, ypos, GetWide()-(m_iMargen*4)-m_pScrollBar->GetWide(), iLastHeight);
					}
				}
			}
		}

		m_iMaxSliderVal = 0;
		for (int i=0; i<maxGroups; i++)
		{
			m_iMaxSliderVal+=m_pHeaderList[i]->GetTall();
		}
		m_iMaxSliderVal+=m_iGroupMargen;

		m_pScrollBar->SetRange(0, m_iMaxSliderVal);
	}


}


void PD_PanelList::SetSelected( int index )
{
	if (index >= 0 && index < m_pPanelList.Count() && m_pPanelList[index]->isSelectable == true)
		m_iSelected = index;
}

void PD_PanelList::SetMargen( int amount )
{
	if (this->IsProportional())
		m_iMargen = scheme()->GetProportionalScaledValue(amount); 
	else
		m_iMargen = amount;
	
	updateSize();
}

void PD_PanelList::SetHeight( int amount )
{
	if (this->IsProportional())
		m_iHeight = scheme()->GetProportionalScaledValue(amount); 
	else
		m_iHeight = amount;
	
	updateSize();
}


void PD_PanelList::PurgeAll( void )
{
	for (int x=0; x<m_pPanelList.Count(); x++)
	{
		delete m_pPanelList[x]->pPanel;
	}

	//for (int x=0; x<m_pHeaderList.Count(); x++)
	//{
	//	delete m_pHeaderList[x];
	//}

	m_pPanelList.PurgeAndDeleteElements();
	m_pHeaderList.PurgeAndDeleteElements();
	m_iSelected =-1;
	m_pScrollBar->SetRange(0,0);
	m_iMaxSliderVal = 0;
}

void PD_PanelList::Reset()
{

}


void PD_PanelList::OnSizeChanged(int newWide, int newTall)
{
	m_pScrollBar->SetBounds(newWide-m_pScrollBar->GetWide(), 0, m_pScrollBar->GetWide(),  newTall);

	for(int i = 0; i < m_pPanelList.Count();i++)
	{
		m_pPanelList[i]->pPanel->SetWide(newWide-m_iMargen-m_iMargen-m_pScrollBar->GetWide());
	}

	m_pScrollBar->SetRangeWindow(GetTall()-m_iMargen-m_iMargen);


	ResetAllChildPos();
}


void PD_PanelList::OnThink( void )
{
	Update();
	BaseClass::OnThink();
}

void PD_PanelList::OnCommand(char* command)
{
	//Msg("Panel List Command: %s\n", command);
	if (strcmp(command,"UpdatePos")==0)
	{
		ResetAllChildPos();
	}
	else if ( GetVParent() )
	{
		KeyValues *msg = new KeyValues("Command");
		msg->SetString("command", command);	
		PostActionSignal( msg );
	};

}

void PD_PanelList::OnRequestUpdate(const char* PanelName)
{
	ResetAllChildPos();
	//OnItemClicked( PanelName );
}
void PD_PanelList::Update( void )
{
	if (!m_pPanelList.Count())
		return;

	int range = m_pPanelList.Count()*m_iHeight+m_pPanelList.Count()*m_iMargen+m_iMargen;
	if (range < GetTall())
		m_pScrollBar->SetEnabled( false );
	else
		m_pScrollBar->SetEnabled(true);
}

void PD_PanelList::OnSliderMoved(int value)
{
	if (m_bRejectSliderMove)
	{
		m_bRejectSliderMove=false;
		return;
	}

	//Msg("Moving to: %d [%d] [%d]\n", value, m_iMaxSliderVal, GetTall());
	if (value < 0)
		value = 0;

	if (value > m_iMaxSliderVal)
		value = m_iMaxSliderVal;

	int x,y;

	//if (m_bHeaderDisabled == true)
		m_pPanelList[0]->pPanel->GetPos(x, y);
	/*else
		m_pHeaderList[0]->GetPos(x, y);*/

	int ammount =  -1*(value+(y-m_iMargen));

	//stops an loop happning where moveby updates slider and makes it move again.
	m_bRejectSliderMove = true;

	MoveBy(ammount, true );
}

void PD_PanelList::ResetSelected()
{
	if (m_iSelected != -1 && m_bShowSelected)
	{
		int x,y;
		m_pPanelList[m_iSelected]->pPanel->GetPos(x,y);

		m_pSelImage->SetVisible( true );

		int tall=m_iMargen+m_iMargen;

		if (m_bFixedSize)
			tall += m_iHeight;
		else
			tall += m_pPanelList[m_iSelected]->pPanel->GetTall();

		m_pSelImage->SetBounds(m_iMargen, y-m_iMargen, GetWide()-m_pScrollBar->GetWide()-m_iMargen-m_iMargen,  tall);
	}
	else
	{
		m_pSelImage->SetVisible( false );
	}
}

void PD_PanelList::UpdateSelected()
{
	// make sure selected is allways shown
	if (m_iSelected != -1)
	{
		int x,y;
		m_pPanelList[m_iSelected]->pPanel->GetPos(x,y);

		if (m_bFixedSize)
		{
			if (y+m_iHeight > GetTall())	
				MoveBy( -1 *((y+m_iHeight+m_iMargen) - GetTall()));
		}
		else
		{
			int tall = m_pPanelList[m_iSelected]->pPanel->GetTall();
			//Msg("Im %d tall\n", tall);
			if (y+tall+m_iMargen > GetTall())	
				MoveBy( -1 *((y+tall+m_iMargen) - GetTall()));
		}

		if (y < 0)
			MoveBy( -y + m_iMargen );

	}

	ResetSelected();
	InvalidateLayout();
}




void PD_PanelList::MoveBy( int amount, bool slider )
{
	int x,y;
	for (int i=0; i<m_pPanelList.Count(); i++)
	{
		m_pPanelList[i]->pPanel->GetPos(x, y);
		m_pPanelList[i]->pPanel->SetPos(x, y+amount);
	}

	for (int i=0; i<m_pHeaderList.Count(); i++)
	{
		m_pHeaderList[i]->GetPos(x, y);
		m_pHeaderList[i]->SetPos(x, y+amount);
	}

	if (!slider)
	{
		m_pPanelList[0]->pPanel->GetPos(x, y);
		m_pScrollBar->SetValue(abs(y-m_iMargen));
	}

	ResetSelected();
	InvalidateLayout();
}

#define BORDERWIDTH 1

void PD_PanelList::Paint()
{
	BaseClass::Paint();
	//PaintBorder();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void PD_PanelList::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	SetBorder(pScheme->GetBorder("ButtonDepressedBorder"));
}

void PD_PanelList::PaintBackground()
{
	if (!m_bPaintBg)
		return;

	int wide, tall;
	GetSize(wide, tall);

	IScheme *pScheme = scheme()->GetIScheme( GetScheme() );

	vgui::surface()->DrawSetColor( GetSchemeColor("ListPanel.BgColor", pScheme) );
	vgui::surface()->DrawFilledRect(0, 0, wide, tall);
}

/*void PD_PanelList::PaintBorder()
{
	int wide, tall;
	GetSize(wide, tall);

	int sw = m_pScrollBar->GetWide();

	IScheme *pScheme = scheme()->GetIScheme( GetScheme() );

	surface()->DrawSetColor( GetSchemeColor("Border.Light", pScheme) );

	////draw top
	surface()->DrawFilledRect(BORDERWIDTH, 0, wide-sw, BORDERWIDTH);

	//draw bottom
	surface()->DrawFilledRect(BORDERWIDTH, tall-BORDERWIDTH, wide-sw, tall);

	//draw left
	surface()->DrawFilledRect(0, 0, BORDERWIDTH, tall);

	//draw right
	//surface()->DrawFilledRect(wide-BORDERWIDTH-BORDERINSTEP, captionHeight+1, wide-BORDERINSTEP, tall);
}*/

void PD_PanelList::updateSize( void )
{
	OnSizeChanged(GetWide(), GetTall());
}

int PD_PanelList::FindItemUsingIdent(const char *pszIdent)
{
	for(int i = 0; i < m_pPanelList.Count();i++)
	{
		vgui::Panel *pItem = m_pPanelList[i]->pPanel;
		if(pItem == NULL)
			continue;

		if(strcmp(pszIdent,pItem->GetName())==0)
			return i;

		//Msg("Comparing: [%s], [%s]\n", pszIdent,pItem->GetName());
	}
	return -1;
}


int PD_PanelList::FindHeaderUsingIdent(const char *pszIdent)
{
	for(int i = 0; i < m_pHeaderList.Count();i++)
	{
		vgui::Panel *pItem = m_pHeaderList[i];
		//headerInfo* pItem = m_pHeaderList[i];
		if(pItem == NULL)
			continue;

		if(strcmp(pszIdent,pItem->GetName())==0)
			return i;

		//Msg("Comparing: [%s], [%s]\n", pszIdent,pItem->GetName());
	}
	return -1;
}

void PD_PanelList::PrintLoc()
{
	for(int i = 0; i < m_pPanelList.Count();i++)
	{
		int x,y;
		m_pPanelList[i]->pPanel->GetPos(x,y);
		Msg("Panel %d, X:%d, Y:%d\n",i,x,y);	
	}
}

void PD_PanelList::OnKeyCodeTyped(KeyCode code)
{
	//Msg("KeyCode %d\n", code);
	if ( code == KEY_W || code == KEY_UP)
	{
		if (m_pPanelList.Count() > 0)
		{
			if (m_iSelected == -1)
				SetSelected(0);

			if (m_iSelected != 0)
				SetSelected(m_iSelected-1);
		}
		else
		{
			m_iSelected=-1;
		}

		UpdateSelected();
	}
	else if ( code == KEY_S || code == KEY_DOWN)
	{
		if (m_pPanelList.Count() > 0)
		{
			if (m_iSelected < m_pPanelList.Count()-1)
				SetSelected(m_iSelected+1);
		}
		else
		{
			m_iSelected=-1;
		}
		UpdateSelected();
	}
	//ShowSelected();
}

void PD_PanelList::OnCursorMoved( int x, int y)
{
}


void PD_PanelList::OnMouseWheeled(int delta)
{
	delta*=-1;
	if (m_pPanelList.Count() > 0)
	{
		if (m_iSelected == -1)
			m_iSelected = 0;
		else if ((m_iSelected + delta) >= 0 && (m_iSelected +delta) < m_pPanelList.Count())
			SetSelected(m_iSelected+delta);
	}
	else
	{
		m_iSelected=-1;
	}
	UpdateSelected();
}

void PD_PanelList::OnSetSelected(const char* name)
{
		OnItemClicked(name);
}

void PD_PanelList::OnItemClicked(const char* name)
{
	int id = FindItemUsingIdent( name );

	SetSelected(id);
	UpdateSelected();
}

void PD_PanelList::OnItemDoubleClicked(const char* name)
{
	//SetSelected(FindItemUsingIdent(name));

	KeyValues *msg = new KeyValues("Command");
	msg->SetString("command", "Select");	
	PostActionSignal( msg );

	//PostActionSignal(new KeyValues("ItemDoubleClick","name", this->GetName()));
}

//void PD_PanelList::OnMouseDoublePressed(MouseCode code)
//{
//	Msg("Mouse clicked\n");
//}


/////////////////////////////////////////////////////////////////////////////////////////////////////


PD_HeaderPanel::PD_HeaderPanel(Panel *parent, const char *name) : PD_BasePanel(parent, name)
{


	m_pTitleBG = new vgui::ImagePanel(this, "TitleBG");
	m_pTitleBG->SetFillColor(Color(255,255,0,255));
	m_pTitleBG->SetShouldScaleImage( true );

	m_pBodyBG = new vgui::ImagePanel(this, "BodyBG");
	m_pBodyBG->SetFillColor(Color(255,0,255,0));
	m_pBodyBG->SetVisible( false );

	m_pTitle = new vgui::Label(this, "TitleLabel", "[title]");
	m_pTitle->SetMouseInputEnabled(false);


	m_bNeedsUpdate = true;
	Q_strncpy(m_szTitle,"NULL NAME", 256);

	OnSizeChanged(GetWide(), GetTall());
}

void PD_HeaderPanel::OnThink( void )
{
	if (m_bNeedsUpdate)
	{
		m_pTitleBG->SetImage( HEADERIMG );

		m_pTitle->SetText(GetName());
		//m_pTitle->SetFgColor(Color(0,0,0,255));

		m_bNeedsUpdate = false;
	}
}

void PD_HeaderPanel::PaintBackground()
{
	return;

	//int wide, tall;
	//GetSize(wide, tall);
	//vgui::surface()->DrawSetColor(Color(120,120,120,255));
	//vgui::surface()->DrawFilledRect(0, 0, wide, tall);
}

void PD_HeaderPanel::OnSizeChanged(int newWide, int newTall)
{
	m_pTitle->SetBounds( 10, 0, newWide, TITLEHIEGHT);

	m_pTitleBG->SetBounds(0,0,newWide, TITLEHIEGHT);
	m_pBodyBG->SetBounds(0,TITLEHIEGHT,newWide, newTall-TITLEHIEGHT);
}