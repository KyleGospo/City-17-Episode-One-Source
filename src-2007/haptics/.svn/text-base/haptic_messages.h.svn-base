// Haptics --------------------------------------------------------------------
// Purpose: This header has all the static functions for reading incoming
// messages sent to the client from the server.
//
// Also at the very bottom of this header, there is a static function named
// HookHapticMessages()
// This function hooks all the static functions that deal with reading the data.
// 
// If you do add more effects and haptic related user messages:
//
// #1 : first register your new message type in "shared_haptic_registers.h".
// you can specify the size in bytes of the message or put -1 for dynamic size.
// EXAMPLE: #MESSAGE_TYPE_NAME_HERE# being what you choose to name your message
//          and #SIZE_OF_MESSAGE# being the size of the message.
// usermessages->Register( "#MESSAGE_TYPE_NAME_HERE", SIZE_OF_MESSAGE );
//
// #2 : create a static message function like so:
// EXAMPLE:
// static void _MsgFunc_#MESSAGE_TYPE_NAME_HERE#( bf_read &msg )
//
// #3 : code in how the buffer should be read and what to do with the decoded
// variables.
// 
// #4 : Add a line to the HookHapticMessages() containing a hook to the type
// EXAMPLE:
// HOOK_MESSAGE( #MESSAGE_TYPE_NAME_HERE# ); //( no quotes around type name )
//
// #5 : Test compile and test your new message type. and example of how to send
// these messages to clients can be found in baseplayer_shared.cpp inside
// the following functions.
// CBasePlayer::HapticsStep
// CBasePlayer::HapticsPunch
// CBasePlayer::HapticsDamage
//
// note: This is a example of how to directly start/update effects from the 
// server side. most of these effects can be kept 100% client side via 
// animation events (ACT_) for weapon/temporary effects or sampling client 
// side data for constant/always running effects such as moving forces.
// 
// an example of a constant/always running effect kept totaly client side is
// the player forces. While the vehicle forces and player forces are very 
// similarly processed the player forces work off client side data while 
// vehicle forces are sent to the client directly from the server.
//
// note: the optimized predef has not been tested and may not work.
//-----------------------------------------------------------------------------
#include "usermessages.h"

// Haptics Hook functions

static void __MsgFunc_HapStep( bf_read &msg )
{
	if(haptics==NULL)
		return;

	float strength;
	byte type;
#ifdef HAP_NETWORK_OPTIMIZE
	strength = (float)msg.ReadByte()/128.0f;
	type = msg.ReadByte();
#else
	strength = msg.ReadFloat();
	type = msg.ReadByte();
#endif // HAP_NETWORK_OPTIMIZE

	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		pPlayer->HapticsStep(type,strength);
	}
}

static void __MsgFunc_HapPunch( bf_read &msg )
{
	if(haptics==NULL)
		return;

	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		// Haptics Our punch strength
		float Strength = msg.ReadFloat();

		// Haptics the punch angle
		QAngle AngleOffset;
		AngleOffset.Init();
#ifdef HAP_NETWORK_OPTIMIZE
		msg.ReadBitAngles(AngleOffset);
#else
		AngleOffset[PITCH] = msg.ReadFloat();
		AngleOffset[YAW] = msg.ReadFloat();
		AngleOffset[ROLL] = msg.ReadFloat();
#endif // HAP_NETWORK_OPTIMIZE
		// Haptics Do the punch
		pPlayer->HapticsPunch(AngleOffset,Strength);
	}
}

static void __MsgFunc_HapDmg( bf_read &msg )
{
	if(haptics==NULL)
		return;

	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		float pitch, yaw, damage;
#ifdef HAP_NETWORK_OPTIMIZE
		QAngle Angles;
		Angles.Init();
		msg.ReadBitAngles(Angles);
		pitch = Angles[PITCH];
		yaw = Angles[YAW];
		damage = (float) msg.ReadByte();
#else
		pitch = msg.ReadFloat();
		yaw = msg.ReadFloat();
		damage = msg.ReadFloat();
#endif //HAP_NETWORK_OPTIMIZE
		haptics->TriggerDamageEffect(QAngle(pitch,yaw,0),damage);
	}
}

static void __MsgFunc_HapMassOn( bf_read &msg )
{
	if(haptics==NULL)
		return;

	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		float mass = msg.ReadFloat();
		pPlayer->HapticsMass(mass);
	}
}

static void __MsgFunc_HapMassOff( bf_read &msg )
{
	if(haptics==NULL)
		return;
	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		pPlayer->HapticsMass(0);
	}
}

static void __MsgFunc_HapVehicleEnter( bf_read &msg )
{
	if(haptics==NULL)
		return;

	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		pPlayer->HapticsVehicleStart((int)msg.ReadByte());
	}
}

static void __MsgFunc_HapVelocityUpdate( bf_read &msg )
{
	if(haptics==NULL)
		return;

	Vector Velocity;
	Velocity.Init();
#ifdef HAP_NETWORK_OPTIMIZE
	msg.ReadBitVec3Coord( Velocity );
#else
	Velocity.x = msg.ReadFloat();
	Velocity.y = msg.ReadFloat();
	Velocity.z = msg.ReadFloat();
#endif // HAP_NETWORK_OPTIMIZE
	C_BasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if(pPlayer)
	{
		pPlayer->HapticsVelocityUpdate(Velocity);
	}
}

static void HookHapticMessages()
{
	HOOK_MESSAGE( HapPunch );
	HOOK_MESSAGE( HapStep );
	HOOK_MESSAGE( HapDmg );
	HOOK_MESSAGE( HapMassOn );
	HOOK_MESSAGE( HapMassOff );
	HOOK_MESSAGE( HapVehicleEnter );
	HOOK_MESSAGE( HapVelocityUpdate );
}