#ifndef SHEDITSYSTEM_H
#define SHEDITSYSTEM_H

#include "cbase.h"

class ShaderEditorHandler : public CAutoGameSystemPerFrame
{
public:
	ShaderEditorHandler( char const *name );
	~ShaderEditorHandler();

	virtual bool Init();
	virtual void Shutdown();

	virtual void Update( float frametime );
	virtual void PreRender();
	virtual void PostRender();

	void CustomPostRender();
	void UpdateSkymask( bool bCombineMode = false );
	const bool IsReady();

private:
	bool m_bReady;

	void RegisterCallbacks();
	void PrepareCallbackData();
};

extern ShaderEditorHandler *g_ShaderEditorSystem;


#endif