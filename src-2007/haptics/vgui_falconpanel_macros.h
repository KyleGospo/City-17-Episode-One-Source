/*---------------------------------------------------------------------------*
/ File Name: vgui_falconpanel_macros.h
/ Author: Anthony Iacono
/ Ending Date: April 19, 2007
/ Ending Time: 5:51 PM - Pacific Time (GMT -08:00)
/ Ideal File Location: src\cl_dll\VGUIFalcon\vgui_falconpanel_macros.h
/ Description: Some very important macros that are better suitted in a seperate
/ file, just makes things a little more organized
\*---------------------------------------------------------------------------*/

#define ElementInvisible(pointer)\
	ignore=0;\
	if(##pointer##)\
	##pointer##->SetVisible(false)
#define ElementVisible(pointer)\
	ignore=0;\
	if(##pointer##)\
	##pointer##->SetVisible(true)

#define ThinkForSliderTextEntry(slider,textentry,oldslidervalue,oldtextentryvalue,mintextentryvalue,maxtextentryvalue)\
	if(##slider##)\
	{\
		if(##textentry##)\
		{\
			char tmpstr[256];\
			##textentry##->GetText(tmpstr,256);\
			if(!FStrEq(tmpstr,""))\
			{\
				tmpstr[256];\
				##textentry##->GetText(tmpstr,256);\
				char oldtmp[256];\
				sprintf(oldtmp,"%s",tmpstr);\
				int txtvalue=atoi(tmpstr);\
				if(txtvalue>##maxtextentryvalue##)\
					txtvalue=##maxtextentryvalue##;\
				if(txtvalue<##mintextentryvalue##)\
					txtvalue=##mintextentryvalue##;\
				sprintf(tmpstr,"%i",txtvalue);\
				if(!FStrEq(oldtmp,tmpstr))\
					##textentry##->SetText(tmpstr);\
				int sldrvalue=##slider##->GetValue();\
				if(sldrvalue==##oldslidervalue## && txtvalue!=##oldtextentryvalue##)\
				{\
					##slider##->SetValue(txtvalue);\
					##oldtextentryvalue##=txtvalue;\
					##oldslidervalue##=txtvalue;\
				}\
				else if(sldrvalue!=##oldslidervalue## && txtvalue==##oldtextentryvalue##)\
				{\
					sprintf(tmpstr,"%i",sldrvalue);\
					##textentry##->SetText(tmpstr);\
					##oldtextentryvalue##=sldrvalue;\
					##oldslidervalue##=sldrvalue;\
				}\
			}\
			else\
			{\
				if(##slider##->GetValue()!=##oldslidervalue##)\
				{\
					sprintf(tmpstr,"%i",##slider##->GetValue());\
					##textentry##->SetText(tmpstr);\
					##oldtextentryvalue##=##slider##->GetValue();\
					##oldslidervalue##=##slider##->GetValue();\
				}\
			}\
		}\
	}

#define ThinkForSliderTextEntry_array(basename,indicecount)\
	for(int x=0;x<##indicecount##;x++)\
	{\
		ThinkForSliderTextEntry(m_##basename##Slider[x],\
								m_##basename##TextEntry[x],\
								m_##basename##Slider_OldValue[x],\
								m_##basename##TextEntry_OldValue[x],\
								m_##basename##TextEntry_MinValue[x],\
								m_##basename##TextEntry_MaxValue[x]);\
	}

#define InitSliderTextEntry(basename)\
	vgui::Slider *m_##basename##Slider;\
	vgui::TextEntry *m_##basename##TextEntry;\
	int m_##basename##Slider_OldValue;\
	int m_##basename##TextEntry_OldValue;\
	int m_##basename##TextEntry_MinValue;\
	int m_##basename##TextEntry_MaxValue;

#define InitSliderTextEntry_array(basename,indicecount)\
	vgui::Slider *m_##basename##Slider[##indicecount##];\
	vgui::TextEntry *m_##basename##TextEntry[##indicecount##];\
	int m_##basename##Slider_OldValue[##indicecount##];\
	int m_##basename##TextEntry_OldValue[##indicecount##];\
	int m_##basename##TextEntry_MinValue[##indicecount##];\
	int m_##basename##TextEntry_MaxValue[##indicecount##];

#define NullSliderTextEntry(basename)\
	m_##basename##Slider=NULL;\
	m_##basename##TextEntry=NULL;\
	m_##basename##Slider_OldValue=0;\
	m_##basename##TextEntry_OldValue=0;\
	m_##basename##TextEntry_MinValue=0;\
	m_##basename##TextEntry_MaxValue=0;

#define NullSliderTextEntry_array(basename,indicecount)\
	for(x=0;x<##indicecount##;x++)\
	{\
		m_##basename##Slider[##indicecount##]=NULL;\
		m_##basename##TextEntry[##indicecount##]=NULL;\
		m_##basename##Slider_OldValue[##indicecount##]=0;\
		m_##basename##TextEntry_OldValue[##indicecount##]=0;\
		m_##basename##TextEntry_MinValue[##indicecount##]=0;\
		m_##basename##TextEntry_MaxValue[##indicecount##]=0;\
	}

#define InitWeaponRecoilTable()\
	int m_WeaponRecoilComboBox_357;\
	int m_WeaponRecoilComboBox_AR2;\
	int m_WeaponRecoilComboBox_Crossbow;\
	int m_WeaponRecoilComboBox_Crowbar;\
	int m_WeaponRecoilComboBox_GravityGun;\
	int m_WeaponRecoilComboBox_Pistol;\
	int m_WeaponRecoilComboBox_Stunstick;\
	int m_WeaponRecoilComboBox_Shotgun;\
	int m_WeaponRecoilComboBox_SMG;

#define FS vgui::filesystem()