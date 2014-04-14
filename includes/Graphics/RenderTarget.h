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
            virtual ~RenderTarget();
            virtual bool isValid();

            bool operator == (const RenderTarget&);


        private:
            RenderTarget( Texture2D*, ID3D11RenderTargetView*, const std::string& );

            Texture2D* m_tex2D;
            ID3D11RenderTargetView* m_rtView;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION

            friend class DeviceDX11;
        };



    } // end namespace Graphics
} // end namespace ZH

#endif
