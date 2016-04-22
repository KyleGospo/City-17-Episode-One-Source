// Haptics --------------------------------------------------------------------
// Purpose: Usermessage registers. This is outside of the standard register in
// usermessages.cpp to allow for easier implementation and less need to edit
// perhaps mod/game specific usermessages already set up. 
//
// This file should only be included to usermesssages.cpp and the function
// RegisterHapticUserMessages() should be called right below the standard
// call to RegisterUserMessages() inside CUserMessages::CUserMessages()
//
// See haptic_messages.h on the client side project for more information on
// how to use these.
//-----------------------------------------------------------------------------
static void RegisterHapticUserMessages()
{
	// sends a haptics punch to the client.
	usermessages->Register( "HapPunch",
#ifdef HAP_NETWORK_OPTIMIZE
							-1 // 1 float (strength) 1 angles (rarely goes above 7bytes)
#else
							16 // 1 float (strength) 3 float (angles)
#endif // HAP_NETWORK_OPTIMIZE
							);

	// just contains volume. the client checks what surface they are on.
	usermessages->Register( "HapStep",  
#ifdef HAP_NETWORK_OPTIMIZE
							-1 // 1 byte (strength) 1 byte (steptype) ( max of 2 bytes)
#else
							5 // 1 float (strength) 1 byte (steptype)
#endif // HAP_NETWORK_OPTIMIZE
							);

	// informs the client the direction(pitch and yaw) and amount of damage delt on.
	usermessages->Register( "HapDmg", 
#ifdef HAP_NETWORK_OPTIMIZE
							-1  // 1 byte (damage), 1 angles (pitch and yaw ROLL IS ALWAYS ZERO) generaly 6-7 bytes
#else
							12 // 1 float (damage amount) 2 floats (angles pitch and yaw)
#endif // HAP_NETWORK_OPTIMIZE
							);

	// informs client we've picked up a object and gives its mass.
	usermessages->Register( "HapMassOn", 4 );

	// tells the client we have dropped a object.
	usermessages->Register( "HapMassOff", 0 );

	// tells the client we are going into velocity mode and what vehicle type it is.
	usermessages->Register( "HapVehicleEnter", 1 );

	// updates the clients knowledge of velocity it's vehicle is going.
	usermessages->Register( "HapVelocityUpdate",
#ifdef HAP_NETWORK_OPTIMIZE
		-1
#else
		12
#endif // HAP_NETWORK_OPTIMIZE
		);

}