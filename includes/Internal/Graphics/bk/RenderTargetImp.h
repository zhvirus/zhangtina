#ifndef RENDERTARGET_IMP_H
#define RENDERTARGET_IMP_H

#include "Common/ZHSTD.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        class RenderTargetImp{
        public:
            RenderTargetImp( ID3D11RenderTargetView* );
            ~RenderTargetImp();

            bool isValid();

            ID3D11RenderTargetView* getRenderTargetView_d3d() { return m_pRenderTargetView_d3d; }

        private:
            ID3D11RenderTargetView* m_pRenderTargetView_d3d;

        };
    } // Graphics
} // ZH



#endif


