//===== Copyright © 2009, Gear Software, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//==================================================================//
#ifndef C17_SUNSHAFTS_H
#define C17_SUNSHAFTS_H
#ifdef _WIN32
#pragma once
#endif
 
#include "ScreenSpaceEffects.h"
 
class CSunShaftEffect : public IScreenSpaceEffect
{
public:
	CSunShaftEffect( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) {};
	virtual bool IsEnabled( void ) { return true; }
 
	virtual void RegisterEnt( EHANDLE hEnt );
	virtual void DeregisterEnt( EHANDLE hEnt );
 
	virtual void Render( int x, int y, int w, int h );
 
protected:
	int FindShaftEnt( EHANDLE hEnt );
	void RenderToStencil( int idx, IMatRenderContext *pRenderContext );
	void RenderLightToTexture( int idx, IMatRenderContext *pRenderContext );
 
private:
	struct sShaftEnt
	{
		EHANDLE	m_hEnt;
	};
 
	CUtlVector<sShaftEnt*>	m_vShaftEnts;
 
	CTextureReference	m_SunQuarterRes;
	CTextureReference	m_SunFullRes;
 
	CMaterialReference	m_SunPassMaterial;
	CMaterialReference	m_SunPassFinalMaterial;
	CMaterialReference	m_BlackMaterial;
};
 
#endif