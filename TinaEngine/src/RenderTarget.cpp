#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderTarget.h"
#include "Internal/Graphics/RenderTargetImp.h"
#include "Graphics/Texture2D.h"


namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderTarget )

        RenderTarget::RenderTarget( Texture2D* tex, RenderTargetImp* imp, const char* const name ):
            m_pTex2D( tex ),
            m_pRtImp( imp )
        {
            Name::constructName( RenderTarget::m_cClassName, name );
        }

        RenderTarget::~RenderTarget()
        {
            delete m_pRtImp;
        }

        bool RenderTarget::operator == ( const RenderTarget& v )
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }

        bool RenderTarget::isValid()
        {
            return m_pTex2D && m_pTex2D->isValid() && m_pRtImp;
        }
    }
}