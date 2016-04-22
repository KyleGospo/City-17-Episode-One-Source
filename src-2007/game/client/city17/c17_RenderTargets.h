//========= Copyright © 1996-2008, The New Era team, All rights reserved. ============//
//
// Purpose: C17's Various Render Targets
//
// $NoKeywords: $
//=============================================================================//

#ifndef C17RENDERTARGETS_H_
#define C17RENDERTARGETS_H_
#ifdef _WIN32
#pragma once
#endif
 
#include "baseclientrendertargets.h" // Base class, with interfaces called by engine and inherited members to init common render   targets
 
// externs
class IMaterialSystem;
class IMaterialSystemHardwareConfig;
 
class CC17RenderTargets : public CBaseClientRenderTargets
{ 
	// no networked vars 
	DECLARE_CLASS_GAMEROOT( CC17RenderTargets, CBaseClientRenderTargets );
public:
	virtual void InitClientRenderTargets( IMaterialSystem* pMaterialSystem, IMaterialSystemHardwareConfig* pHardwareConfig );
	virtual void ShutdownClientRenderTargets();
 
	ITexture* CreateScopeTexture( IMaterialSystem* pMaterialSystem, int iSize = 1024 );
	//ITexture* CreateSunTexture( IMaterialSystem* pMaterialSystem, int iSize = 128 );

private:
	CTextureReference		m_ScopeTexture; 
	//CTextureReference		m_SunShaftBlackTexture;
};
 
extern CC17RenderTargets* C17RenderTargets;
 
#endif //C17RENDERTARGETS_H_