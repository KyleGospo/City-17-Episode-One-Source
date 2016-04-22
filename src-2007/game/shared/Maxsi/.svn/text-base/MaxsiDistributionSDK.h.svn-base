/****************************************************************************

	COPYRIGHT(C) MAXSI SOFTWARE, JONAS 'SORTIE' TERMANSEN 2008, 2009, 2010

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
		* Redistributions of source code must retain the above copyright
		  notice, this list of conditions and the following disclaimer.
		* Maxsi Distribution may not be used for evil. If you intend to use this
		  software for evil, you must purchase a license to use Maxsi Distribution
		  for evil from Maxsi Software.

	THIS SOFTWARE IS PROVIDED BY JONAS 'SORTIE' TERMANSEN ''AS IS'' AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL JONAS 'SORTIE' TERMANSEN BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	IN ADDITION, IN NO EVENT SHALL JONAS 'SORTIE' TERMANSEN BE LIABLE IF ANY USERS
	OF THIS SOFTWARE BECOME INFECTED WITH SWINE FLU OR ANY OTHER INFECTIOUS
	DECEASES (INCLUDING, BUT NOT LIMITED TO, BIRD FLU AND ZOMBIE OUTBREAKS), EVEN
	IF ADVISED OF THIS POSSIBILITY. (YES, THIS HAPPENED DURING INTERNAL BETA-TESTING)

	Maxsi Distribution SDK
	An interface to the Maxsi Distribution Platform.

	MaxsiDistributionSDK.h
	Declarations of the Maxsi Distribution Interfaces provided in the Maxsi
	Distribution SDK and various helper functions.

****************************************************************************/

#ifndef MaxsiDistributionInterfaces_H
#define MaxsiDistributionInterfaces_H

//=============================================================================
//	Declare helper functions for the Maxsi Engine String System MESTR.
//=============================================================================
#ifndef MESTR
#define MESTR char
#endif
#ifndef _MESTR
#define _MESTR(String) String
#endif

//=============================================================================
//	We need to include the Windows SDK from some files. Source gently avoids
//	using Windows.h by declaring a bunch of macros. These macros needs to be
//	unloaded before loading the Windows SDK.
//=============================================================================
#ifdef MAXSIDISTRIBUTIONSDK_CPP
#undef INVALID_HANDLE_VALUE
#undef GetCommandLine
#undef ReadConsoleInput
#undef RegCreateKey
#undef RegCreateKeyEx
#undef RegOpenKey
#undef RegOpenKeyEx
#undef RegQueryValue
#undef RegQueryValueEx
#undef RegSetValue
#undef RegSetValueEx
#undef RegCloseKey

//=============================================================================
//	Include the Windows SDK and other headers if required.
//=============================================================================
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <iostream> 
#include <fcntl.h>
#include <SYS\Stat.h>

#endif

//=============================================================================
//	Basic Win32/Win64 Support Macros used to load the correct build.
//=============================================================================
#ifdef _WIN64
#define PSIZESTR "64"
#else
#define PSIZESTR "32"
#endif

//=============================================================================
//	Declare stuff from the Windows SDK if it wasn't included.
//=============================================================================
#ifndef _WINDEF_
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;      /* HMODULEs can be used in place of HINSTANCEs */
#endif

//=============================================================================
//	Declare a SHA1 data structure
//=============================================================================
class SHA1_t
{
public:
	unsigned __int32 Hash[5];

public:
	SHA1_t()
	{
		// Default to the hash of an empty string.
		Hash[0]		=	0xDA39A3EE;
		Hash[1]		=	0x5E6B4B0D;
		Hash[2]		=	0x3255BFEF;
		Hash[3]		=	0x95601890;
		Hash[4]		=	0xAFD80709;
	}

	SHA1_t(	unsigned __int32 Hash1, 
			unsigned __int32 Hash2,
			unsigned __int32 Hash3,
			unsigned __int32 Hash4,
			unsigned __int32 Hash5)
	{
		Hash[0]		=	Hash1;
		Hash[1]		=	Hash2;
		Hash[2]		=	Hash3;
		Hash[3]		=	Hash4;
		Hash[4]		=	Hash5;
	}

public:
	inline bool operator==( const SHA1_t &val ) const
	{ 
		return (	Hash[0] == val.Hash[0] &&
					Hash[1] == val.Hash[1] && 
					Hash[2] == val.Hash[2] && 
					Hash[3] == val.Hash[3] && 
					Hash[4] == val.Hash[4] );
	} 

	inline bool operator!=( const SHA1_t &val ) const 
	{ 
		return (	Hash[0] != val.Hash[0] ||
					Hash[1] != val.Hash[1] || 
					Hash[2] != val.Hash[2] || 
					Hash[3] != val.Hash[3] || 
					Hash[4] != val.Hash[4] );
	} 
};


//=============================================================================
//	Declare macros and definitions used by the Maxsi Distribution SDK.
//=============================================================================
#define DECLARE_FUNCTION(return_type, name, parameters) typedef return_type (__cdecl* type_##name  ) parameters ; type_##name name;
#define IMPORT_FUNCTION(Executable,name)				name = reinterpret_cast<type_##name>(GetProcAddress(Executable, #name ));

#define	RESULT_DONT_RUN						0
#define	RESULT_RUN							1
#define	RESULT_UPDATE_UPDATING_SOFTWARE		2
#define	RESULT_UPDATE_UPDATED_SOFTWARE		3
#define	RESULT_UPDATE_UPDATING_FAILED		4
#define	RESULT_INVALID_PARAMETER_SIZE		5

#define	BINARY_MAXSIDISTRIBUTION_DLL			0
#define	BINARY_MAXSIDISTRIBUTIONUPDATE_DLL		1
#define	BINARY_MAXSIFEEDBACK_EXE				2
#define	BINARY_MAXSIINSTALLER_EXE				3

#define MDSDKEE_INTERFACE
#define MD_DLL_INTERFACE = 0

#if defined(HL2MP) || defined(MP_SDK)
#define MD_MP
#if defined(GAME_DLL)
#define MD_MP_SERVER
#else
#define MD_MP_CLIENT
#endif
#endif

//=============================================================================
//	Defines the current interface version. This calling system should not be
//	changed by other than Maxsi Software - while you technically are allowed to
//	do thism it's much better if we do this together - so that ALL versions of
//	Maxsi Distribution remains FULLY COMPATIBLE. If you create a version that
//	isn't combatible, contact Maxsi Software and we'll assign you a version number.
//	But please - make sure your changes are compatible with ALL other versions
//	of Maxsi Distribution.
//  Please see http://www.maxsi.dk/developer/index.php?title=Interfaces
//=============================================================================

// Maxsi Distribution SDK Callback Interface
#define	MaxsiDistributionClass						MaxsiDistributionClass001
#define MAXSIDISTRIBUTION_CLASS_VERSION				"MaxsiDistributionClass001"

// Maxsi Distribution Interface
#define	MaxsiDistributionInterface					MaxsiDistributionInterface001
#define MAXSIDISTRIBUTION_INTERFACE_VERSION			"MaxsiDistributionInterface001"

// Maxsi Distribution Module Interface
#define	MaxsiDistributionModule						MaxsiDistributionModule001
#define MAXSIDISTRIBUTIONMODULE_INTERFACE_VERSION	"MaxsiDistributionModuleInterface001"

// Maxsi Achievements
#define	MaxsiAchievementsInterface					MaxsiAchievementsInterface001
#define MAXSIACHIEVEMENTS_INTERFACE_VERSION			"MaxsiAchievementsInterface001"

// Maxsi Achievement Data Class
#define	MaxsiAchievement							MaxsiAchievement001
#define MAXSIACHIEVEMENT_INTERFACE_VERSION			"MaxsiAchievement001"

// Forward-declare all interfaces.
class MaxsiDistributionClass;
class MaxsiDistributionInterface;
class MaxsiDistributionModule;
class MaxsiAchievementsInterface;
class MaxsiAchievement;

//=============================================================================
// Maxsi Distribution Interface
//=============================================================================
class	MaxsiDistributionClass
{
public:
	MaxsiDistributionClass();
	~MaxsiDistributionClass();

	//=============================================================================
	//	void*	GetInterface(char* Version);
	//
	//	Returns the Version interface or NULL upon failure.
	//=============================================================================

	typedef void* (__cdecl* type_GetInterface  ) (MESTR* Version) ;
	type_GetInterface GetInterface;

	//=============================================================================
	//	bool	InitializeMaxsiDistribution();
	//
	//	Links to MaxsiDistribution.dll and sets g_MaxsiDistribution. If an error,
	//	occurs the function returns false and the game quits and an error message
	//	is presented. If true, the game can safely continue.
	//=============================================================================

	virtual	bool	InitializeMaxsiDistribution() MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	InitializeMaxsiDistributionLibrary();
	//
	//	Links to MaxsiDistribution.dll and sets g_MaxsiDistribution. If an error,
	//	occurs the function returns false and the game quits and an error message
	//	is presented. If true, the game can safely continue.
	//=============================================================================

	virtual	bool	InitializeMaxsiDistributionLibrary(int	Binary) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	BuildString(unsigned int NumParameters, ...);
	//
	//	Takes the [NumParameters] strings following NumParameters and combine them
	//	to a single string. Delete[] when done using the string.
	//=============================================================================

	virtual	char*	BuildString(unsigned int NumParameters, ...) MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	str_replace(char* input, char* find, char* replace);
	//
	//	Replaces any occurences of find with replace in input. Changes the input
	//	buffer - so find and replace must be of the same size.
	//=============================================================================

	virtual	bool	str_replace(char* input, char* find, char* replace) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	GetWorkingDir(void);
	//
	//	Gets the current working directory.
	//=============================================================================

	virtual	char*	GetWorkingDir(void) MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	ReportError(const char* Error, bool DontShowNotepad = false);
	//
	//	If not [DontShowNotepad] then quit the game and present the user with [Error]
	//	as text in a notepad file. If [DontShowNotepad] then write the error to the
	//	ingame console.
	//=============================================================================

	virtual	bool	ReportError(const char* Error, bool DontShowNotepad
#ifndef GAME_DLL // Hack to prevent error messages popping up twice!
		= false
#else
		= true
#endif
		) MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	IngameError(const char* Error);
	//
	//	Writes [Error] to the console.
	//=============================================================================

	virtual	bool	IngameError(const char* Error) MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	IngameInfo(const char* Error);
	//
	//	Writes [Info] to the console.
	//=============================================================================

	virtual	bool	IngameInfo(const char* Info) MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	IngameMsg(const char* Msg);
	//
	//	Writes Msg to the console.
	//=============================================================================

	virtual	bool	IngameMsg(const char* Msg) MDSDKEE_INTERFACE;

	//=============================================================================
	//	bool	IngameMsgDebug(const char* Msg);
	//
	//	Writes Msg to the console.
	//=============================================================================

	virtual	bool	IngameMsgDebug(const char* Msg) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	IToA(int In);
	//
	//	Converts an 32-bit signed integer to a string
	//=============================================================================

	virtual	char*	IToA(int In) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	UIToA(unsigned int In);
	//
	//	Converts an 32-bit unsigned integer to a string
	//=============================================================================

	virtual	char*	UIToA(unsigned int In) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	IToA64(__int64 In);
	//
	//	Converts an 64-bit signed integer to a string
	//=============================================================================

	virtual	char*	IToA64(__int64 In) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	UIToA(unsigned __int64 In);
	//
	//	Converts an 64-bit unsigned integer to a string
	//=============================================================================

	virtual	char*	UIToA64(unsigned __int64 In) MDSDKEE_INTERFACE;

	//=============================================================================
	//	SHA1_t	CalculateSHA1(char* Buffer, size_t BufferLen);
	//
	//	Calculates a 160-bit SHA1 hash
	//=============================================================================

	virtual	SHA1_t	CalculateSHA1(char* Buffer, size_t BufferLen) MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	GetSteamUsername();
	//
	//	Detects the Steam Username of the current user.
	//=============================================================================

	virtual	char*	GetSteamUsername() MDSDKEE_INTERFACE;

	//=============================================================================
	//	void	CreateInterfaces();
	//
	//	Creates the g_Maxsi* interfaces/modules used by Maxsi Distribution.
	//=============================================================================

	virtual	void	CreateInterfaces() MDSDKEE_INTERFACE;

	//=============================================================================
	//	char*	GetRegMaxsiRoot();
	//
	//	Returns the registy value of where Maxsi Distribution is installed, or
	//	NULL if not found. Any non-NULL value must be delete[]'d afterwards.
	//=============================================================================

	virtual	char*	GetRegMaxsiRoot() MDSDKEE_INTERFACE;

public:
	HMODULE		MaxsiDistributionLibrary;
	int			iSeekPath;
	int			iBuild;
	int			iBinFolder;
	int			iBinary;

};

//=============================================================================
// Maxsi Distribution Interface
//=============================================================================
class	MaxsiDistributionInterface
{
public:
	MaxsiDistributionInterface();
	~MaxsiDistributionInterface();

	//=============================================================================
	//	bool	RegisterModule(MaxsiDistributionModule* Module);
	//
	//	Module is added to the array of MD-extension modules accessed with the
	//	MaxsiDistributionModule interface.
	//=============================================================================
	
	virtual bool	RegisterModule(MaxsiDistributionModule* Module) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	Initialize(char** Variables, size_t NumVariables);
	//
	//	Sets a series of variables inside Maxsi Distribution.
	//=============================================================================

	virtual	bool	Initialize(MESTR** Variables, size_t NumVariables) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	CreateSession();
	//
	//	Terminates all data belonging to previous sessions and creates a new default
	//	session. A ses
	//=============================================================================

	virtual bool	CreateSession() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SaveSession(char** OutBuffer, size_t* OutBufferLength);
	//
	//	Exports a buffer with all the session-data this module needs to store
	//	permanently for this session specificly. The application should take care
	//	of storing this and passing it onto RestoreSession() when the session is
	//	restored. OutBuffer points to a	char* which will be changed to point to a
	//	buffer containing OutBufferLength bytes that should be saved. Call
	//	SavedSession() when done with the buffer.
	//=============================================================================

	virtual bool	SaveSession(char** OutBuffer, size_t* OutBufferLength) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SavedSession(char* OutBuffer, size_t OutBufferLength);
	//
	//	Deletes the buffer allocated for use by the application when SaveSession()
	//	is called. You must call SavedSession() when done with the buffer.
	//=============================================================================
	
	virtual bool	SavedSession(char* OutBuffer, size_t OutBufferLength) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	RestoreSession(char* OutBuffer, size_t OutBufferLength);
	//
	//	Restores a previous session.
	//=============================================================================

	virtual bool	RestoreSession(char* Buffer, size_t BufferLength) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	IsInitialized();
	//
	//	Returns true if ->Initialize() has already been run. Returns false if it
	//	haven't and should be run.
	//=============================================================================

	virtual	bool	IsInitialized() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	IsDisabled();
	//
	//	Returns true if Maxsi Distribution is disabled. (Meaning that Maxsi
	//	Distribution may not access the Internet - all other Maxsi Distribution
	//	features are running).
	//=============================================================================

	virtual	bool	IsDisabled() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	IsEnabled();
	//
	//	Returns true if Maxsi Distribution is enabled.  (Meaning that Maxsi
	//	Distribution may  access the Internet).
	//=============================================================================

	virtual	bool	IsEnabled() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	Disable();
	//
	//	Disables Maxsi Distribution (prevents Maxsi Distribution from accessing
	//	the Internet).
	//=============================================================================

	virtual	bool	Disable() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	Enable();
	//
	//	Enables Maxsi Distribution (allows Maxsi Distribution to access	the
	//	Internet).
	//=============================================================================

	virtual	bool	Enable() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	UpdateProduct();
	//
	//	Connects, if possible, to a content server and checks if the current version
	//	of this product is up to date. If it's not, the thread exits and this function
	//	returns false. If it's safe to launch the product the function returns true.
	//=============================================================================

	virtual	int		UpdateProduct() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	ReportCrashes();
	//
	//	Scans for crash minidumps (.mdmp's) related to this product and uploads
	//	any new crashes to a content server.
	//=============================================================================

	virtual	bool	ReportCrashes() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SDKMsg(char* Message);
	//
	//	Sends a message to the client app console.
	//=============================================================================

	virtual	bool	SDKMsg(char* Message) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SDKMsgDebug(char* Message);
	//
	//	Sends a message to the client app console if debug information is enabled
	//	in the SDK.
	//=============================================================================

	virtual	bool	SDKMsgDebug(char* Message) MD_DLL_INTERFACE;

public:
	bool	m_Initialized;
	bool	m_IsEnabled;

};

//=============================================================================
// Maxsi Distribution Module
//=============================================================================
class	MaxsiDistributionModule
{
public:
	MaxsiDistributionModule();
	~MaxsiDistributionModule();

	//=============================================================================
	//	char*	GetModuleName()
	//
	//	Returns a static string with the name of this particular MD Module.
	//=============================================================================

	virtual	MESTR*	GetModuleName() = 0;
	
	//=============================================================================
	//	bool	SetMaxsiDistributionInterface(MaxsiDistributionInterface* Interface)
	//
	//	Returns a static string with the name of this particular MD Module.
	//=============================================================================

	virtual	bool	SetMaxsiDistributionInterface(MaxsiDistributionInterface* Interface) = 0;

	//=============================================================================
	//	bool	CreateSession();
	//
	//	Terminates all data belonging to previous sessions and creates a new default
	//	session. A ses
	//=============================================================================

	virtual bool	CreateSession() = 0;

	//=============================================================================
	//	bool	SaveSession(char** OutBuffer, size_t* OutBufferLength);
	//
	//	Exports a buffer with all the session-data this module needs to store
	//	permanently for this session specificly. The application should take care
	//	of storing this and passing it onto RestoreSession() when the session is
	//	restored. OutBuffer points to a	char* which will be changed to point to a
	//	buffer containing OutBufferLength bytes that should be saved. Call
	//	SavedSession() when done with the buffer.
	//=============================================================================

	virtual bool	SaveSession(char** OutBuffer, size_t* OutBufferLength) = 0;

	//=============================================================================
	//	bool	SavedSession(char* OutBuffer, size_t OutBufferLength);
	//
	//	Deletes the buffer allocated for use by the application when SaveSession()
	//	is called. You must call SavedSession() when done with the buffer.
	//=============================================================================
	
	virtual bool	SavedSession(char* OutBuffer, size_t OutBufferLength) = 0;

	//=============================================================================
	//	bool	RestoreSession(char* OutBuffer, size_t OutBufferLength);
	//
	//	Restores a previous session.
	//=============================================================================

	virtual bool	RestoreSession(char* Buffer, size_t BufferLength) = 0;

};

//=============================================================================
// Maxsi Achievements declarations/definitions
//=============================================================================
#define MD_ACHIEVEMENT_LOCKED	0x00000000
#define MD_ACHIEVEMENT_UNLOCKED	0xFFFFFFFF

//=============================================================================
// Maxsi Achievement Data Class Interface
//=============================================================================
class	MaxsiAchievement
{
public:
	MaxsiAchievement();
	~MaxsiAchievement();

	//=============================================================================
	//	char* GetAchievementName();
	//
	//	Returns a string with the name of this achievement.
	//=============================================================================

	virtual	MESTR*			GetAchievementName() MD_DLL_INTERFACE;

	//=============================================================================
	//	char* GetAchievementDescription();
	//
	//	Returns a string with the description of this achievement.
	//=============================================================================

	virtual	MESTR*			GetAchievementDescription() MD_DLL_INTERFACE;

	//=============================================================================
	//	unsigned int SetAchievementProgress(unsigned int NewProgress);
	//
	//	Sets the progress of this achievement.
	//=============================================================================

	virtual	unsigned int	SetAchievementProgress(unsigned int NewProgress) MD_DLL_INTERFACE;

	//=============================================================================
	//	unsigned int GetAchievementProgress();
	//
	//	Returns the progress for this achievement.
	//=============================================================================

	virtual	unsigned int	GetAchievementProgress() MD_DLL_INTERFACE;

	//=============================================================================
	//	unsigned int GetAchievementProgressMax();
	//
	//	Returns the max progress for this achievement.
	//=============================================================================

	virtual	unsigned int	GetAchievementProgressMax() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool IsSessionAchievement();
	//
	//	Returns whether this achievement is a session achievement. 
	//=============================================================================

	virtual	bool			IsSessionAchievement() MD_DLL_INTERFACE;

	//=============================================================================
	//	int LogoResourceId();
	//
	//	Returns an Id for the logo for this Achievement that can later be retrieved
	//	from the MaxsiAchievementsInterface class.
	//=============================================================================

	virtual	int				LogoResourceId() MD_DLL_INTERFACE;

public:
	MESTR*			AchievementName;
	MESTR*			AchievementDesc;
	unsigned int	Progress;
	unsigned int	ProgressMax;
	bool			SessionAchievement;
	int				ResourceId;

};

//=============================================================================
// Maxsi Achievements Interface
//=============================================================================
class	MaxsiAchievementsInterface : public MaxsiDistributionModule
{
public:
	MaxsiAchievementsInterface();
	~MaxsiAchievementsInterface();

	//=============================================================================
	//	bool	GetAchievements(MaxsiAchievement*** ListOut, size_t* NumOut);
	//
	//	If returns true, the MaxsiAchievement** member pointed to by ListOut is now
	//	filled with a pointer pointing to an array of MaxsiAchievement* members
	//	each pointing to an achievement. The size_t pointed to by Numout is filled
	//	with an integer descibing how many entires are in the array pointed to by
	//	the member pointed to by ListOut. When done, call DeleteAchievementList()
	//	with the member pointed to by ListOut and the member pointed to by NumOut.
	//=============================================================================

	virtual	bool	GetAchievements(MaxsiAchievement*** ListOut, size_t* NumOut) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	DeleteAchievementList(MaxsiAchievement** ListOut, size_t NumOut);
	//
	//	Deletes this achievement array with NumOut members.
	//=============================================================================

	virtual	bool	DeleteAchievementList(MaxsiAchievement** ListOut, size_t NumOut) MD_DLL_INTERFACE;

	//=============================================================================
	//	MaxsiAchievement*	GetAchievement(MESTR* Achievement);
	//
	//	Returns the achievement Achievement if such one is found, otherwise NULL.
	//=============================================================================

	virtual	MaxsiAchievement*	GetAchievement(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	UnlockAchievement(MESTR* Achievement);
	//
	//	Unlocks the achievement called Achievement for the current user and
	//	contacts the master server if it was previously locked.
	//=============================================================================

	virtual	bool	UnlockAchievement(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	LockAchievement(MESTR* Achievement);
	//
	//	Unlocks the achievement called Achievement for the current user and
	//	contacts the master server if it was previously unlocked.
	//=============================================================================

	virtual	bool	LockAchievement(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SetAchievementProgress(MESTR* Achievement) = 0;
	//
	//	Sets the progress for the achievement called Achievement and contacts
	//	the master server if it has changed.
	//=============================================================================

	virtual	bool	SetAchievementProgress(MESTR* Achievement, unsigned int Progress) MD_DLL_INTERFACE;
	
	//=============================================================================
	//	unsigned int	GetAchievementProgress(MESTR* Achievement);
	//
	//	Returns the progress for this achievement.
	//=============================================================================

	virtual	unsigned int	GetAchievementProgress(MESTR* Achievement) MD_DLL_INTERFACE;
	
	//=============================================================================
	//	unsigned int	GetAchievementProgressMax(MESTR* Achievement);
	//
	//	Returns the max progress for this achievement.
	//=============================================================================

	virtual	unsigned int	GetAchievementProgressMax(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	IncrementAchievementProgress(MESTR* Achievement, [unsigned int Progress = 1]);
	//
	//	Increments the progress for the achievement Achievement with the optional
	//	amount Progress. Unlocks the Achievement if adding progress hits the
	//	achievement progress max. The Master Server is contacted if Progress is
	//	not 0 and Achievement is not a session achievement. The Master Server is
	//	not contacted, if Achievement is a session achievement, unless Progress
	//	hits the max.
	//=============================================================================

	virtual bool	IncrementAchievementProgress(MESTR* Achievement, unsigned int Progress = 1) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	DecrementAchievementProgress(MESTR* Achievement, [unsigned int Progress = 1]);
	//
	//	Decrements the progress for the achievement Achievement with the optional
	//	amount Progress. Unlocks the Achievement if adding progress hits the
	//	achievement progress max. Master Server is contacted if Progress is not 0.
	//=============================================================================

	virtual bool	DecrementAchievementProgress(MESTR* Achievement, unsigned int Progress = 1) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool IsSessionAchievement(MESTR* Achievement);
	//
	//	Returns whether this achievement is a session achievement. 
	//=============================================================================

	virtual	bool	IsSessionAchievement(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SessionFailedUnlock(MESTR* Achievement);
	//
	//	This per-session attempt to unlock Achievement failed.
	//=============================================================================

	virtual	bool	SessionUnlockFailed(MESTR* Achievement) MD_DLL_INTERFACE;
	
	//=============================================================================
	//	bool	SessionUnlockAttempt(MESTR* Achievement);
	//
	//	This per-session attempt to unlock Achievement has not failed yet.
	//=============================================================================

	virtual	bool	SessionUnlockAttempt(MESTR* Achievement) MD_DLL_INTERFACE;
	
	//=============================================================================
	//	bool	SessionFailedUnlock(MESTR* Achievement);
	//
	//	All session achievements - that has not failed yet - are hereby completed.
	//	The Master Server is instantly contacted and notified by the success, if
	//	any achievements were unlocked. The Master Server is not contacted if no
	//	session achievements were unlocked.
	//=============================================================================

	virtual	bool	SessionUnlockSucceded(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	int LogoResourceId(MESTR* Achievement);
	//
	//	Returns an Id for the logo for this Achievement that can later be retrieved
	//	from the MaxsiAchievementsInterface class.
	//=============================================================================

	virtual	int		LogoResourceId(MESTR* Achievement) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool GetLogoResource(char** BufferOut, size_t* BufferLengthOut, int ResourcId, int AchievementStatus );
	//
	//	Returns true if BufferOut gives a data buffer containing BufferLengthOut
	//	bytes giving the resource pointed by ResourceId and AchievementStatus
	//	which takes the two values: MD_ACHIEVEMENT_UNLOCKED and
	//	MD_ACHIEVEMENT_LOCKED.
	//=============================================================================

	virtual	bool	GetLogoResource(char** BufferOut, size_t* BufferLengthOut, int ResourcId, int AchievementStatus ) MD_DLL_INTERFACE;

	//=============================================================================
	//	char*	GetModuleName()
	//
	//	Returns a static string with the name of this particular MD Module.
	//=============================================================================

	virtual	MESTR*	GetModuleName() MD_DLL_INTERFACE;
	
	//=============================================================================
	//	bool	SetMaxsiDistributionInterface(MaxsiDistributionInterface* Interface)
	//
	//	Sets the the Maxsi Distribution main interface that modules should
	//	communicate through.
	//=============================================================================

	virtual	bool	SetMaxsiDistributionInterface(MaxsiDistributionInterface* Interface) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	CreateSession();
	//
	//	Terminates all data belonging to previous sessions and creates a new default
	//	session. A ses
	//=============================================================================

	virtual bool	CreateSession() MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SaveSession(char** OutBuffer, size_t* OutBufferLength);
	//
	//	Exports a buffer with all the session-data this module needs to store
	//	permanently for this session specificly. The application should take care
	//	of storing this and passing it onto RestoreSession() when the session is
	//	restored. OutBuffer points to a	char* which will be changed to point to a
	//	buffer containing OutBufferLength bytes that should be saved. Call
	//	SavedSession() when done with the buffer.
	//=============================================================================

	virtual bool	SaveSession(char** OutBuffer, size_t* OutBufferLength) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	SavedSession(char* OutBuffer, size_t OutBufferLength);
	//
	//	Deletes the buffer allocated for use by the application when SaveSession()
	//	is called. You must call SavedSession() when done with the buffer.
	//=============================================================================
	
	virtual bool	SavedSession(char* OutBuffer, size_t OutBufferLength) MD_DLL_INTERFACE;

	//=============================================================================
	//	bool	RestoreSession(char* OutBuffer, size_t OutBufferLength);
	//
	//	Restores a previous session.
	//=============================================================================

	virtual bool	RestoreSession(char* Buffer, size_t BufferLength) MD_DLL_INTERFACE;

public:
	MaxsiDistributionInterface* MDInterface;
	MaxsiAchievement**			Achievements;
	size_t						NumAchievements;

};

//=============================================================================
//	Declare external pointers to all used interfaces.
//=============================================================================
extern	MaxsiDistributionInterface*		g_MaxsiDistribution;
extern	MaxsiAchievementsInterface*		g_MaxsiAchievements;
extern	MaxsiDistributionClass			d_MaxsiDistribution;

//=============================================================================
//	Declare a Save/Restore Block Handler for Source Save Game Files.
//=============================================================================
#ifdef GAME_DLL

	#include "saverestore.h"

	ISaveRestoreBlockHandler *GetMaxsiDistributionSaveRestoreBlockHandler();

#endif

#endif