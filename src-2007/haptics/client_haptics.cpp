
#include "cbase.h"
#include "client_haptics.h"
// for control of haptics device.
#include "in_haptics.h"
// to receive messages from server.
#include "hud_macros.h"

#include "in_haptics_hl2.h"

#include "tier0/memdbgon.h"

static ConVar hap_shake_time("hap_shake_time","20",FCVAR_ARCHIVE);
static ConVar hap_shake_peakpct("hap_shake_peakpct","0.25",FCVAR_ARCHIVE);
static ConVar hap_shake_scale("hap_shake_scale","0.5",FCVAR_ARCHIVE);
static ConVar hap_shake_direction("hap_shake_direction","2",FCVAR_ARCHIVE,"0=always down\n1=always up\n2=alternate\n3=random");
static ConVar hap_debug_shake("hap_debug_shake","0",FCVAR_ARCHIVE);
static ConVar hap_shake("hap_shake","1",FCVAR_ARCHIVE,"enable haptic shake effects");
static bool s_bLastShakeDirectionDown=false;

class CHapticsInterface : public IHaptics
{
public:
	CHapticsInterface()
	{
		// Haptics Hook Messages for steps and punches. ( the functions for these are in "haptic_messages.h" )

	}
	bool IsConnected()
	{
		if(cliHaptics)
		{
			// code here to check if connection is ready.
			return true;
		}
		return false;
	}
	void TriggerPunchEffect(QAngle direction, float strength )
	{
		if(cliHaptics)
		{
			cliHaptics->DoRecoil(direction,strength);
		}
	}
	void TriggerFootStepEffect(byte type, float strength )
	{
		if(cliHaptics)
		{
			cliHaptics->DoFootstep((StepType_t)type,strength);
		}
	}
	void TriggerDamageEffect(QAngle direction, float damage)
	{
		if(cliHaptics)
		{
			cliHaptics->DoDamageSimulation(direction[PITCH],direction[YAW],damage);
		}
	}
	//example of reusing effects without editing in_haptics
	void TriggerVerticalShakeEffect(float strength)
	{
		if(hap_shake.GetInt()==0)
			return;
		float original = strength;
		if(cliHaptics)
		{
			switch(hap_shake_direction.GetInt())
			{
			case 3:
				strength *= random->RandomInt(0,1)==1? 1.0f : -1.0f;
				break;
			case 2:
				s_bLastShakeDirectionDown = !s_bLastShakeDirectionDown;
				strength *= s_bLastShakeDirectionDown ? 1.0f : -1.0f;
				break;
			case 0:
				strength *= -1.0f;
			case 1:
			default:
				break;
			}
			strength*=hap_shake_scale.GetFloat();
			if(hap_debug_shake.GetInt()!=0)
			{
				Msg("Shake = [ %f ( %f )]\n", strength, original);
			}			
		
			//using the damage effect but with our variables.
			cliHaptics->TriggerEffect( 2,//Damage
				strength,
				0,
				1,
				0,
				hap_shake_time.GetInt(), 
				hap_shake_peakpct.GetFloat());
		}
	};
	void TriggerMassEffect(float mass)
	{
		if(cliHaptics)
		{
			cliHaptics->SetHeldMass(mass);
		}
	}
	void ProcessVelocitySample(Vector velocity)
	{
		if(cliHaptics)
		{
			cliHaptics->ProcessVelocitySample(velocity);
		}
	}
	void SetVehicleMode(int type)
	{
		if(cliHaptics)
		{
			C_HapticsHL2*hl2Haptics = dynamic_cast<C_HapticsHL2*>(cliHaptics);
			if(hl2Haptics)
				hl2Haptics->SetVehicleMode(type);
		}
	}
	void ResetEffects()
	{
		if(cliHaptics)
		{
			cliHaptics->ResetSamples();
			//TODO: make this terminate all effects
		}
	}
};

static CHapticsInterface g_HapticInterface;
IHaptics* haptics = (IHaptics*)&g_HapticInterface;