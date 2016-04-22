#include "CBase.h"
#include "HapticPages.h"
#include <KeyValues.h>
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"
// Base class for our pages in the haptics panel.
CHapticsPage::CHapticsPage(vgui::PropertySheet *parent, KeyValues *pageData) :
BaseClass(parent,"hapticpage")
{
	SetName(GetTabTitle());
	m_pKeys = pageData;
	SetVisible(false);
	//parent->AddPage(this,GetTabTitle()) move this to new classes extending of this.
}
const char *CHapticsPage::GetTabTitle()
{
	return "unset";
}
void CHapticsPage::AddCVarControl( Panel *control )
{
	if(control==NULL)
		return;

	if(!m_controls.HasElement(control))
	{
		m_controls.AddToTail(control);
	}
}
void CHapticsPage::SaveVars()
{
	for( int i = 0; i != m_controls.Count(); i++ )
	{
		m_controls[i]->OnCommand(SAVE_CURRENT_DATA);
	}
}
void CHapticsPage::LoadDefaultVars()
{
	for( int i = 0; i != m_controls.Count(); i++ )
	{
		m_controls[i]->OnCommand(READ_DEFAULT_DATA);
	}
}
void CHapticsPage::ResetVars()
{
	for( int i = 0; i != m_controls.Count(); i++ )
	{
		m_controls[i]->OnCommand(READ_CURRENT_DATA);
	}
}
void CHapticsPage::OnCommand(const char *command)
{
	if( (!V_stricmp(command,"ok")) || (!V_stricmp(command,"apply")) )
	{
		SaveVars();
	}
	else if( !V_stricmp(command,"cancel") )
	{
		ResetVars();
	}
	else if( !V_stricmp(command,"default") )
	{
		LoadDefaultVars();
	}else
		BaseClass::OnCommand(command);

}

void CHapticsPage::InitControls()
{
	// here would be the control placement code.
	//m_pKeys = new KeyValues(LookUpName);
	//if(m_pKeys->LoadFromFile(vgui::filesystem(),HAPTIC_UI_FILE,"MOD"))
	{
		//m_pKeys = m_pKeys->FindKey(LookUpName);
		if(m_pKeys)
		{
			Msg("woo");
			//Process Keys
			KeyValues *curKey = m_pKeys->GetFirstTrueSubKey();
			while(curKey)
			{

			Msg(curKey->GetName());
				AddCVarControl( ProcessKV(this,curKey) );
				
				curKey = curKey->GetNextTrueSubKey();
			}

		}
		else
		{
			Warning("Could not find key %s in UI file %s",m_pKeys->GetName(),HAPTIC_UI_FILE);
		}
	}//else{
		//Warning("Could not find UI file! ( %s )\n",HAPTIC_UI_FILE);
	//}
};
void CHapticsPage::OnPageShow()
{
	ResetVars();
}