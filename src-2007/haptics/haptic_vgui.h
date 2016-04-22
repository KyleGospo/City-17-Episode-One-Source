// vgui elements - elegant dynamic convar controlling vgui classes ;)

#ifndef HAPTICVGUI_H
#define HAPTICVGUI_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Label.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/TextEntry.h>

//heres some shared defines for easy changing
#define HUI_STANDARD_TALL 24
#define HUI_SLIDER_TALL 32
#define HUI_SPACER_SMALL 5
#define HUI_LABEL_SMALL 128
#define HUI_SMALL_TEXTBOX 32
#define HUI_STANDARD_WIDE 480
#define HUI_STANDARD_INDENT 20
#define HUI_SLIDER_SPACE 10

#define READ_CURRENT_DATA "READCURRENT"
#define SAVE_CURRENT_DATA "SAVECURRENT"
#define READ_DEFAULT_DATA "READDEFAULT"
//must line up to eachother!
enum CVarUITypes
{
	HUI_SLIDER = 0,
	HUI_ROWS,
	HUI_LABEL,
};
static const char * s_CVarUITypeNames[] = 
{
	"slider",
	"rows",
	"label",
};
static int GetUIType(const char *name)
{
	for( int i = 0; i!=ARRAYSIZE(s_CVarUITypeNames); i++ )
	{
		if(!V_stricmp(s_CVarUITypeNames[i],name))
			return i;
	}
	return -1;
};

// Quick macros to check types make sure the keyvalues is named curKey
#define HUI_DECLARE_SHARED_VARIABLES int x,y,wide,tall;const char *name;x=y=wide=tall=0;name="UNNAMED";

#define HUI_TABLE_START(keyValue) KeyValues*curKey = ##keyValue##->GetFirstTrueSubKey();while(curKey){switch(curKey->GetDataType()){ default:

#define HUI_MATCH_INT(x) else if(!V_stricmp(#x,curKey->GetName())){##x##=curKey->GetInt();break;}

#define HUI_MATCH_INT_START case 2:{if(!V_stricmp("x",curKey->GetName())){ x =curKey->GetInt();break;}else if(!V_stricmp("y",curKey->GetName())){y=curKey->GetInt();break;}else if(!V_stricmp("wide",curKey->GetName())){wide=curKey->GetInt();break;}else if(!V_stricmp("tall",curKey->GetName())){tall=curKey->GetInt();break;}

#define HUI_MATCH_FLOAT_START(x) case 3:{if(!V_stricmp(#x,curKey->GetName())){ x =curKey->GetFloat();break;}
#define HUI_MATCH_FLOAT(x) else if(!V_stricmp(#x,curKey->GetName())){##x##=curKey->GetFloat();break;}

#define HUI_MATCH_STRING_START(x) case 1:{if(!V_stricmp(#x,curKey->GetName())){ x =curKey->GetString();break;}
#define HUI_MATCH_STRING(x) else if(!V_stricmp(#x,curKey->GetName())){##x##=curKey->GetString();break;}
#define HUI_MATCH_END };break;

#define HUI_TABLE_END }curKey = curKey->GetNextTrueSubKey();};

#define HUI_READ_STANDARD_INT HUI_MATCH_INT(x) 
// Forward declaration
struct CVInfo;

// Text entry for numbers only.
class NumberEntry : public vgui::TextEntry
{
	DECLARE_CLASS_SIMPLE(NumberEntry,vgui::TextEntry);
public:
	NumberEntry(vgui::Panel *parent,const char *name,float value);
	float GetValue();
	void SetValue(float value);
protected:
	//virtual MESSAGE_FUNC_WCHARPTR( OnSetText, "SetText", text );
};

// Slider with numeric display, lable, and ConVar handling.
class CVarSlider : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CVarSlider,vgui::Panel);
public:
	CVarSlider(vgui::Panel *parent, const char *name, const char *labeltext, const char *convarname, float defaultvalue, float realLow, float realHigh, int UILow, int UIHigh, int tickmarks, int align, float add=0, float mul=1 );
	virtual void OnCommand(const char *command);
	float GetRealValue();
	virtual void SetSize(int wide,int tall);
	
	static CVarSlider *CreateFromKeyValues(vgui::Panel *Parent, KeyValues *key, bool lookupSize=true);
	static CVarSlider *CreateRowsFromKeyValues(vgui::Panel *Parent, KeyValues *key, bool lookupSize=true);
	void SetLabelVisible(bool state){m_pLabel->SetVisible(state);};
	void SetLabelWidth( int wide );
	void SetSliderVisible(bool state){m_pSlider->SetVisible(state);if(m_pNumeralEntry)m_pNumeralEntry->SetVisible(state);};
	void AddConvarMimic(const char *convar_name, float add=0, float mul=1);
	void SetAutoSizeLabels(bool state){m_bAutoSize = state;};
	bool GetAutoSizeLabels(bool state){return m_bAutoSize;};
	virtual void OnCursorEntered();
	virtual void OnCursorExited();
	virtual void Paint();
	float GetSlidePercent();
protected:
	virtual void ReadCV();
	virtual void SaveCV();
	virtual void DefaultCV();
	void SetTextBoxNumber(float number);
	//virtual void SendSliderMovedMessage();
	float GetSliderValue();
	float GetCVarValue();
	//MESSAGE_FUNC( OnSliderMoved, "SliderMoved" );
	MESSAGE_FUNC( OnSetText, "TextChanged" );
	bool m_bAutoSize;
	//width of label ( label, combo, checkbox )
	int m_iLabelWidth;
protected:
	CVInfo *m_pConVar;
	// since there doesnt seem to be a GetNumTicks for slider class..
	int m_iNumTicks;
	vgui::Panel *m_pLabel;
	vgui::Slider*m_pSlider;
	NumberEntry *m_pNumeralEntry;
};
//slider with a chec box.
class CVarSliderCheck : public CVarSlider
{
	DECLARE_CLASS_SIMPLE(CVarSliderCheck,CVarSlider);
public:
	CVarSliderCheck(vgui::Panel *parent, const char *name, const char *label, 
					const char *convar_check, const char *convar_slide, 
					int convar_check_default = 1, float convar_slide_default = 1, 
					float convar_slide_min = 0, float convar_slide_max = 10, 
					int off_check = 0, int on_check = 1, 
					int ui_slide_min = 0, int ui_slide_max = 10, 
					int tickmarks = 10, int align = 0, float add = 0, float mul = 1 );
protected:
	virtual void ReadCV();
	virtual void SaveCV();
	virtual void DefaultCV();
	MESSAGE_FUNC( OnCheckPressed, "ButtonToggled" );
	int m_iOn;
	int m_iOff;
	int m_iDefault;
	const char *m_pCheckConVarName;
	const ConVar *m_pCheckConVar;
};
//combo box containing multiple sliders.
class CVarSliderCombo : public CVarSlider
{
	DECLARE_CLASS_SIMPLE(CVarSliderCombo,CVarSlider);
public:
	CVarSliderCombo(vgui::Panel *parent, const char *name, const char *label, const char *firstCvar, const char *firstCvarName, float firstDefault, float realLow, float realHigh, int UILow, int UIHigh, int tickmarks, int align, float add = 0, float mul = 1 );
	CVarSlider*AddSlider(const char *cvar_name, const char *cvar_title, float cvar_default);
	CVarSlider*AddSlider(CVarSlider *add, const char *cvar_title);
	virtual void SetSize(int wide, int tall);
	virtual void OnCommand(const char *command);
protected:
	MESSAGE_FUNC( OnSetText, "TextChanged" );
private:
	vgui::Panel*	m_pSliderStart;
	vgui::Label*	m_pName;
	vgui::ComboBox*	m_pCombo;
	int				m_iCurrentCount;
	CUtlVector<CVarSlider*>Sliders;

};
// places elements in rows. (from keyvalues lookup)
class PlacementGraph : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(PlacementGraph,vgui::Panel);
public:
	PlacementGraph(vgui::Panel *parent,KeyValues *data);
private:
	CUtlVector<vgui::Panel*>m_controls;
};

// line holder for line graphs
class HistoryLine
{
public:
	HistoryLine(const char *Name, Color color)
	{
		m_pName = Name;
		m_vColor = color;
	}
	bool GetValueAt( float &fValue, int iOffset )
	{
		if(iOffset>m_History.Count()+1)
			return false;
		else
			fValue = m_History[iOffset];
		return true;
	}
	void AddValue(float value, int maxhistory)
	{
		m_History.AddToHead(value);
		while(m_History.Count()>maxhistory)
		{
			m_History.Remove(m_History.Count()-1);
		}
	}
	Color GetColor()
	{
		return m_vColor;
	}
	void ClearHistory()
	{
		m_History.Purge();
	}
private:
	const char *m_pName;
	Color m_vColor;
	CUtlVector<float>m_History;
};
//float graph has not been tested.
class FloatGraph : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(FloatGraph,vgui::Panel);
	typedef CUtlVector<float>HistoricLine_t;
public:
	// create new float graph. UpdateRate is in milliseconds
	FloatGraph(vgui::Panel *parent, int UpdateRate, float fMin, float fMax );
	virtual void Paint();
	virtual void SetVisible(bool state);
	int AddLine(HistoryLine *Line);
	int AddLine(const char *Name);
	int AddLine(const char *Name,Color color);
protected:
	virtual void InitializeLines();
	virtual void OnTick();
	virtual float UpdateValues(int lineID);
private:
	CUtlVector<HistoryLine*>m_Lines;
	float GetPercent(float value);
	float m_flMin;
	float m_flMax;
};
class ButtonBindCombo : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(ButtonBindCombo,vgui::Panel);
public:
	ButtonBindCombo(vgui::Panel *parent,int keydef, const char *name, const char *image, const char *defaultaction = NULL );
	virtual void SetSize(int wide, int tall);
	virtual void		Paint();
	virtual void OnCommand(const char *command);
	void				AlignBinds();
	void				SaveBinds();
	// if action set to NULL the internal default that is set on construction will be checked.
	void				SetBind(const char *action=NULL);
protected:
private:
	vgui::ComboBox		*m_pCombo;
	vgui::ImagePanel	*m_pImage;
	vgui::Label			*m_pLabel;
	const char			*m_pDefaultAct;
	//int					m_iKey;
	ButtonCode_t		m_iKey;
	const char			*m_pKey;
};
// pat: draws a wireframe cube.

class ControlBoxVisual : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(ControlBoxVisual,vgui::Panel);
public:
	ControlBoxVisual(vgui::Panel *parent, CVarSlider *near, CVarSlider *right, CVarSlider *up, CVarSlider *far, CVarSlider *left, CVarSlider *down);
	virtual void Paint();
	MESSAGE_FUNC_PARAMS(OnSlideEnter, "CursorEnteredSlider", data);
	MESSAGE_FUNC_PARAMS(OnSlideExit, "CursorExitedSlider", data);
protected:
	void DrawCube(float Near=-1, float Right=-1, float Up=-1, float Far=1, float Left=1, float Down=1, int specialside=-1);
	enum eBoxID
	{
		HUI_BOX_UP =0,
		HUI_BOX_RIGHT,
		HUI_BOX_NEAR,
		HUI_BOX_DOWN,
		HUI_BOX_LEFT,
		HUI_BOX_FAR,
		HUI_BOX_SLIDERCOUNT,
	};

	struct CVarSliderCube
	{
		CVarSliderCube(CVarSlider *n,CVarSlider *r,CVarSlider *u,CVarSlider *f,CVarSlider *l,CVarSlider *d)
		{
			Near = n;
			Right = r;
			Up = u;
			Far = f;
			Left = l;
			Down = d;
		};
		CVarSlider *Near;
		CVarSlider *Right;
		CVarSlider *Up;
		CVarSlider *Far;
		CVarSlider *Left;
		CVarSlider *Down;
	};

	CVarSliderCube *SlideValues;// up right near down left far and spingk
	int m_iMouseOver;
	float m_flTime;
};

vgui::Panel *ProcessKV(vgui::Panel *parent, KeyValues *curKey);


#endif //HAPTICVGUI_H