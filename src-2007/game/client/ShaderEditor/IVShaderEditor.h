#ifndef IV_SHADEREDITOR
#define IV_SHADEREDITOR

#ifdef _WIN32
#pragma once
#endif // _WIN32

#define pFnClCallback( x ) void(* x )( float *pfl4 )
#define pFnClCallback_Declare( x ) void x( float *pfl4 )

#ifndef PROCSHADER_DLL

#ifdef SHADER_EDITOR_DLL
#include "../public/tier1/interface.h"
#else
#include "interface.h"
#include "ShaderEditor/ShaderEditorSystem.h"
#endif // NOT SHADER_EDITOR_DLL

enum SEDIT_SKYMASK_MODE
{
	SKYMASK_OFF = 0,
	SKYMASK_QUARTER, // render at 1/4 fb size where possible
	SKYMASK_FULL, // render at full fb size
};

class IVShaderEditor : public IBaseInterface
{
public:
	virtual bool Init( CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals,
					void *pSEditMRender,
					bool bCreateEditor, bool bEnablePrimaryDebug, int iSkymaskMode ) = 0;
	virtual void Shutdown() = 0;

	virtual void OnFrame( float frametime ) = 0;
	virtual void OnPreRender( void *viewsetup ) = 0;
	virtual void OnUpdateSkymask( bool bCombineMode ) = 0;
	virtual void OnPostRender( bool bUpdateFB ) = 0;

	virtual void RegisterClientCallback( const char *name, pFnClCallback(callback), int numComponents ) = 0;
	virtual void LockClientCallbacks() = 0;
};

#define SHADEREDIT_INTERFACE_VERSION "ShaderEditor002"

#ifdef SHADER_EDITOR_DLL
class ShaderEditorInterface;
extern ShaderEditorInterface *shaderEdit;
#else
extern IVShaderEditor *shaderEdit;
#endif // NOT SHADER_EDITOR_DLL

#endif // NOT PROCSHADER_DLL

#endif // NOT IV_SHADEREDITOR