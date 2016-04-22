//UI CLASSES JUST FOR THE DEMO VERSION OF HAPTICS LIFE 2

#include "cbase.h"
#include "ienginevgui.h"
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Controls.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui/isystem.h>
#include "haptics_demo_ui.h"
#include "../kioskfiles/kiosk_instructionPanel.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
//TIMER: Set convar sleep_timer to a non zero positive number(seconds) and this panel will be created
// and game will shut down once the amount of seconds has been hit.
CSleepTimer *g_pSleepTimer = NULL;
///Timer for demo usage (no more than 99 minutes)
void sleep_timer_set( IConVar *pConVar, const char *pOldString, float flOldValue );
ConVar sleep_timer("sleep_timer","-1",0,"set to amount of time (seconds) before game is shutdown",true,-1,true,99*60,sleep_timer_set);

CSleepTimer::CSleepTimer(vgui::VPANEL parent) : BaseClass(NULL,"sleep_timer"){
	if(g_pSleepTimer==NULL)
	{
		SetParent(parent);
		SetSize(270,320);
		SetPos(5,5);
		m_pMinutes = new vgui::Label(this,"minutes","00:00");
		//m_pSeconds = new vgui::Label(this,"seconds","00");
		
		m_pMinutes->SetPos(5,24);
		m_pMinutes->SetWide(256);	
		//m_pSeconds->SetPos(5+m_pMinutes->GetWide(),24);
		//m_pSeconds->SetWide(m_pMinutes->GetWide());
		vgui::Label *title = new vgui::Label(this,"title","Time Remaining");
		//vgui::Label *colon = new vgui::Label(this,"c",":");
		//colon->SetPos(1+m_pMinutes->GetWide(),24);
		title->SetWide(GetWide());
		SetTitle("DEMO",true); 
		SetVisible(sleep_timer.GetInt()!=-1);
		SetMouseInputEnabled(false);
		SetMoveable(false);
		SetSizeable(false);
		SetPaintBackgroundEnabled(false);
		SetCloseButtonVisible(false);
		SetKeyBoardInputEnabled(false);
		timeUp = new CTimerUpPanel(parent);
	}
}
ConVar sleep_timer_stopped("sleep_timer_stopped","0");
static bool bRanOnceSleepTime = false;
static int s_gameUpTime = 0;
void CSleepTimer::OnTick(){
	if(!bRanOnceSleepTime)
	{
		
		#ifdef HAP_DEMO
		if(instPanel)
		{
			vgui::ImagePanel *pImage = new vgui::ImagePanel(this,"In-game Image");
			pImage->SetImage(instPanel->GetIngameImageURL());
			int x;
			int y;
			m_pMinutes->GetPos(x,y);
			pImage->SetPos(x,48);
			bRanOnceSleepTime = true;
		}
#endif
		
	}
	if(sleep_timer_stopped.GetInt()!=0)
	{
		m_pMinutes->SetText("#kiosk_times_up");
		vgui::ivgui()->RemoveTickSignal(GetVPanel());
		return;
	}
	if(sleep_timer.GetInt()==0)
	{
		timeUp->ExitGame();
		sleep_timer.SetValue(-1);
	}else if(sleep_timer.GetInt()>0)
	{
		char time[16];
		int m = sleep_timer.GetInt()/60;
		int s = sleep_timer.GetInt()%60;
		/*
		char mc[2];
		char sc[2];
		if(s<10)
			Q_snprintf(sc,2,"0%i",s);
		else
			Q_snprintf(sc,2,"%i",s);

		if(s<10)
			Q_snprintf(mc,2,"0%i",m);
		else
			Q_snprintf(mc,2,"%i",m);
*/
		if(s>9&&m>9)
			Q_snprintf(time,16,"%i:%i",m,s);
		else if(s<10&&m>9)
			Q_snprintf(time,16,"%i:0%i",m,s);
		else if(s<10&&m<10)
			Q_snprintf(time,16,"0%i:0%i",m,s);
		else if(s>9&&m<10)
			Q_snprintf(time,16,"0%i:%i",m,s);

		m_pMinutes->SetText(time);
		sleep_timer.SetValue(sleep_timer.GetInt()-1);
	}else if(s_gameUpTime<4){
		s_gameUpTime ++;
	}else{
		engine->ClientCmd("exit\n");	

	}
};
ConVar sleep_timer_allow_reset("sleep_timer_allow_reset","1");
CTimerUpPanel::CTimerUpPanel(vgui::VPANEL parent):BaseClass(null,"times Up")
{
	SetTitle("#kiosk_times_up",true);
	SetVisible(false);
	exitMessage = new vgui::Label(this,"exitMessage","#kiosk_exitmessage");
	SetWide(256);
	exitMessage->SetPos(8,28);
	exitMessage->SetWide(256-8);
	SetMouseInputEnabled(false);
	SetMoveable(false);
	SetSizeable(false);
	SetCloseButtonVisible(false);
	SetKeyBoardInputEnabled(false);
}
void CTimerUpPanel::ExitGame()
{
	SetMouseInputEnabled(true);
	SetKeyBoardInputEnabled(true);
	SetPos((ScreenWidth()-GetWide())/2,(ScreenHeight()-GetTall())/2);
	SetVisible(true);
	vgui::ivgui()->AddTickSignal(GetVPanel(),1000);
}
static int TUpElapsed=0;
ConVar kiosk_exitTime("kiosk_exitTime","3",FCVAR_ARCHIVE);
void CTimerUpPanel::OnTick()
{
	TUpElapsed++;
	if(TUpElapsed==kiosk_exitTime.GetInt())
	{
		engine->ClientCmd("exit");		
	}

}
void timetoseconds( IConVar *pConVar, const char *pOldString, float flOldValue )
{
	ConVarRef var( pConVar );

	if( !var.IsValid() )
		return;

	CUtlVector<char *>values;
	V_SplitString(var.GetString(),":",values);
	switch(values.Count())
	{
	case 0:
	case 1:
		if((float)var.GetInt()!=var.GetFloat())
		{
			Warning("%s Rounding to seconds ( no decimals! ) %f = %i\n",var.GetName(),var.GetFloat(),(int)var.GetFloat());
			var.SetValue((int)var.GetFloat());
		}
		break;
	case 2://minutes:seconds
		var.SetValue(atoi(values[0])*60+atoi(values[1]));
		break;
	case 3://hours:minutes:seconds
		var.SetValue(atoi(values[0])*(60*60)+atoi(values[1])*60+atoi(values[2]));
		break;
	case 4://days:hours:minutes:seconds .... lol
		var.SetValue(atoi(values[0])*(60*60*24)+atoi(values[1])*(60*60)+atoi(values[2])*60+atoi(values[3]));
		break;
	case 5:
		Warning("%s I am not doing years bub.\n",var.GetName());
	default:	
		var.SetValue(atoi(pOldString));
		break;		
	}
}
//these are to keep track of time per demo!
ConVar sleep_timer_shooter("sleep_timer_shooter","300",0,"How long the shooter demo timer runs. (default = \"5:00\") \n",timetoseconds);
ConVar sleep_timer_vehicle("sleep_timer_vehicle","180",0,"How long the vehicle demo timer runs. (default = \"3:00\") \n",timetoseconds);
ConVar sleep_timer_mode("sleep_timer_mode","STARTUP");
CON_COMMAND(sleep_timer_smart,"set to either STARTUP SHOOTER or VEHICLE")
{
	
	if(args.ArgC()>1)
	{
		int mode = 0;
		int time = 0;
		if(!V_stricmp(args.Arg(1),"STARTUP")){
			mode = 1;
		}else if(!V_stricmp(args.Arg(1),"SHOOTER")){
			mode = 2;
			time = sleep_timer_shooter.GetInt();
		}else if(!V_stricmp(args.Arg(1),"VEHICLE")){
			mode = 3;
			time = sleep_timer_vehicle.GetInt();
		}
		switch(mode)
		{
		case 0:
			Warning("Invalid mode set use STARTUP SHOOTER or VEHICLE\n");
			break;
		case 1:
			Warning("No changes made\n");
			break;
		default:
			if(!V_stricmp(sleep_timer_mode.GetString(),"STARTUP"))
			{
				//first time ran.
			}else if(!V_stricmp(sleep_timer_mode.GetString(),"SHOOTER"))
			{
				Msg("Cut %s demo off with %i seconds remaining.\n",sleep_timer_mode.GetString(),sleep_timer_shooter.GetInt());
				sleep_timer_shooter.SetValue(sleep_timer.GetInt());
			}else if(!V_stricmp(sleep_timer_mode.GetString(),"VEHICLE"))
			{
				Msg("Cut %s demo off with %i seconds remaining.\n",sleep_timer_mode.GetString(),sleep_timer_shooter.GetInt());
				sleep_timer_vehicle.SetValue(sleep_timer.GetInt());			
			}
			//set in our change now
			sleep_timer_mode.SetValue(args.Arg(1));
			char cmd[80];
			V_snprintf(cmd,sizeof(cmd),"%s %i\n",sleep_timer.GetName(),time);
			engine->ClientCmd(cmd);
		}
	}
}
#define CHEATN 0
#define CHEATV 1
//           N
#define CHEATT 2
ConVar kiosk_ti("kiosk_ti","1000");
static long lastPress = 0;
static int stage = 0;
static bool checkCheatTimer()
{
	if(vgui::system()->GetTimeMillis()>lastPress+kiosk_ti.GetInt()||stage == 0)
	{
		stage = 0;
		lastPress = vgui::system()->GetTimeMillis();
	}else{
		if(stage==4)
		{
			stage = 0;
			return true;
		}
	}
	return false;
}
void sleep_timer_set( IConVar *pConVar, char const *pOldString, float flOldValue )
{
	ConVarRef var( pConVar );

	if( !var.IsValid() )
		return;

	if(pOldString==NULL||atoi(pOldString)==-1||sleep_timer_allow_reset.GetInt())
	{
		if(var.GetInt()!=-1)
		{
			if(g_pSleepTimer)
			{
				if(!g_pSleepTimer->IsVisible())
				{
					vgui::ivgui()->AddTickSignal(g_pSleepTimer->GetVPanel(),1000);
					g_pSleepTimer->SetVisible(true);
				}
			}
		}else{
			if(g_pSleepTimer)
			{
				g_pSleepTimer->SetVisible(false);
				//vgui::ivgui()->RemoveTickSignal(g_pSleepTimer->GetVPanel());
			}
		}
	}else{
		/*
		if(atoi(pOldString)!=-1)
			var->SetValue(atoi(pOldString));
			*/
	}
}
CON_COMMAND( kiosk_n, "?\n")
{
	if(checkCheatTimer())
	{
		//nvntv
		//cheat accepted!
		//add one minute
		sleep_timer.SetValue(sleep_timer.GetInt()+60);
		return;
	}
	switch(stage)
	{
	case 0:
	case 2:
		stage++;
		break;
	default:
		stage = 0;
		break;
	}
}
CON_COMMAND( kiosk_v, "?\n")
{
	if(checkCheatTimer())
	{
		//nvntv
		//cheat accepted!
		//toggle timer
		sleep_timer_stopped.SetValue(sleep_timer_stopped.GetInt()!=0?0:1);
	}
	switch(stage)
	{
	case 1:
		stage++;
		break;
	default:
		stage = 0;
		break;
	}
}
CON_COMMAND( kiosk_t, "?\n")
{
	if(checkCheatTimer())
	{
		//nvntv
		//cheat accepted!
		//reset timer
		engine->ClientCmd("exec time_settings.cfg");
		engine->ClientCmd("sleep_timer_smart");
		//sleep_timer_smart();
	}
	switch(stage)
	{
	case 3:
		stage++;
		break;
	default:
		stage = 0;
		break;
	}
}

#define KEY_CHEAT(key) \
	CON_COMMAND( kiosk_##key##, "?\n" ) \
	{ \
		if(checkCheatTimer()) \
		{ \
			engine->ClientCmd("exec "#key".cfg"); \
		} \
	}

KEY_CHEAT(b);
KEY_CHEAT(i);
KEY_CHEAT(j);
KEY_CHEAT(k);
KEY_CHEAT(l);
KEY_CHEAT(m);
KEY_CHEAT(o);
KEY_CHEAT(p);
KEY_CHEAT(u);
KEY_CHEAT(y);
