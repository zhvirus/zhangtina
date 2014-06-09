#include "Internal/Graphics/RenderTargetImp.h"

namespace ZH{
    namespace Graphics{

        RenderTargetImp::RenderTargetImp( ID3D11RenderTargetView* pRTView_d3d ):
            m_pRenderTargetView_d3d(pRTView_d3d)
        {
        }

        RenderTargetImp::~RenderTargetImp()
        {
            SAFE_RELEASE( m_pRenderTargetView_d3d );
        }

        bool RenderTargetImp::isValid()
        {
            return (NULL != m_pRenderTargetView_d3d);
        }



    } // Graphics
} // ZH


