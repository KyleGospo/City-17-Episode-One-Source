#include "cbase.h"
#include "baseentity.h"
#include "entityoutput.h"
#include "igameevents.h"

// FMOD
#include "fmod/fmod_manager.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CAmbientGenericFMOD : public CPointEntity
{
public:
	DECLARE_CLASS( CAmbientGenericFMOD, CPointEntity );
	DECLARE_DATADESC();

	void Spawn( void );

	virtual void UpdateOnRemove( void );

	void PlaySound( void );
	void StopSound( bool bFade );
	
	// Input handlers
	void InputPlaySound( inputdata_t &inputdata );
	void InputStopSound( inputdata_t &inputdata );
	void InputSetSoundPath( inputdata_t &inputdata );
	void InputFadeSoundOut( inputdata_t &inputdata );

private:
	string_t	m_iszSound;		// Path/filename of WAV file to play.
};

LINK_ENTITY_TO_CLASS( ambient_generic_fmod, CAmbientGenericFMOD );

BEGIN_DATADESC( CAmbientGenericFMOD )
	DEFINE_KEYFIELD( m_iszSound, FIELD_SOUNDNAME, "message" ),

	// Inputs
	DEFINE_INPUTFUNC(FIELD_VOID, "PlaySound", InputPlaySound ),
	DEFINE_INPUTFUNC(FIELD_VOID, "StopSound", InputStopSound ),
	DEFINE_INPUTFUNC(FIELD_VOID, "FadeSoundOut", InputFadeSoundOut ),
	DEFINE_INPUTFUNC(FIELD_STRING, "SetSoundPath", InputSetSoundPath ),
END_DATADESC()

//-----------------------------------------------------------------------------
// Spawn
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::Spawn( void )
{
	char *szSoundFile = (char *)STRING( m_iszSound );
	if ( !m_iszSound || strlen( szSoundFile ) < 1 )
	{
		Warning( "Empty %s (%s) at %.2f, %.2f, %.2f\n", GetClassname(), GetDebugName(), GetAbsOrigin().x, GetAbsOrigin().y, GetAbsOrigin().z );
		UTIL_Remove(this);
		return;
	}

	if ( m_iszSound == NULL_STRING )
	{
		UTIL_Remove(this);
		return;
	}

    SetSolid( SOLID_NONE );
    SetMoveType( MOVETYPE_NONE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::UpdateOnRemove( void )
{
	//We've been removed. Stop all FMOD handled audio.
	StopSound( false );

	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
// Purpose: Handles playing sounds via FMOD.
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::PlaySound( void )
{
	if ( m_iszSound != NULL_STRING )
	{
		FMODManager()->TransitionAmbientSounds( STRING( m_iszSound ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Handles stopping sounds via FMOD.
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::StopSound( bool bFade )
{
	FMODManager()->StopAmbientSound( bFade );
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that begins playing the sound.
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::InputPlaySound( inputdata_t &inputdata )
{
	PlaySound();
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that stops playing the sound.
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::InputStopSound( inputdata_t &inputdata )
{
	StopSound( false );
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that stops playing the sound.
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::InputFadeSoundOut( inputdata_t &inputdata )
{
	StopSound( true );
}

//-----------------------------------------------------------------------------
// Purpose: Input handler that stops playing the sound.
//-----------------------------------------------------------------------------
void CAmbientGenericFMOD::InputSetSoundPath( inputdata_t &inputdata )
{
	m_iszSound = MAKE_STRING( inputdata.value.String() );
}