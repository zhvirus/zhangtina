#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderTarget.h"


namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderTarget, "RenderTarget" )

        RenderTarget::RenderTarget( Texture2D* tex, ID3D11RenderTargetView* rtView ):
            m_tex2D( tex ),
            m_rtView( rtView )
        {
        }

        bool RenderTarget::operator== ( const RenderTarget& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }
    }
}