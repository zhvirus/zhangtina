#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{

        class Texture2D;

        class ZH_GRAPHICS_DLL RenderTarget : public Resource {
        public:
            RenderTarget( Texture2D*, ID3D11RenderTargetView* );

        private:
            Texture2D* m_tex2D;
            ID3D11RenderTargetView* m_rtView;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };



    } // end namespace Graphics
} // end namespace ZH

#endif
