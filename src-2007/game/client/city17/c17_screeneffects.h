#ifndef C17_SCREENSPACEEFFECTS_H
#define C17_SCREENSPACEEFFECTS_H
#ifdef _WIN32
#pragma once
#endif
 
#include "ScreenSpaceEffects.h"

#define FXAA_TYPE2

class CFXAA: public IScreenSpaceEffect
{
public:
	CFXAA( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }

	virtual void Render( int x, int y, int w, int h );
 
private:
	bool				m_bEnabled;

	CMaterialReference	m_Luma;
	CMaterialReference	m_FXAA;
};

class CUnsharpEffect : public IScreenSpaceEffect
{
public:
	CUnsharpEffect( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }

	virtual void Render( int x, int y, int w, int h );
 
private:
	bool				m_bEnabled;

	CTextureReference	m_UnsharpBlurFB;
	CMaterialReference	m_UnsharpBlur;
	CMaterialReference	m_Unsharp;
};

class CSSAOEffect : public IScreenSpaceEffect
{
public:
	CSSAOEffect( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }

	virtual void Render( int x, int y, int w, int h );
 
private:
	bool				m_bEnabled;

	CTextureReference	m_SSAOFB0;
	CTextureReference	m_SSAOFB1;
	CTextureReference	m_SSAOFB2;

	CMaterialReference	m_ShowZ;
	CMaterialReference	m_SSAO;
	CMaterialReference	m_SSAOBlur;
	CMaterialReference	m_SSAOCombine;
};

class CSunShaftEffect : public IScreenSpaceEffect
{
public:
	CSunShaftEffect( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }

	virtual bool IsEnabled( ) { return m_bEnabled; }
	virtual bool ShaftsRendering( void );

	virtual void Render( int x, int y, int w, int h );
 
private:
	bool				m_bEnabled;

	CMaterialReference	m_SunShaft_BlurX;
	CMaterialReference	m_SunShaft_BlurY;

	CMaterialReference	m_SunShaftBlendMat;
	CMaterialReference	m_SunShaftMask;
	CMaterialReference	m_SunShaftDebug;
};

class CWaterEffects : public IScreenSpaceEffect
{
public:
	CWaterEffects( void ) { };

	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params );
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }

	virtual void Render( int x, int y, int w, int h );

	virtual float GetViscosity( ) { return fViscosity; }
	virtual float GetAmount( ) { return fAmount; }
	virtual void SetViscosity( float fNewViscosity ) { fViscosity = fNewViscosity; }
	virtual void SetAmount( float fNewAmount ) { fAmount = fNewAmount; }
	virtual bool IsUnderwater( ) { return m_bUnderwater; }

private:
	bool	m_bEnabled;
	bool	m_bUnderwater;

	float	fViscosity;
	float	fAmount;

	CMaterialReference	m_ChromaticDisp;
	CMaterialReference	m_WaterFX;
	CMaterialReference	m_BlurX;
	CMaterialReference	m_BlurY;
};

class CHealthEffects : public IScreenSpaceEffect
{
public:
	CHealthEffects( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }

	virtual void Render( int x, int y, int w, int h );
 
private:
	bool	m_bEnabled;

	float	fDispersionAmount;
	float	fVignettingAmount;
	float	fDispersionLerpTo;
	float	fVignettingLerpTo;
	float	fNoiseAmount;
	float	fNoiseLerpTo;
	float	fScanAmount;
	float	fScanLerpTo;
	float	fStaminaBlurAmount;
	float	fStaminaBlurAmountLerpTo;
	float	fSightDOF;
	float	fSightDOFLerpTo;
	float	fBlurWidth;
	float	fBlurWidthLerpTo;
	float	fDesat;
	float	fDesatLerpTo;
	float	fPain;
	float	fPainLerpTo;

	//int		iLastHealth;
	//bool	bDisableDispersionOneFrame;

	CMaterialReference	m_ChromaticDisp;
	CMaterialReference	m_VignetMat;
	CMaterialReference	m_Noise;
	CMaterialReference	m_BlurX;
	CMaterialReference	m_BlurY;
	CMaterialReference	m_RadialBlurX;
	CMaterialReference	m_RadialBlurY;
	CMaterialReference	m_RadialBlurMat;
	CMaterialReference	m_DesatMat;
	CMaterialReference	m_Pain;
};

class CEntGlowEffect : public IScreenSpaceEffect
{
public:
	CEntGlowEffect( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }
 
	virtual void RegisterEnt( EHANDLE hEnt, Color glowColor = Color(255,255,255,64), float fGlowScale = 1.0f );
	virtual void DeregisterEnt( EHANDLE hEnt );
 
	virtual void SetEntColor( EHANDLE hEnt, Color glowColor );
	virtual void SetEntGlowScale( EHANDLE hEnt, float fGlowScale );
 
	virtual void Render( int x, int y, int w, int h );
 
protected:
	int FindGlowEnt( EHANDLE hEnt );
	void RenderToStencil( int idx, IMatRenderContext *pRenderContext );
	void RenderToGlowTexture( int idx, IMatRenderContext *pRenderContext );
 
private:
	bool			m_bEnabled;
 
	struct sGlowEnt
	{
		EHANDLE	m_hEnt;
		float	m_fColor[4];
		float	m_fGlowScale;
	};
 
	CUtlVector<sGlowEnt*>	m_vGlowEnts;
 
	CTextureReference	m_GlowBuff1;
	CTextureReference	m_GlowBuff2;
 
	CMaterialReference	m_WhiteMaterial;
	CMaterialReference	m_EffectMaterial;
 
	CMaterialReference	m_BlurX;
	CMaterialReference	m_BlurY;
};

class CColorCorrectionEffect : public IScreenSpaceEffect
{
public:
	CColorCorrectionEffect( void ) { };
 
	virtual void Init( void );
	virtual void Shutdown( void );
	virtual void SetParameters( KeyValues *params ) {};
	virtual void Enable( bool bEnable ) { m_bEnabled = bEnable; }
	virtual bool IsEnabled( ) { return m_bEnabled; }

	virtual void Render( int x, int y, int w, int h );
 
private:
	bool				m_bEnabled;

	CMaterialReference	m_Negative;
	CMaterialReference	m_BleachBypass;
	CMaterialReference	m_ColorClipping;
	CMaterialReference	m_CrossProcessing;
	CMaterialReference	m_NextGen;
	CMaterialReference	m_Complements;
	CMaterialReference	m_CubicDistortion;
	CMaterialReference	m_Desaturate;
	CMaterialReference	m_Nightvision;
};

#endif