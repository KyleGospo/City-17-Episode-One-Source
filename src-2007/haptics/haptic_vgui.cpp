// Special vgui classes
#include "cbase.h"
#include "haptic_vgui.h"
#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/ImagePanel.h>
#include "vgui_helpers.h"
#include "filesystem.h"
//#include <keydefs.h>
#include "inputsystem/ButtonCode.h"
#include <kbutton.h>
#include <IGameUIFuncs.h>
#include <vgui/isurface.h>
#include <vgui/IInputInternal.h>
// normal haptics access
#include "../haptics/client_haptics.h"
// low level haptics access
#include "../haptics/in_haptics.h"
#include <keyvalues.h>

// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

//quick functions to hook commands regarding the client settings to print
ConVar hap_debug_ui("hap_debug_ui", "0", FCVAR_ARCHIVE);
static void RunClientCommand( const char *cmd )
{
	if(hap_debug_ui.GetInt()!=0)
	{
		Msg("HUI Client Command - { %s }\n",cmd);
	}	
	engine->ClientCmd(cmd);
};
static bool SetConVarValue( const ConVar *ccvar, float value )
{
	ConVar *cv = (ConVar *)ccvar;
	if(cv)
	{
		if(hap_debug_ui.GetInt()!=0)
		{
			Msg("HUI ConVar Setting - { %s = %f }\n", cv->GetName(), value );
		}
		cv->SetValue(value);
		return true;
	}else{
		if(hap_debug_ui.GetInt()!=0)
		{
			Warning("HUI ConVar Setting - { %s = INVALID }\n", cv->GetName() );
		}
		return false;
	}
}
static bool SetConVarValue( const char *cvarname, float value )
{
	const ConVar *ccvar = cvar->FindVar(cvarname);
	if(ccvar)
	{
		return SetConVarValue(ccvar, value);
	}
	else if(hap_debug_ui.GetInt()!=0)
	{
		Warning("HUI ConVar Setting - { CONVAR %s NOT FOUND }\n", cvarname );
	}
	return false;
}
//Below is temporary solution untill all defaults are in. it will read off default.cfg for default value one time only and then store the values in for ui ussage.
struct quickDefault
{
	char cvarname[80];
	char cvarvalue[80];
};
static CUtlVector<quickDefault*>* s_DefaultData = NULL;

static const char *GetDefaultValue(const ConVar *cv)
{
	if(s_DefaultData==NULL)
	{
		s_DefaultData = new CUtlVector<quickDefault*>();
		char filepath[1024];
		sprintf(filepath,"%s\\cfg\\default.cfg",engine->GetGameDirectory());
		//FileHandle_t pFile=vgui::filesystem()->Open(filepath,"r");
		FileHandle_t pFile = filesystem->Open(filepath, "r");

		// clear our bindings.
		s_DefaultData->RemoveAll();
		
		// now parse our default.cfg
		//while(!vgui::filesystem()->EndOfFile(pFile))
		while(!filesystem->EndOfFile(pFile))
		{
			char line[512];
			//vgui::filesystem()->ReadLine(line,sizeof(line),pFile);
			filesystem->ReadLine(line,sizeof(line),pFile);
			CUtlVector<char *>params;
			V_SplitString((const char *)line," ",params);
			if(params.Count()>1)
			{
				quickDefault *current = new quickDefault;
				V_strcpy(current->cvarname,params[0]);
				V_SplitString((const char *)line,"\"",params);
				V_strcpy(current->cvarvalue,params[1]);
				s_DefaultData->AddToTail(current);
			}
		}
	}
	for(int i =0;i!= s_DefaultData->Count(); i++)
	{
		if(!V_stricmp(s_DefaultData->Element( i )->cvarname,cv->GetName()))
		{
			if(hap_debug_ui.GetInt()!=0)
			{
				Msg("HUI default.cfg Read ConVar %s value = %s\n", s_DefaultData->Element(i)->cvarname, s_DefaultData->Element(i)->cvarvalue);
			}
			return s_DefaultData->Element(i)->cvarvalue;
		}
	};
	return "";
}

// Quick Struct to adjust scaling of CVars
struct CVInfo
{
	struct MimicVars
	{
		MimicVars(const char *name, float mul=1, float add=0)
		{
			sz_name = name;
			CVar = NULL;
			additive = add;
			multiply = mul;
		}
		MimicVars(const ConVar *cv, float mul=1, float add=0)
		{
			CVar = cv;
			multiply = mul;
			additive = add;
		}
		const ConVar*GetConVar()
		{
			if(CVar==NULL)
				CVar = cvar->FindVar(sz_name);
			return CVar;
		}
		const char *sz_name;
		const ConVar*CVar;
		float additive;
		float multiply;
	};
	CUtlVector<MimicVars*>ConVars;
	const ConVar *CVar;//head ConVar.
	float cmin;
	float cmax;
	int umin;
	int umax;
	float fTemp;
	float fDefault;
	bool bInitialized;//initialize on first usage. this ensures global pointer cvar is intact.
	//Converts CV value to UI value
	float CVtoUI(float value)
	{
		float perc = ( value - cmin ) / ( cmax - cmin );
		return umin + perc*(float)(umax-umin);
	}
	//Converts UI value to CV value
	float UItoCV(float value)
	{
		float perc = ( value - umin ) / (float)( umax - umin );
		return cmin + perc*(cmax-cmin);
	}
	//Constructor
	CVInfo(const char *CVarName, float ConVarDefault, float ConVarMin, float ConVarMax, int UIMin, int UIMax, float add=0, float mul=1)
	{
		CVar = NULL;
		ConVars.AddToTail(new MimicVars(CVarName,mul,add));
		bInitialized = false;
		//note : once we do get a final set of defaults, it should really use atof(CVar->GetDefault())
		fDefault = ConVarDefault;

		umin = UIMin;
		umax = UIMax;
		cmin = ConVarMin;
		cmax = ConVarMax;

	}
	// converts current value to UI value.
	float GetUIValue(bool temp=true)
	{
		return CVtoUI(GetCVValue(temp));
	}
	//gets the cv value.
	float GetCVValue(bool temp=true)
	{
		if(!bInitialized)
		{
			for(int i = 0;i != ConVars.Count();i++)
			{
				if(i==0)
					CVar = ConVars[i]->GetConVar();
				else
					ConVars[i]->GetConVar();//initialize.
			};
			bInitialized = true;
		}
		if( CVar )
		{
			return temp ? fTemp : CVar->GetFloat();
		}
		else
		{
			return fTemp;
		}
	}
	//gets the default cvvalue.
	float GetCVDefault()
	{
		//return fDefault;
		const char *dVal = GetDefaultValue(CVar);
		if(dVal==NULL)
		{
			Warning("Could not find default value in default.cfg for convar named \"%s\"\n", CVar->GetName());
		}
		return atof( dVal );
	}
	//gets the default uivalue.
	float GetUIDefault()
	{
		return CVtoUI(fDefault);
	}
	//sets in the cv value by the UI value.
	void SetUIValue(float uivalue, bool temp=true)
	{
		SetCVValue(UItoCV(uivalue),temp);
	}
	void SetCVValue(float value,bool temp=true)
	{
		fTemp = value;
	
		if(!temp)
			SaveTemp();
	}
	// saves temp to actual cvar value.
	void SaveTemp()
	{
		for(int i = 0;i!= ConVars.Count();i++)
		{
			SetConVarValue(ConVars[i]->GetConVar(),(fTemp * ConVars[i]->multiply) + ConVars[i]->additive );
		}
	}
	// returns true if temp doesnt equal actual cvar value.
	bool Unsaved()
	{
		return (CVar->GetFloat()==fTemp);
	}
	// add mimic: This must be called prior to grabing any variables ( preferably right after construction )
	void AddMimic(const char *ConVarName, float add = 0, float mul = 1)
	{
		ConVars.AddToTail(new MimicVars(ConVarName, mul, add));
	}
};



//Quick text box for numeric only ( with built in look up )

NumberEntry::NumberEntry(vgui::Panel *parent, const char *name, float value) :
BaseClass(parent,name)
{
	SetAllowNumericInputOnly(true);
	SetValue(value);
}
void NumberEntry::SetValue(float value)
{
	char sz_value[32];
	int rounded = (int)value;
	if(value==(float)rounded)// if float and int are identical ( no remainder )
		V_snprintf(sz_value,sizeof(sz_value),"%i",rounded);//use the int
	else
	{
		V_snprintf(sz_value,sizeof(sz_value),"%f",value);
		// now remove unused decimals.
		CUtlVector<char *>Decimal;
		V_SplitString((const char *)sz_value,".",Decimal);
		CUtlVector<char *>Zeros;
		char numb[6];
		V_strcpy(numb,Decimal[1]);
		int place=0;
		for(int i = 5;i!=-1;i--)
		{
			if(numb[i]==(char)"0")
			{
				place++;
			}else{
				break;
			}
		}
		if(place!=0)
		{
			char rDec[6];
			V_snprintf(rDec,place,"%s",Decimal[0]);
			V_snprintf(sz_value,sizeof(sz_value),"%s.%s",Decimal[0],rDec);
		}
	}
	SetText(sz_value);
}
float NumberEntry::GetValue()
{
	char sz_number[32];
	GetText(sz_number,sizeof(sz_number));
	return atof((const char *)sz_number);
}
CVarSlider *CVarSlider::CreateFromKeyValues(vgui::Panel *Parent, KeyValues *key, bool lookupsize)
{
	//note: the names of the variables below are directly tied to the keyvalues lookup.
	const char *label_text = "";
	const char *convar = "";
	float value_default = 1;
	float value_min =0;
	float value_max = 10;
	int slide_min = 0;
	int slide_max = 1;
	int slide_ticks = 10;
	int text_align = 0;//above
	//declares x y wide tall and name
	HUI_DECLARE_SHARED_VARIABLES
	//start the reading table.
	HUI_TABLE_START(key)
	
	//integers
	HUI_MATCH_INT_START
	HUI_MATCH_INT(slide_min)
	HUI_MATCH_INT(slide_max)
	HUI_MATCH_INT(slide_ticks)
	HUI_MATCH_INT(text_align)
	HUI_MATCH_END
	
	//floating points
	HUI_MATCH_FLOAT_START(value_default)
	HUI_MATCH_FLOAT(value_min)
	HUI_MATCH_FLOAT(value_max)
	HUI_MATCH_END

	//strings
	HUI_MATCH_STRING_START(name)
	HUI_MATCH_STRING(label_text)
	HUI_MATCH_STRING(convar)
	HUI_MATCH_END

	HUI_TABLE_END
	
	CVarSlider *pSlider = new CVarSlider(	Parent, name, label_text, convar, 
											value_default, value_min, value_max, 
											slide_min, slide_max, slide_ticks,
											text_align );
	if(lookupsize)
		pSlider->SetBounds(x,y,wide,tall);
	return pSlider;

};
CVarSlider *CVarSlider::CreateRowsFromKeyValues(vgui::Panel *Parent, KeyValues *key, bool lookupsize)
{
	HUI_DECLARE_SHARED_VARIABLES

	HUI_TABLE_START(key)

	//integers
	HUI_MATCH_INT_START
	HUI_MATCH_END

	HUI_TABLE_END

	KeyValues *ck = key->GetFirstTrueSubKey();
	vgui::Panel *Slider = Parent;
	while(ck)
	{
		if( ! V_stricmp(ck->GetName(),"row") )
		{
			Slider = CreateFromKeyValues(Slider,ck,false);
			Slider->SetWide(wide);
			Slider->SetTall(tall);
			if(Slider==Parent)
			{
				Slider->SetPos(x,y);
			}
			else
			{
				Slider->SetPos(0,tall);
			}
		}
		ck = ck->GetNextTrueSubKey();
	}

	if(Slider==Parent)
		return NULL;
	else
		return (CVarSlider *)Slider;
};

CVarSlider::CVarSlider(vgui::Panel *parent, const char *name, const char *labeltext, const char *convarname, float defaultvalue, float realLow, float realHigh, int UILow, int UIHigh, int tickmarks, int align, float add, float mul ) :
BaseClass(parent,name)
{
	m_bAutoSize = false;
	m_iNumTicks = tickmarks;
	m_pConVar = NULL;
	m_iLabelWidth = HUI_LABEL_SMALL;
	m_pSlider = new vgui::Slider(this,name);
	m_pSlider->AddActionSignalTarget(GetVPanel());
	m_pLabel = NULL;
	m_pConVar = new CVInfo(convarname, defaultvalue, realLow, realHigh, UILow, UIHigh, add, mul);
	m_pSlider->SetRange(m_pConVar->umin,m_pConVar->umax);
	m_pSlider->SetNumTicks(tickmarks);
	m_pSlider->SetTickCaptions("#GameUI_Low","#GameUI_High");

	char lablename[32];
	V_snprintf(lablename,sizeof(lablename),"%s_lb",name);
	if(align!=-1 && labeltext!=NULL )
	{
		m_pLabel = new vgui::Label(this, lablename, labeltext);
		// disable mouse input fro the lable ( so we can drag it easier in build mode )
		m_pLabel->SetMouseInputEnabled(false);
	}
	V_snprintf(lablename,sizeof(lablename),"%s_tx",name);
	m_pNumeralEntry = new NumberEntry(this,lablename, 0);
	m_pNumeralEntry->AddActionSignalTarget(GetVPanel());
	//m_pNumeralEntry->SetAllowNumericInputOnly(true);

	SetSize( HUI_STANDARD_WIDE - HUI_STANDARD_INDENT, HUI_SLIDER_TALL );
}
void CVarSlider::OnCursorEntered()
{
	PostActionSignal(new KeyValues("CursorEnteredSlider","VPANEL",(int)GetVPanel()));
	BaseClass::OnCursorEntered();
}
void CVarSlider::OnCursorExited()
{
	PostActionSignal(new KeyValues("CursorExitedSlider","VPANEL",(int)GetVPanel()));
	BaseClass::OnCursorExited();
}
void CVarSlider::SetSize(int wide, int tall)
{
	if(m_bAutoSize && m_pLabel)
	{
		m_pLabel->SetWide(264-(HUI_SLIDER_SPACE+11));//TEMPORARY. (only the movement checkbox uses this feature so we are fitting it to that.
		// we would want to put in a function to getthe size of the text ( but remember the child classes of this don't
		// just use the vgui::Label class, currently they also use vgui::CheckButton and vgui::ComboBox.

		m_pLabel->SetPos(0,0);
		m_pSlider->SetBounds(m_pLabel->GetWide()+HUI_SLIDER_SPACE,0,( wide- ( m_pLabel->GetWide() + (m_pNumeralEntry ? (HUI_SMALL_TEXTBOX + HUI_SLIDER_SPACE) : 0 ) ) ),HUI_SLIDER_TALL);
	}else{
		m_pSlider->SetBounds(m_pLabel ? (m_iLabelWidth+HUI_SLIDER_SPACE) : 0,
			0,
			wide-( (m_pLabel ? ( m_iLabelWidth + HUI_SLIDER_SPACE ) : 0 )+( m_pNumeralEntry ? (HUI_SMALL_TEXTBOX + HUI_SLIDER_SPACE) : 0 ) ),
			HUI_SLIDER_TALL);

		if(m_pLabel)
			m_pLabel->SetBounds(0,0,m_iLabelWidth,HUI_STANDARD_TALL);
	}
	if(m_pNumeralEntry)
		m_pNumeralEntry->SetBounds(wide-HUI_SMALL_TEXTBOX,0,HUI_SMALL_TEXTBOX,HUI_STANDARD_TALL);
	BaseClass::SetSize(wide,tall);
}
/*void CVarSlider::OnSliderMoved()
{
	//send a signal saying that we are on the slider.
	PostActionSignal(new KeyValues("CursorEnteredSlider","VPANEL",(int)GetVPanel()));
	m_pNumeralEntry->SetValue(m_pSlider->GetValue());
}*/
void CVarSlider::OnSetText()
{
	//send a signal saying that we are on the slider.
	m_pSlider->SetValue((int)m_pNumeralEntry->GetValue(),false);
}
void CVarSlider::SaveCV()
{
	if(m_pConVar)
	{
		m_pConVar->SetUIValue(m_pNumeralEntry->GetValue());
		m_pConVar->SaveTemp();
	}
}
void CVarSlider::ReadCV()
{
	if(m_pConVar)
	{
		float value = abs(m_pConVar->GetUIValue(false));
		m_pNumeralEntry->SetValue(value);
		m_pSlider->SetValue((int)value,false);
	}
}
void CVarSlider::DefaultCV()
{
	if(m_pConVar)
	{
		float fUIDefault = m_pConVar->GetUIDefault();
		m_pNumeralEntry->SetValue(fUIDefault);
		m_pSlider->SetValue((int)fUIDefault,false);
	}
}
void CVarSlider::OnCommand(const char *command)
{
	if(!V_stricmp(command,SAVE_CURRENT_DATA))
		SaveCV();
	else if(!V_stricmp(command,READ_CURRENT_DATA))
		ReadCV();
	else if(!V_stricmp(command,READ_DEFAULT_DATA))
		DefaultCV();
	else if(!V_stricmp(command,"NUMBERUPDATE"))
	{
		if(!IsBeingDragged())// only if not dragging.
			m_pSlider->SetValue( (int)(m_pNumeralEntry->GetValue()) );
	}
	else
		BaseClass::OnCommand(command);
}
float CVarSlider::GetSliderValue()
{
	return m_pNumeralEntry->GetValue();//(float)(GetValue()-UIScale[0])/(float)(UIScale[1]-UIScale[0]);
}
void CVarSlider::AddConvarMimic(const char *CVname, float add, float mul)
{
	m_pConVar->AddMimic(CVname,add,mul);
}
void CVarSlider::Paint()
{
	BaseClass::Paint();
	/*
	if(m_pLabel)
	{
		vgui::surface()->DrawSetColor(0,0,0,100);
		vgui::surface()->DrawFilledRect(-5,0,GetWide()+5,GetTall());
	}
	*/
};
float CVarSlider::GetSlidePercent()
{
	return ((float)(m_pNumeralEntry->GetValue()-m_pConVar->umin)/(float)(m_pConVar->umax-m_pConVar->umin));
}
void CVarSlider::SetLabelWidth( int wide )
{
	//set in the variable
	m_iLabelWidth = wide;
	//do a full reset.
	SetSize(GetWide(),GetTall());
};
CVarSliderCheck::CVarSliderCheck(
	vgui::Panel *parent, const char *name, const char *label, 
	const char *convar_check, const char *convar_slide, 
	int convar_check_default, float convar_slide_default, 
	float convar_slide_min, float convar_slide_max, 
	int off_check, int on_check, 
	int ui_slide_min, int ui_slide_max, 
	int tickmarks, int align, float add, float mul ) :
BaseClass(parent, name, NULL, convar_slide, convar_slide_default, 
		  convar_slide_min, convar_slide_max, 
		  ui_slide_min, ui_slide_max, tickmarks, align, add, mul)
{
	m_iOn = on_check;
	m_iOff = off_check;
	m_iDefault = convar_check_default;
	m_pLabel = new vgui::CheckButton(this, "check", label);
	m_pNumeralEntry->SetEnabled(false);
	m_pSlider->SetEnabled(false);
	m_pLabel->AddActionSignalTarget(this);
	m_pCheckConVar = NULL;
	m_pCheckConVarName = convar_check;
}

void CVarSliderCheck::OnCheckPressed()
{
	vgui::CheckButton *check = (vgui::CheckButton*)m_pLabel;
	bool state = check->IsSelected();
	m_pSlider->SetEnabled(state);
	m_pNumeralEntry->SetEnabled(state);
}
void CVarSliderCheck::ReadCV()
{
	BaseClass::ReadCV();
	if(m_pCheckConVar==NULL)
	{
		m_pCheckConVar = cvar->FindVar(m_pCheckConVarName);
	}
	vgui::CheckButton *check = (vgui::CheckButton*)m_pLabel;
	check->SetSelected(m_pCheckConVar->GetInt()==m_iOn);
	OnCheckPressed();
}
void CVarSliderCheck::DefaultCV()
{
	BaseClass::DefaultCV();
	vgui::CheckButton *check = (vgui::CheckButton*)m_pLabel;
	check->SetSelected(m_iDefault==m_iOn);
	OnCheckPressed();
}
void CVarSliderCheck::SaveCV()
{
	BaseClass::SaveCV();
	ConVar *CVcheck = (ConVar *)m_pCheckConVar;
	if(CVcheck)
	{
		vgui::CheckButton *check = (vgui::CheckButton*)m_pLabel;
		if(check)
			CVcheck->SetValue(check->IsSelected() ? m_iOn : m_iOff);
		Msg("%s %s\n",CVcheck->GetName(),check->IsSelected() ? "on" : "off");
	}
}

CVarSliderCombo::CVarSliderCombo(vgui::Panel *parent, const char *name, const char *label, const char *firstCvar, const char *firstCvarTitle, float firstDefault, float realLow, float realHigh, int UILow, int UIHigh, int tickmarks, int align, float add, float mul ) : 
BaseClass(parent,name,NULL,firstCvar,firstDefault,realLow,realHigh,UILow,UIHigh,tickmarks,align,add,mul)
{
	SetTall(HUI_SLIDER_TALL + HUI_STANDARD_TALL); 
	m_pName = new vgui::Label(this,"name",label);
	vgui::ComboBox *combo = new vgui::ComboBox(this,"combo",0,false);
	combo->AddActionSignalTarget(GetVPanel());
	m_pLabel = (vgui::Panel*)combo;
	m_iCurrentCount = 0;
	if(m_pSlider!=NULL)
	{
		AddSlider(this,firstCvarTitle);
	}
};
CVarSlider *CVarSliderCombo::AddSlider(const char *cvar_name, const char *cvar_title, float cvar_default)
{
	char id[10];
	V_snprintf(id,sizeof(id),"combo%i",m_iCurrentCount);
	CVarSlider *current = new CVarSlider(this,id,cvar_title,cvar_name,cvar_default,
										m_pConVar->cmin, m_pConVar->cmax,
										m_pConVar->umin, m_pConVar->umax,
										m_iNumTicks,-1);
	return AddSlider(current,cvar_title);
}
CVarSlider *CVarSliderCombo::AddSlider(CVarSlider *add,const char *cvar_title)
{
	if(add!=this)
		add->SetVisible(false);
	else
		add->SetSliderVisible(false);
	vgui::ComboBox *combo = (vgui::ComboBox*)m_pLabel;
	combo->AddItem(cvar_title,new KeyValues("numbers","id",m_iCurrentCount));
	Sliders.AddToTail(add);
	if(m_iCurrentCount==0)
	{
		if(add==this)
			add->SetSliderVisible(true);
		else
			add->SetVisible(true);
		combo->ActivateItemByRow(0);
		OnSetText();
	}
	m_iCurrentCount++;
	SetSize(GetWide(),GetTall());
	return add;
}
// handles the combo box changes too!
void CVarSliderCombo::OnSetText()
{
	vgui::ComboBox*combo = (vgui::ComboBox*)m_pLabel;
	// set each slider not visible
	for(int i = 0;i != combo->GetItemCount(); i++)
	{
		if(Sliders[combo->GetItemUserData(i)->GetInt("id")]==this)
			SetSliderVisible(false);
		else
			( Sliders[ combo->GetItemUserData(i)->GetInt("id") ] ) ->SetVisible(false);
	}
	// set our newly selected slider visible.
	CVarSlider *cvs = ( Sliders[ combo->GetActiveItemUserData()->GetInt("id") ] );
	if(cvs==this)
		cvs->SetSliderVisible(true);
	else
		cvs->SetVisible(true);
	BaseClass::OnSetText();
}
void CVarSliderCombo::SetSize(int wide, int tall)
{
	
	BaseClass::SetSize(wide, tall);
	int x,y;
	m_pSlider->GetPos(x,y);
	m_pSlider->SetPos(x,HUI_STANDARD_TALL);
	m_pLabel->GetPos(x,y);
	m_pLabel->SetPos(x,HUI_STANDARD_TALL);
	if(m_pNumeralEntry)
	{
		m_pNumeralEntry->GetPos(x,y);
		m_pNumeralEntry->SetPos(x,HUI_STANDARD_TALL);
	}
	
	for(int i = 0;i!=Sliders.Count();i++)
	{
		if(Sliders[i]!=this)
		{
			Sliders[i]->SetSize(m_pSlider->GetWide()+ ( m_pNumeralEntry ? HUI_SMALL_TEXTBOX + HUI_SLIDER_SPACE : 0 ),HUI_SLIDER_TALL);
			m_pSlider->GetPos(x,y);
			Sliders[i]->SetPos(x,y);//HUI_STANDARD_TALL);
		}
		// then offset each ( could possibly make a parent panel instead )
		
	}
	m_pName->SetPos(0,0);
	m_pName->SetWide(wide);
	
}
void CVarSliderCombo::OnCommand(const char *command)
{	
	for(int i = 0;i!= Sliders.Count();i++)
	{
		if(Sliders[i]==this)//since we actualy are entered into this loop check if we are the current in loop
			BaseClass::OnCommand(command);// if its us do our base class function to resolve looping.
		else
			Sliders[i]->OnCommand(command);
	}
}
PlacementGraph::PlacementGraph(vgui::Panel *parent, KeyValues *data) :
BaseClass(parent)
{
	int elements=1;
	HUI_DECLARE_SHARED_VARIABLES

	HUI_TABLE_START(data)

	//integers
	HUI_MATCH_INT_START
	HUI_MATCH_INT(elements)
	HUI_MATCH_END

	HUI_TABLE_END
	SetTall(tall*elements);
	SetWide(wide);
	SetPos(x,y);
	SetName(name);
	KeyValues *row=data->GetFirstTrueSubKey();

	while(row)
	{
		vgui::Panel *curPanel = ProcessKV(this, row);
		
		if(curPanel)
		{
			curPanel->SetPos(0,tall*m_controls.Count());
			curPanel->SetTall(tall);
			curPanel->SetWide(wide);
			m_controls.AddToTail(curPanel);
		}
		row = row->GetNextTrueSubKey();
		
	}
}

FloatGraph::FloatGraph(vgui::Panel *parent, int UpdateRate, float fMin, float fMax) :
BaseClass(parent)
{
	m_flMin = fMin;
	m_flMax = fMax;
	vgui::ivgui()->AddTickSignal(GetVPanel(),UpdateRate);
	InitializeLines();
};
void FloatGraph::Paint()
{
	BaseClass::Paint();
	for(int i = 0;i != m_Lines.Count();i++)
	{
		vgui::surface()->DrawSetColor(m_Lines[i]->GetColor());
		for(int w = 0; w != GetWide(); w ++)
		{
			float valuestart;
			float valueend;
			if(m_Lines[i]->GetValueAt(valuestart,w)&&m_Lines[i]->GetValueAt(valueend,w+1))
			{
				vgui::surface()->DrawLine(GetWide()-w,GetPercent(valuestart),GetWide()-(w+1),GetPercent(valueend));
			}
		}
	}
}
void FloatGraph::SetVisible(bool state)
{
	if(!state&&IsVisible())// continue only if was visible prior to this call.
	{
		for(int i = 0; i!= m_Lines.Count(); i++)
		{
			m_Lines[i]->ClearHistory();
		}
	}
	BaseClass::SetVisible(state);
}
void FloatGraph::OnTick()
{
	if(IsVisible())
	{
		for(int i = 0;i!=m_Lines.Count(); i++)
		{
			m_Lines[i]->AddValue(UpdateValues(i),GetWide());
		}
	}
}
float FloatGraph::UpdateValues(int LineNumber)
{
	// update line by thier id. example:
	// switch( m_iMyLine )
	// {
	// case 0:
	//    return sin(gpGlobals->curtime);
	// }
	return 0;
}
void FloatGraph::InitializeLines()
{
	//put in your code here to initialize lines that should be drawn using
	//AddLine("name")
	// or overload. Setting int members will keep track of what line you are
	// processing, so that you can use switch statements for each line.
	// m_iMyLine = AddLine("name");
}
float FloatGraph::GetPercent(float value)
{
	return (value-m_flMin)/(m_flMax-m_flMin);
}
int FloatGraph::AddLine( HistoryLine* line)
{
	int ID = -1;
	if(line)
	{	
		ID =  m_Lines.Count();
		m_Lines.AddToTail(line);
	}

	return ID;
}
int FloatGraph::AddLine( const char *Name, Color color)
{
	HistoryLine *line = new HistoryLine(Name,color);
	return AddLine(line);
}
int FloatGraph::AddLine( const char *Name )
{
	int r,g,b;
	r=g=b=0;
	//make us a rainbow
	for(int i = 0; i!= m_Lines.Count() % 6; i++)
	{
		switch( i )
		{
		case 0:
			r+=256;
			break;
		case 1:
			g+=256;
			break;
		case 2:
			r-=256;
			break;
		case 3:
			b+=256;
			break;
		case 4:
			g-=256;
			break;
		case 5:
			r+=256;
			break;
		}
	}
	int darkenvalue = m_Lines.Count()/6 + 1;
	r/=darkenvalue;//darkens by half.
	g/=darkenvalue;
	b/=darkenvalue;
	return AddLine(Name,Color(clamp(r,0,255),clamp(g,0,255),clamp(b,0,255)));
}
//static ( set up first time related function is called )
struct BindEntry
{
	char sz_text[40];
	char sz_bind[40];
	bool bTitle;
	BindEntry(const char *bind,const char *text,bool title = false)
	{
		V_strcpy(sz_bind,bind);
		V_strcpy(sz_text,text);
		bTitle = title;
	}
	const char *GetBind()
	{
		return (const char *)sz_bind;
	}
	const char *GetText()
	{
		return (const char *)sz_text;
	}
};
typedef CUtlVector<BindEntry*> BindData;
static BindData *BindLookUp = NULL;
static BindData *GetBindActions()
{
	if(BindLookUp==NULL)
	{
		BindLookUp = new BindData();
		char filepath[1024];
		sprintf(filepath,"%s\\scripts\\kb_act.lst",engine->GetGameDirectory());
		//FileHandle_t pFile=vgui::filesystem()->Open(filepath,"r");
		FileHandle_t pFile = filesystem->Open(filepath, "r");

		// clear our bindings.
		BindLookUp->RemoveAll();
		
		// adding none bind in first.
		BindLookUp->AddToTail(new BindEntry("","#GameUI_None"));
		//while(!vgui::filesystem()->EndOfFile(pFile))
		while(!filesystem->EndOfFile(pFile))
		{
			char bindline[512];
			//vgui::filesystem()->ReadLine(bindline,sizeof(bindline),pFile);
			filesystem->ReadLine(bindline,sizeof(bindline),pFile);
			CUtlVector<char *>binddata;
			V_SplitString((const char *)bindline,"\"",binddata);
			//first make sure our line is valid. should have at least 2 segments.
			// Middle members of the array are ignored, as they are probably tabs.
			if(binddata.Count()>=2)
			{
				//skip "blank" entries.
				if(V_stricmp(binddata[0],"blank")!=0)
				{
					BindLookUp->AddToTail( new BindEntry((const char *)binddata[0], (const char *)binddata[binddata.Count()-2]) );
				}else{
					if(V_stricmp(binddata[binddata.Count()-2],"==========================")!=0)// if its a seperator of sorts, skip it.
					{
						BindLookUp->AddToTail( new BindEntry((const char *)binddata[0], (const char *)binddata[binddata.Count()-2], true) );
					}
				}			
			}
		}
	}
	return BindLookUp;
}
ButtonBindCombo::ButtonBindCombo(vgui::Panel *parent,int keydef, const char *name, const char *image, const char *defaultAction ) :
BaseClass(parent,name)
{
	m_pDefaultAct = defaultAction;
	SetPaintEnabled(true);
	m_iKey = (ButtonCode_t)keydef;
	//m_pKey = gameuifuncs->Key_NameForKey(m_iKey);
	//m_pKey = engine->Key_BindingForKey(m_iKey);
	//CITY17: FIX ME
	m_pKey = "KEY_FALCON1";

	if(!m_pKey)
		Warning("%s has UNKNOWN keydef of %i. Aborting.\n",keydef);
	else
	{
		char tempname[32];
		
		// initialize controls.
		V_snprintf(tempname,sizeof(tempname),"buttonlabel%i",keydef);
		m_pLabel = new vgui::Label(this,tempname,name);
		
		V_snprintf(tempname,sizeof(tempname),"buttonimage%i",keydef);
		m_pImage = new vgui::ImagePanel(this,tempname);
		
		V_snprintf(tempname,sizeof(tempname),"buttoncombo%i",keydef);
		m_pCombo = new vgui::ComboBox(this,tempname,0,false);

		m_pImage->SetImage(image);
		m_pImage->SetShouldScaleImage(false);
		//now that our controls are initialized size in
		SetSize(480-20,HUI_STANDARD_TALL);	

		//Fill combo with posible binds
		BindData *BindKeys =GetBindActions();
		for(int i = 0;i!=BindKeys->Count();i++)
		{
			// kind of complicated. This is just one line but it sets in the key data, and name 
			// while also enabling or disabling the item based on whether its a section title.
			m_pCombo->SetItemEnabled( 
				m_pCombo->AddItem( BindKeys->Element(i)->GetText(), 
					new KeyValues("BindData","Bind",BindKeys->Element(i)->GetBind()) ),
				!BindKeys->Element(i)->bTitle);
		}
	}
}
void ButtonBindCombo::SetSize(int wide, int tall)
{
	//size up the panel as ushual.
	BaseClass::SetSize(wide, tall);
	
	// now place the controlls (left to right)
	// now resize the image to 24 x 24 ( crop, dont scale the image )
	m_pImage->SetBounds(0,0,24,24);
	m_pImage->SetVisible(true);
	// then move the lable to the right of the image ( 5 is pixles between button icon )
	m_pLabel->SetBounds(m_pImage->GetWide()+5,0,80,24);
	// get the current position of this train like control
	int width = (m_pLabel->GetWide()+m_pImage->GetWide()+5);
	if(width>wide)
	{
		Warning("Button bind combo scaled too small.\n");
		return;
	}
	// set in combo.
	m_pCombo->SetBounds(width,0,wide-width,24);
}
void ButtonBindCombo::AlignBinds()
{
	// set in the None entry just incase we dont find anything bound to our key.
	m_pCombo->ActivateItem(0);// 0 should always be the None entry.

	// grab our bind
	//const char *kAct = gameuifuncs->Key_BindingForKey( &m_iKey );
	//City17: REMEMBER TO FIX ME.
	//const char *kKey = engine->Key_BindingForKey( m_iKey );
	//const char *kAct = engine->Key_LookupBinding( kKey );
	const char *kAct = engine->Key_BindingForKey( m_iKey );

	// if in fact we do have no bind, just continue to the next loop.
	if(kAct==NULL)
	{
		//Msg("could not find anything bound to %s\n",m_pKey);
		return;
	}
	for(int i = 0;i != m_pCombo->GetItemCount();i++)
	{
		//first check if the key is enabled.
		if(!m_pCombo->GetItemUserData(i))// if its not continue the loop.
			continue;

		if(!V_stricmp(kAct, m_pCombo->GetItemUserData(i)->GetString("Bind")))
		{
			// we found the item. now active it and break the loop off.
            m_pCombo->ActivateItem(i);
			break;
		}
	}
}

void ButtonBindCombo::SaveBinds()
{
	if(m_pCombo->GetActiveItemUserData()==NULL)// should never happen but incase we have a disabled item selected return
		return;
	char line[128];
	if(m_pCombo->GetActiveItem()==0)
	{
		V_snprintf(line,sizeof(line),"unbind %s",m_pKey);
	}else{
		V_snprintf(line,sizeof(line),"bind %s \"%s\"",m_pKey,m_pCombo->GetActiveItemUserData()->GetString("Bind"));
	}
	RunClientCommand((const char *)line);
}
void ButtonBindCombo::SetBind(const char *action)
{
	if(action == NULL)
	{
		action = m_pDefaultAct;
	}
	m_pCombo->ActivateItemByRow(0);
	for(int i = 0;i != m_pCombo->GetItemCount();i++)
	{
		//first check if the key is enabled.
		if(!m_pCombo->GetItemUserData(i))// if its not continue the loop.
			continue;

		if(!V_stricmp(action, m_pCombo->GetItemUserData(i)->GetString("Bind")))
		{
			// we found the item. now active it and break the loop off.
			m_pCombo->ActivateItem(i);
			break;
		}
	}
}
void ButtonBindCombo::OnCommand(const char *command)
{
	if(!V_stricmp(command,SAVE_CURRENT_DATA))
		SaveBinds();
	else if(!V_stricmp(command,READ_CURRENT_DATA))
		AlignBinds();
	else if(!V_stricmp(command,READ_DEFAULT_DATA))
		SetBind();
	else 
		BaseClass::OnCommand(command);
}

void ButtonBindCombo::Paint()
{
	//draw a little box
	BaseClass::Paint();
	vgui::surface()->DrawSetColor(0,0,0,200);
	int x, y, wide, tall;
	GetBounds(x,y,wide,tall);
	vgui::surface()->DrawOutlinedRect(x,y,x+wide,y+tall);
}

ControlBoxVisual::ControlBoxVisual(vgui::Panel *parent,CVarSlider *n, CVarSlider *r, CVarSlider *u, CVarSlider *f, CVarSlider *l, CVarSlider *d) :
BaseClass(parent)
{
	m_iMouseOver = -1;
	m_flTime=0;

	//disable mouse input.
	SetMouseInputEnabled(false);

	SlideValues = new CVarSliderCube(n, r, u, f, l, d);

	SlideValues->Down->AddActionSignalTarget(GetVPanel());
	SlideValues->Far->AddActionSignalTarget(GetVPanel());
	SlideValues->Up->AddActionSignalTarget(GetVPanel());
	SlideValues->Near->AddActionSignalTarget(GetVPanel());
	SlideValues->Left->AddActionSignalTarget(GetVPanel());
	SlideValues->Right->AddActionSignalTarget(GetVPanel());
	SetWide(64);
	SetTall(64);
}

void ControlBoxVisual::OnSlideEnter(KeyValues*data)
{
	vgui::VPANEL fromPanel = data->GetInt("VPANEL");
	if(fromPanel == SlideValues->Up->GetVPanel())
		m_iMouseOver = HUI_BOX_UP;
	else if(fromPanel == SlideValues->Down->GetVPanel())
		m_iMouseOver = HUI_BOX_DOWN;
	else if(fromPanel == SlideValues->Right->GetVPanel())
		m_iMouseOver = HUI_BOX_RIGHT;
	else if(fromPanel == SlideValues->Left->GetVPanel())
		m_iMouseOver = HUI_BOX_LEFT;
	else if(fromPanel == SlideValues->Far->GetVPanel())
		m_iMouseOver = HUI_BOX_FAR;
	else if(fromPanel == SlideValues->Near->GetVPanel())
		m_iMouseOver = HUI_BOX_NEAR;
}
void ControlBoxVisual::OnSlideExit(KeyValues*data)
{
	//vgui::VPANEL fromPanel = data->GetInt("VPANEL");
	m_iMouseOver=-1;
}
void ControlBoxVisual::Paint()
{
	m_flTime+=gpGlobals->absoluteframetime;
	BaseClass::Paint();
	vgui::surface()->DrawSetColor(0,0,0,255);
	DrawCube();
	vgui::surface()->DrawSetColor(255,255,255,255);

	//first draw the cube once.
	DrawCube(	SlideValues->Near->GetSlidePercent()*-1,
				SlideValues->Right->GetSlidePercent()*-1,
				SlideValues->Up->GetSlidePercent()*-1,
				SlideValues->Far->GetSlidePercent(),
				SlideValues->Left->GetSlidePercent(),
				SlideValues->Down->GetSlidePercent());
	//then check if we have something selected
	if(m_iMouseOver!=-1)
	{
		vgui::surface()->DrawSetColor(255,0,0,255);
		// if we do, draw a special cube.
		DrawCube(	SlideValues->Near->GetSlidePercent()*-1,
					SlideValues->Right->GetSlidePercent()*-1,
					SlideValues->Up->GetSlidePercent()*-1,
					SlideValues->Far->GetSlidePercent(),
					SlideValues->Left->GetSlidePercent(),
					SlideValues->Down->GetSlidePercent(),
					m_iMouseOver);


	}
};

void ControlBoxVisual::DrawCube(float n, float r, float u, float f, float l, float d, int specialside)
{
	

	l*=-1;
	r*=-1;//flip



	Vector right[4];
	//right side
	right[0]= Vector(f,r,d);
	right[1]= Vector(f,r,u);
	right[2]= Vector(n,r,u);
	right[3]= Vector(n,r,d);

	Vector left[4];
	//left side
	left[0]= Vector(f,l,d);
	left[1]= Vector(f,l,u);
	left[2]= Vector(n,l,u);
	left[3]= Vector(n,l,d);
	//yikes, this is kind of alot of typing.
	switch(specialside)
	{
	case HUI_BOX_UP:
		left[0] = left[1];
		left[3] = left[2];
		left[1] =Vector(1,-1,-1);
		left[2] =Vector(-1,-1,-1);
		right[0]= right[1];
		right[3]= right[2];
		right[1]=Vector(1,1,-1);
		right[2]=Vector(-1,1,-1);
		break;
	case HUI_BOX_DOWN:
		left[1] = left[0];
		left[2] = left[3];
		left[0] =Vector(1,-1,1);
		left[3] =Vector(-1,-1,1);
		right[1] = right[0];
		right[2] = right[3];
		right[0] =Vector(1,1,1);
		right[3] =Vector(-1,1,1);
		break;
	case HUI_BOX_LEFT:
		right[0] = left[0];
		right[1] = left[1];
		right[2] = left[2];
		right[3] = left[3];
		left[0] =Vector(1,-1,1);
		left[1] =Vector(1,-1,-1);
		left[2] =Vector(-1,-1,-1);
		left[3] =Vector(-1,-1,1);
		break;
	case HUI_BOX_RIGHT:
		left[0] = right[0];
		left[1] = right[1];
		left[2] = right[2];
		left[3] = right[3];
		right[0] =Vector(1,1,1);
		right[1] =Vector(1,1,-1);
		right[2] =Vector(-1,1,-1);
		right[3] =Vector(-1,1,1);
		break;
	case HUI_BOX_FAR:
		left[3] = left[0];
		left[2] = left[1];
		left[0] =Vector(1,-1,1);
		left[1] =Vector(1,-1,-1);
		right[3] = right[0];
		right[2] = right[1];
		right[0] =Vector(1,1,1);
		right[1] =Vector(1,1,-1);
		break;
	case HUI_BOX_NEAR:
		left[0] = left[3];
		left[1] = left[2];
		left[3] =Vector(-1,-1,1);
		left[2] =Vector(-1,-1,-1);
		right[0] = right[3];
		right[1] = right[2];
		right[3] =Vector(-1,1,1);
		right[2] =Vector(-1,1,-1);
		break;
	default:
		break;
	}

	Vector pos = Vector(0,0.25 + sin(m_flTime),0.5);
	VMatrix Project = SetupMatrixProjection(pos,VPlane(Vector(1,0,0),-3));
	Vector vdrawsize = Vector(1,(float)GetWide()/10.0f,(float)GetTall()/10.0f);
	for(int i=0;i!=4;i++)
	{
		right[i] = Project.VMul3x3(right[i])*vdrawsize;
		left[i] = Project.VMul3x3(left[i])*vdrawsize;
	}
	vdrawsize *=5;
	for(int i = 0;i!=4;i++)
	{
		int next = i+1;
		if(next>3)
			next = 0;
		vgui::surface()->DrawLine(vdrawsize.y+left[i].y,vdrawsize.z+left[i].z,vdrawsize.y+left[next].y,vdrawsize.z+left[next].z);
		vgui::surface()->DrawLine(vdrawsize.y+right[i].y,vdrawsize.z+right[i].z,vdrawsize.y+right[next].y,vdrawsize.z+right[next].z);
		vgui::surface()->DrawLine(vdrawsize.y+left[i].y,vdrawsize.z+left[i].z,vdrawsize.y+right[i].y,vdrawsize.z+right[i].z);
	}
};
vgui::Panel *ProcessKV(vgui::Panel *parent, KeyValues *curKey)
{
	vgui::Panel *newEntry = NULL;
	switch(GetUIType(curKey->GetString("type")))
	{
	case HUI_SLIDER:
		newEntry = CVarSlider::CreateFromKeyValues(parent,curKey);
		break;
	case HUI_ROWS:
		newEntry = new PlacementGraph(parent,curKey);
	default:
		break;
	}
	return newEntry;
}