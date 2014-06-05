#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{

        class Texture2D;
        class RenderTargetImp;

        class ZH_GRAPHICS_DLL RenderTarget : public Resource {
        public:
            virtual ~RenderTarget();
            virtual bool isValid();

            bool operator == (const RenderTarget&);

            static const char m_sDefaultRenderTargetName[30];
        private:
            RenderTarget(  const char* const, Texture2D*, RenderTargetImp* );

            Texture2D*       m_pTex2D;
            RenderTargetImp* m_pRtImp;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION

            friend class DeviceDX11;
            friend class DeviceDX11Imp;
        };



    } // end namespace Graphics
} // end namespace ZH

#endif
