#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderTarget.h"
#include "Graphics/Texture2D.h"


namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderTarget )

        RenderTarget::RenderTarget( Texture2D* tex, ID3D11RenderTargetView* view, const std::string& name ):
            m_tex2D( tex ),
            m_rtView(view)
        {
            Name::constructName( RenderTarget::m_cClassName, name );
        }

        RenderTarget::~RenderTarget()
        {
        }

        bool RenderTarget::operator == ( const RenderTarget& v )
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }

        bool RenderTarget::isValid()
        {
            return m_tex2D && m_tex2D->isValid() && m_rtView;
        }
    }
}