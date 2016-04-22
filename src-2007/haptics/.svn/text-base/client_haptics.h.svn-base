#ifndef CLIENT_HAPTICS_H
#define CLIENT_HAPTICS_H
#ifdef _WIN32
#pragma once
#endif

// Haptics --------------------------------------------------------------------
// the main purpose of this is to provied a clean interface to just the effect
// related features of our our client(haptics) class. Another feature is the 
// IHaptics::IsConnected function which will return true if a haptics device is
// connected.
//
// this is a interface class because it is ment for game specific settings but
// a shared base of functions.
//
// you'll notice that we try to make a point that if we are calling haptic 
// effects related functions outside of the input class files we include this
// header and use the IHaptics singleton haptics. Generally its good to have a
// interface just for the effects/output forces and leave out the input related
// functions for things that just dont need it and avoid confusion.
// 
// of course if you did want to reach the client interface which holds all the
// data you certainly can by using the cliHaptics singleton 
// (externed in in_haptics.h)
//
//-----------------------------------------------------------------------------
class IHaptics
{
public:
	// Punches haptics in direction local to players view with a power set to strength.
	virtual void		TriggerPunchEffect( QAngle direction, float strength ) = 0;
	// Simulated getting hit, takes direction being shot from local to players view with the power of strength.
	virtual void		TriggerDamageEffect( QAngle direction, float strength ) = 0;
	// Send a small directional impulse to the haptics
	virtual void		TriggerVerticalShakeEffect( float strength ) = 0;
	// Bumps the haptics, type is for the water level ( 0 = solid, 1 = spashing, 2 = wading, 3 = swimming ). strength is power of bump.
	virtual void		TriggerFootStepEffect( byte type, float strength ) = 0;
	// Presses haptic forces down on the user by the mass entered. Running this command with a mass of zero will stop the effect.
	virtual void		TriggerMassEffect( float mass ) = 0;
	// When in acceleration sampling mode ( i.e. driving vehicles ) the velocity is sampled here.
	virtual void		ProcessVelocitySample( Vector velocity ) = 0;
	// Sets haptics to acceleration sampling mode, type entered should be from defines in ( vehicles.h on the server project ).
	virtual void		SetVehicleMode( int type ) = 0;
	// Resets all haptic effects.
	virtual void		ResetEffects() = 0;
	// Returns weather haptics is connected.
	virtual bool		IsConnected() = 0;
};

extern IHaptics *haptics;

#endif // CLIENT_HAPTICS_H