//note: to use these effects you must exclude c_rumble.cpp from you build!

//there has been edits to some of the xbox code calls int he code to enable them in the pc build. here they are hooked
// into the haptics instead.

// this is mainly for the shake effect.
#include "cbase.h"
#include "c_rumble.h"
#include "rumble_shared.h"
#include "inputsystem/iinputsystem.h"

#include "client_haptics.h"

#include "tier0/memdbgon.h"

static float s_flShake = 0;
void RumbleEffect( unsigned char effectIndex, unsigned char rumbleData, unsigned char rumbleFlags )
{
//normaly for the xbox build this would be called with data from the server regarding what sort of rumble effect is being sent.
//currently this will not be called everywhere that it would normaly be called on the xbox build so do not rely on this unless
//you remove the xbox macro checks for all the rumble effects server side and client side.
}

void EnableRumbleOutput(bool bEnable)
{
//do nothing for haptics. We are always on.
}
void StopAllRumbleEffects()
{
	s_flShake = 0;
}
void UpdateRumbleEffects()
{
	//for now lets just send some punches to our haptics.
	if(s_flShake > 0)
	{
		//temporarily just send a damage effect.
		haptics->TriggerVerticalShakeEffect(s_flShake);
	}
}
void UpdateScreenShakeRumble(float shake, float balance )
{
	s_flShake = shake;
}