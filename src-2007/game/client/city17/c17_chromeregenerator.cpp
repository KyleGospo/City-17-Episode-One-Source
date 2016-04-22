#include "cbase.h"
#include "c17_chromeregenerator.h"
#include "toolframework_client.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

inline void FixNPOW2Alpha(unsigned char *data, int texheight, int height, int width, int rowspan)
{
        int WidthSpan = width << 2;
        int RowOffset;
        for(int row = 0; row < texheight; row++)
        {
                RowOffset = row * rowspan;

                for(int colOffset = 0; colOffset < rowspan; colOffset += 4)
                {
                        if(row > 0 && row < height && colOffset > 3 && colOffset < WidthSpan)
                                data[RowOffset + colOffset + 3] = 255;
                        else
                                memset(data + (RowOffset + colOffset), 0, 4);
                }
        }
}

// tex needs to be procedural!!
void CChromeRegenerator::RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect )
{
        if(pBrowser->wipeTex)
        {
                unsigned char *data = pVTFTexture->ImageData( 0, 0, 0 );
                int rowspan = pVTFTexture->RowSizeInBytes( 0 );

                memset(data, 0, rowspan * pBrowser->texheight);

                pBrowser->wipeTex = false;
                return;
        }

        Awesomium::WebView *view = pBrowser->view;
        if(!view->isDirty())
                return;

        unsigned char *data = pVTFTexture->ImageData( 0, 0, 0 );
        int rowspan = pVTFTexture->RowSizeInBytes( 0 );

        view->render(data, rowspan, 4);

        FixNPOW2Alpha(data, pBrowser->texheight, pBrowser->height, pBrowser->width, rowspan);
}

void CChromeRegenerator::Release()
{

}

CChromeProxy::CChromeProxy()
{
	m_pTextureVar = NULL;
	m_pWebsite = NULL;
	m_pTexture = NULL;
	m_pTextureRegen = NULL;
	m_pBrowser = NULL;
}

CChromeProxy::~CChromeProxy()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->SetTextureRegenerator( NULL );
	}
}

void CChromeProxy::OnBind( void *pC_BaseEntity )
{
	if ( ToolsEnabled() )
	{
		ToolFramework_RecordMaterialParams( GetMaterial() );
	}
}

IMaterial *CChromeProxy::GetMaterial()
{
	return m_pTextureVar->GetOwningMaterial();
}

bool CChromeProxy::Init( IMaterial *pMaterial, KeyValues *pKeyValues )
{
	bool found;
	
	m_pTextureVar = pMaterial->FindVar( "$basetexture", &found, false );  // Get a reference to our base texture variable
	if( !found )
	{
		m_pTextureVar = NULL;
		return false;
	}

	m_pWebsite = pMaterial->FindVar( "$WEBSITESURL", &found, false );
	if( !found )
		return false;

	//Make our browser here.
	//HINT: You may want to make it's destination 'website' chosen by the texture var.

	m_pTexture = m_pTextureVar->GetTextureValue();  // Now grab a ref to the actual texture
	if (m_pTexture != NULL && m_pBrowser != NULL)
	{
		m_pTextureRegen = new CChromeRegenerator(m_pBrowser);  // Here we create our regenerator
		m_pTexture->SetTextureRegenerator(m_pTextureRegen); // And here we attach it to the texture.
	}

	return true;
}

EXPOSE_INTERFACE( CChromeProxy, IMaterialProxy, "Awesomium" IMATERIAL_PROXY_INTERFACE_VERSION );