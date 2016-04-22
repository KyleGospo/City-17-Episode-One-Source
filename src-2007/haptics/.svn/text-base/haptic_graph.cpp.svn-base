#include "cbase.h"
#include <KeyValues.h>
#include "haptic_vgui.h"
#include "in_haptics.h"
#include "ienginevgui.h"
#include "haptic_graph.h"
#include <vgui_controls/Frame.h>

// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
HapticsGraph::HapticsGraph(vgui::VPANEL parent) :
BaseClass( NULL, 50, -3, 3) 
{
	// if created by VPanel, well need to put a frame behind us.
	vgui::Frame *frame = new vgui::Frame(NULL,"hdebug");
	frame->SetPos(24,24);
	SetWide(150);
	SetTall(75);
	frame->SetWide(GetWide());
	frame->SetTall(GetTall()+24);
	SetParent(frame);
}

HapticsGraph::HapticsGraph(vgui::Panel *parent, KeyValues *keys ) :
BaseClass( parent, 50, -3, 3) 
{
	if(keys)
	{
		HUI_DECLARE_SHARED_VARIABLES
		HUI_TABLE_START(keys)
		HUI_MATCH_INT_START
		HUI_MATCH_END
		HUI_TABLE_END
		SetBounds(x,y,wide,tall);		
	}
}

//interface class

class IHGraph : public IHapticGraph
{
public:
	virtual void Create( vgui::VPANEL parent )
	{
		Graph = new HapticsGraph(parent);
		Graph->SetWide(800);
		Graph->SetTall(400);
	}
	virtual void Destroy( void )
	{
		if(Graph)
		{
			Graph->DeletePanel();
		}
	}
	virtual vgui::Panel *GetPanel(void)
	{
		return Graph;
	}
private:
	HapticsGraph *Graph;
};

void HapticsGraph::InitializeLines()
{
	const char *names[] = 
	{
		"Force X",
		"Force Y",
		"Force Z",
		"Input X",
		"Input Y",
		"Input Z",
	};
	for(int g = 0;g!=2;g++)
	{
		for(int i = 0;i!=3;i++)
		{
			int nReg=(g+1)*i;
			m_iLines[nReg] = AddLine(names[nReg]);
		}
	};
}
void HapticsGraph::OnTick()
{
	m_vecForce = cliHaptics->GetLastForceSent();
	m_vecInput = cliHaptics->GetInputAxes();
	BaseClass::OnTick();
}
float HapticsGraph::UpdateValues(int line)
{
	if(line>2&&line<6)
	{
		return m_vecInput[line-3];
	}else{
		return m_vecForce[line];
	}
}

static IHGraph s_HGraph;
IHapticGraph *hapgraph= (IHapticGraph*)&s_HGraph;

CON_COMMAND(hap_debug_graph,"toggles haptics graph")
{
	//toggle visibility.
	hapgraph->GetPanel()->SetVisible( ! hapgraph->GetPanel()->IsVisible() );
}