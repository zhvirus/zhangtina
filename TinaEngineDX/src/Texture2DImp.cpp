#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Internal/Graphics/Texture2DImp.h"

namespace ZH{
    namespace Graphics{

        Texture2DImp::Texture2DImp( ID3D11Texture2D* pTex_d3d ):
            m_pTexture2D_d3d(pTex_d3d)
        {
        }

        Texture2DImp::~Texture2DImp()
        {
            SAFE_RELEASE<ID3D11Texture2D*>( m_pTexture2D_d3d );
        }

        bool Texture2DImp::isValid()
        {
            return (NULL != m_pTexture2D_d3d);
        }



    } // Graphics
} // ZH



