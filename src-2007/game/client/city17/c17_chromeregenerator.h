#ifndef CHROMEREGENERATOR_H_
#define CHROMEREGENERATOR_H_
#ifdef _WIN32
#pragma once
#endif

#include "materialsystem/ITexture.h"
#include "materialsystem/IMaterialProxy.h"
#include "materialsystem/IMaterial.h"
#include "materialsystem/IMaterialVar.h"

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

#include <WebCore.h>

#undef Yield
#undef GetObject

class CChromeRegenerator;

struct Browser
{
        Awesomium::WebView *view;
        Awesomium::WebViewListener *listener;

        ITexture *texture;
        CChromeRegenerator *regen;

        int width, height;
        int texwidth, texheight;

        int handler;
        bool free;

        bool wipeTex;
};

class CChromeRegenerator : public ITextureRegenerator
{
public:
        CChromeRegenerator( Browser *browser) : pBrowser(browser) {};
        virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect );
        virtual void Release( void );

private:
        Browser *pBrowser;
};

class CChromeProxy: public IMaterialProxy
{
public:
	CChromeProxy();
	virtual ~CChromeProxy();
	virtual bool Init( IMaterial* pMaterial, KeyValues *pKeyValues );
	virtual void OnBind( void *pC_BaseEntity );
	virtual IMaterial *GetMaterial();
	virtual void Release( void ) { delete this; }

private:
	IMaterialVar		*m_pTextureVar;
	IMaterialVar		*m_pWebsite;
	ITexture			*m_pTexture;
	ITextureRegenerator	*m_pTextureRegen;
	Browser				*m_pBrowser;
};

#endif //CHROMEREGENERATOR_H_