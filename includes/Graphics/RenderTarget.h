#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Array.h"
#include "Util/Cache.h"

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


            friend class DeviceDX11;
            friend class DeviceDX11Imp;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderTarget*>;
        typedef ZH::UTIL::Array<RenderTarget*> RenderTargetPtrArray;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<RenderTarget>;
        typedef ZH::UTIL::Cache<RenderTarget> RenderTargetCache;

    } // end namespace Graphics
} // end namespace ZH

#endif
