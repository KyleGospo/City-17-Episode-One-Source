#ifndef HAPTIC_GRAPH_H
#define HAPTIC_GRAPH_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_helpers.h"
#include <vgui_controls/Frame.h>
using namespace vgui;

//haptics graph has not been tested.
class HapticsGraph : public FloatGraph
{
	typedef FloatGraph BaseClass;
public:
	HapticsGraph( vgui::VPANEL parent );
	// can read from keyvalues or normaly used within code.
	HapticsGraph( vgui::Panel *parent, KeyValues *keys = NULL );
protected:
	virtual void InitializeLines();
	virtual float UpdateValues(int line);
	virtual void OnTick();
private:
	int m_iLines[6];// y = force,input
	Vector m_vecForce;
	Vector m_vecInput;
};
class IHapticGraph
{
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;
	virtual vgui::Panel *GetPanel(void) = 0;
};
extern IHapticGraph *hapgraph;
#endif //HAPTIC_GRAPH_H