#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Texture2D.h"
#include "Internal/Graphics/Texture2DImp.h"


namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Texture2D )

        const char Texture2D::m_sBackBufferName[] = "BackBuffer";


        bool TEX2D_SAMPLE_DESC::operator==(const TEX2D_SAMPLE_DESC& v)
        {
            return
                ( Count == v.Count ) &&
                ( Quality == v.Quality );
        }

        bool TEX2D_DESC::operator==(const TEX2D_DESC& v)
        {
            return
                (Width == v.Width) &&
                (Height == v.Height) &&
                (MipLevels == v.MipLevels) &&
                (ArraySize == v.ArraySize) &&
                (Format == v.Format) &&
                (SampleDesc == v.SampleDesc) &&
                (Usage == v.Usage) &&
                (BindFlags == v.BindFlags) &&
                (CPUAccessFlags == v.CPUAccessFlags ) &&
                (MiscFlags == v.MiscFlags)
                ;
        }

        Texture2D::Texture2D( const char* const name , Texture2DImp* pTex2dImp ):
            Resource( name ),
            m_pTex2DImp(pTex2dImp)
        {
        }

        Texture2D::~Texture2D()
        {
            delete m_pTex2DImp;
        }

        bool Texture2D::isValid()
        {
            return m_pTex2DImp && m_pTex2DImp->isValid();
        }

        bool Texture2D::operator == ( const Texture2D& v )
        {
            return
                (*(v.m_name) == *m_name) &&
                (m_sDesc == v.m_sDesc);
        }


    }
}
