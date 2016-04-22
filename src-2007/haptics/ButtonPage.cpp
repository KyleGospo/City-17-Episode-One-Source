#include "cbase.h"
#include "hapticPages.h"
//#include "keydefs.h"
#include "inputsystem/ButtonCode.h"
#include "tier0/memdbgon.h"

static const char *buttonlables[]=
{
	"#HapButton1",
	"#HapButton2",
	"#HapButton3",
	"#HapButton4",
};
static const char *buttonimages[]=
{
	"falcon/button1",
	"falcon/button2",
	"falcon/button3",
	"falcon/button4",
};

static const char *buttondefaults[]=
{
	"+attack",
	"invprev",
	"+attack2",
	"invnext",
};

CHapticsButtonPage::CHapticsButtonPage(vgui::PropertySheet *parent, KeyValues *pageData) :
BaseClass(parent, pageData)
{
	parent->AddPage(this,GetTabTitle());
	for (int i = 0; i != 4; i ++ )
	{
		ButtonBindCombo *current = new ButtonBindCombo(this, KEY_HAPTIC1+i, buttonlables[i], buttonimages[i], buttondefaults[i]);
		ButtonCombos.AddToTail(current);
		AddCVarControl(current);
		current->SetPos(20,68+i*28);
	}
	LoadControlSettings("resource/UI/FalconButtons.res");
	SetVisible(false);
}
void CHapticsButtonPage::InitControls()
{
	//m_pInstructionLable = new vgui::Label(this,"buttonpagelable","#HapButtonInstructions");
	//m_pInstructionLable->SetPos(20,20);
}
const char *CHapticsButtonPage::GetTabTitle()
{
	return "#HapButtons";
}