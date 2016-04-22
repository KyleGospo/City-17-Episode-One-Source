#include "cbase.h"
#include "fmod_manager.h"

#include "saverestore.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace FMOD;

System				*pSystem;
Sound				*pSound;
SoundGroup			*pSoundGroup;
Channel				*pChannel;
ChannelGroup		*pChannelGroup;
unsigned int		version;
FMOD_SPEAKERMODE	speakermode;
FMOD_CAPS			caps;
char				drivername[256];
FMOD_RESULT			result;

CFMODManager gFMODMng;
CFMODManager* FMODManager()
{
	return &gFMODMng;
}

CFMODManager::CFMODManager()
{
	m_fFadeDelay = 0.0;
	newSoundFileToTransitionTo = "NULL";
	currentSound = "NULL";
	m_bShouldTransition = false;
	m_bFadeIn = false;
	m_bFadeOut = false;
}

CFMODManager::~CFMODManager()
{
	m_fFadeDelay = 0.0;
	newSoundFileToTransitionTo = "NULL";
	currentSound = "NULL";
	m_bShouldTransition = false;
	m_bFadeIn = false;
	m_bFadeOut = false;
}

// Starts FMOD
void CFMODManager::InitFMOD( void )
{
	result = System_Create( &pSystem );
    CheckResult( "System Creation" );
   
	result = pSystem->getVersion( &version );
    CheckResult( "FMOD Version Number Retrieval" );

	if (version < FMOD_VERSION)
	{
		Warning("WARNING: You are using a version of fmodex.dll that City 17: Episode One was not designed to use! Please use the copy of fmodex.dll City 17 shipped with (FMOD Ex Version: 0.4.33.6)\n");
		return;
	}
   
	result = pSystem->getDriverCaps( 0, &caps, 0, 0, &speakermode );
    CheckResult( "Retrieve driver capabilities" );

	result = pSystem->setSpeakerMode( speakermode );
	CheckResult( "Set speaker mode" );

	if (caps & FMOD_CAPS_HARDWARE_EMULATED)
	{
		result = pSystem->setDSPBufferSize(1024, 10);
		CheckResult( "Set DSP Buffer for emulated sound mode" );
		Warning("WARNING: Audio Hardware Acceleration is not enabled! Please enable Audio Hardware Acceleration in Windows, or you will experience sound problems.\n");
	}

	result = pSystem->getDriverInfo( 0, drivername, 256, NULL );
	CheckResult( "Get user driver info" );

	if (strstr(drivername, "SigmaTel"))
	{
		result = pSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
		CheckResult( "Set Software Format for SigmaTel audio hardware" );
	}

	result = pSystem->init( 100, FMOD_INIT_NORMAL, 0 );
	CheckResult( "FMOD Initialization" );
	if ( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
	{
		result = pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		CheckResult( "Set Speaker Mode back to Stereo" );
               
		result = pSystem->init(100, FMOD_INIT_NORMAL, 0);
		CheckResult( "FMOD Stereo Initialization" );
    }

	g_pGameSaveRestoreBlockSet->AddBlockHandler( GetFMODSaveRestoreBlockHandler() );
}

// Stops FMOD
void CFMODManager::ExitFMOD( void )
{
	g_pGameSaveRestoreBlockSet->RemoveBlockHandler( GetFMODSaveRestoreBlockHandler() );
	result = pSystem->release();

	CheckResult( "System Termination" );
}

// Error checks FMOD
void CFMODManager::CheckResult( const char* failedSystem )
{
	if (result != FMOD_OK)
	{
		Warning("FMOD ERROR: Failed on step: '%s'!\n", failedSystem);
	}
	else
	{
		DevMsg("FMOD: Step: '%s' succeeded!\n", failedSystem);
	}
}

// Returns the full path of a specified sound file in the /sounds folder
const char* CFMODManager::GetFullPathToSound( const char* pathToFileFromModFolder )
{
	char* gamedir = new char[512];
	engine->GetGameDir( gamedir, 512 );

	char* resultpath = new char[512];
	Q_snprintf( resultpath, 512, "%s/sound/%s", gamedir, pathToFileFromModFolder );

	// convert backwards slashes to forward slashes
	for ( int i = 0; i < 512; i++ )
	{
		if( resultpath[i] == '\\' ) 
			resultpath[i] = '/';
	}

	return resultpath;
}

// Returns the name of the current ambient sound being played
// If there is an error getting the name of the ambient sound or if no ambient sound is currently being played, returns "NULL"
const char* CFMODManager::GetCurrentSoundName( void )
{
	return currentSound;
}

// Handles all fade-related sound stuffs
// Called every frame when the client is in-game
void CFMODManager::FadeThink( void )
{
	ConVar *musicvolume = cvar->FindVar( "snd_musicvolume" );
	float tempvol;
	pChannel->getVolume( &tempvol );
	if ( m_bFadeOut )
	{
		if ( gpGlobals->curtime >= m_fFadeDelay )
		{
			if ( tempvol > 0.0 )
			{
				pChannel->setVolume( tempvol - 0.05 );
				m_fFadeDelay = gpGlobals->curtime + 0.1;
			}
			else
			{
				pChannel->setVolume( 0.0 );
				m_bFadeOut = false;
				m_fFadeDelay = 0.0;
			}
		}
	}
	else if ( m_bShouldTransition )
	{
		result = pSystem->createStream( GetFullPathToSound( newSoundFileToTransitionTo ), FMOD_DEFAULT, 0, &pSound );

		if (result != FMOD_OK)
		{
			Warning("FMOD: Failed to create stream of sound '%s' ! (ERROR NUMBER: %i)\n", newSoundFileToTransitionTo, result);
			newSoundFileToTransitionTo = "NULL";
			m_bShouldTransition = false;
			return;
		}

		result = pSystem->playSound( FMOD_CHANNEL_REUSE, pSound, false, &pChannel);

		if (result != FMOD_OK)
		{
			Warning("FMOD: Failed to play sound '%s' ! (ERROR NUMBER: %i)\n", newSoundFileToTransitionTo, result);
			newSoundFileToTransitionTo = "NULL";
			m_bShouldTransition = false;
			return;
		}

		currentSound = newSoundFileToTransitionTo;
		newSoundFileToTransitionTo = "NULL";
		m_bShouldTransition = false;
	}
	else if ( m_bFadeIn )
	{
		if ( gpGlobals->curtime >= m_fFadeDelay )
		{
			if ( tempvol < ( 1.0 * musicvolume->GetFloat() ) )
			{
				pChannel->setVolume( tempvol + 0.05 );
				m_fFadeDelay = gpGlobals->curtime + 0.1;
			}
			else
			{
				pChannel->setVolume( 1.0 * musicvolume->GetFloat() );
				m_bFadeIn = false;
				m_fFadeDelay = 0.0;
			}
		}
	}

	if( engine->IsPaused() != IsPaused() )
	{
		PauseAmbientSound( engine->IsPaused() );
	}
}

// Compares specified ambient sound with the current ambient sound being played
// Returns true if they match, false if they do not or if no sound is being played
bool CFMODManager::IsSoundPlaying( const char* pathToFileFromSoundsFolder )
{
	const char* currentSoundPlaying = GetCurrentSoundName();
	return strcmp(currentSoundPlaying, pathToFileFromSoundsFolder) == 0;
}

// Abruptly starts playing a specified ambient sound
// In most cases, we'll want to use TransitionAmbientSounds instead
void CFMODManager::PlayAmbientSound( const char* pathToFileFromSoundsFolder, bool fadeIn )
{
	result = pSystem->createStream( GetFullPathToSound( pathToFileFromSoundsFolder ), FMOD_DEFAULT, 0, &pSound );

	if (result != FMOD_OK)
	{
		Warning("FMOD: Failed to create stream of sound '%s' ! (ERROR NUMBER: %i)\n", pathToFileFromSoundsFolder, result);
		return;
	}

	result = pSystem->playSound( FMOD_CHANNEL_REUSE, pSound, false, &pChannel);

	if (result != FMOD_OK)
	{
		Warning("FMOD: Failed to play sound '%s' ! (ERROR NUMBER: %i)\n", pathToFileFromSoundsFolder, result);
		return;
	}

	if ( fadeIn )
	{
		pChannel->setVolume( 0.0 );
		m_bFadeIn = true;
	}

	currentSound = pathToFileFromSoundsFolder;
}

// Abruptly stops playing all ambient sounds
void CFMODManager::StopAmbientSound( bool fadeOut )
{
	ConVar *musicvolume = cvar->FindVar( "snd_musicvolume" );
	if ( fadeOut )
	{
		if ( !m_bFadeOut && !m_bFadeIn )
		{
			pChannel->setVolume( 1.0 * musicvolume->GetFloat() );
			m_bFadeOut = true;
		}
	}
	else
	{
		pChannel->setVolume( 0.0 );
		m_bFadeOut = false;
		m_bFadeIn = false;
	}

	currentSound = "NULL";
}

// Transitions between two ambient sounds if necessary
// If a sound isn't already playing when this is called, don't worry about it
void CFMODManager::TransitionAmbientSounds( const char* pathToFileFromSoundsFolder )
{
	if( IsSoundPlaying( pathToFileFromSoundsFolder ) )
		return;

	float volume;
	pChannel->getVolume( &volume );

	if ( volume == 0.0 )
	{
		PlayAmbientSound( pathToFileFromSoundsFolder, true );
		return;
	}

	ConVar *musicvolume = cvar->FindVar( "snd_musicvolume" );

	pChannel->setVolume( 1.0 * musicvolume->GetFloat() );
	newSoundFileToTransitionTo = pathToFileFromSoundsFolder;

	m_bFadeOut = true;
	m_bShouldTransition = true;
	m_bFadeIn = true;
}

void CFMODManager::PauseAmbientSound( bool bPause )
{
	pChannel->setPaused( bPause );
}

bool CFMODManager::IsPaused( void )
{
	bool bIsPaused;
	pChannel->getPaused( &bIsPaused );

	return bIsPaused;
}

static short FMOD_SAVE_RESTORE_VERSION = 1;
 
//-----------------------------------------------------------------------------
 
class CFMODSaveRestoreBlockHandler : public CDefSaveRestoreBlockHandler
{
public:
	const char *GetBlockName()
	{
		return "FMOD";
	}
 
	//---------------------------------
 
	void Save( ISave *pSave )
	{
		pSave->StartBlock( GetBlockName() );
 
		char*	Data		=	"\n == FMOD Data Save Test 1.00 ==\n\n";
		size_t	DataLength	=	strlen(Data)+1;
 
		pSave->WriteData( (const char*)&DataLength, sizeof(DataLength) );
		pSave->WriteData( (const char*)Data, (int)DataLength );
 
		pSave->EndBlock();
	}
 
	//---------------------------------
 
	void WriteSaveHeaders( ISave *pSave )
	{
		pSave->WriteShort( &FMOD_SAVE_RESTORE_VERSION );
	}
 
	//---------------------------------
 
	void ReadRestoreHeaders( IRestore *pRestore )
	{
		// No reason why any future version shouldn't try to retain backward compatability. The default here is to not do so.
		short version;
		pRestore->ReadShort( &version );
		// only load if version matches and if we are loading a game, not a transition
		m_fDoLoad = ( ( version == FMOD_SAVE_RESTORE_VERSION ) && 
			( ( MapLoad_LoadGame == gpGlobals->eLoadType ) || ( MapLoad_NewGame == gpGlobals->eLoadType )  ) 
		);
	}
 
	//---------------------------------
 
	void Restore( IRestore *pRestore, bool createPlayers )
	{
		if ( m_fDoLoad )
		{
			pRestore->StartBlock();
 
			size_t	DataLength;
 
			pRestore->ReadData((char*)&DataLength,sizeof(size_t),0);
 
			char*	Data	=	new char[DataLength];
 
			pRestore->ReadData((char*)Data,(int)DataLength,DataLength);
 
			Color ConsoleColor(100,255,100,255);
			ConColorMsg(ConsoleColor,Data);
 
			delete[] Data;
 
			pRestore->EndBlock();
		}
	}
 
private:
	bool m_fDoLoad;
};
 
//-----------------------------------------------------------------------------
 
CFMODSaveRestoreBlockHandler g_FMODSaveRestoreBlockHandler;
 
//-------------------------------------
 
ISaveRestoreBlockHandler *GetFMODSaveRestoreBlockHandler()
{
	return &g_FMODSaveRestoreBlockHandler;
}