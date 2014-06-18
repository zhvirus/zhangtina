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

        class ZH_GRAPHICS_DLL RenderTarget : public Resource
        {
            CLASS_IDENTIFIER( E_CID_RENDER_TARGET );

        public:
            virtual ~RenderTarget();
            virtual bool isValid();

            bool operator == (const RenderTarget&);

            static const char m_sDefaultRenderTargetName[30];
        private:
            RenderTarget(  const char* const, RenderTargetImp*, Texture2D* );
            RenderTargetImp* m_pRtImp;
            Texture2D*       m_pTex2D;

            friend class DeviceDX11;
            friend class DeviceDX11Imp;
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderTarget*>;
        typedef ZH::UTIL::Array<RenderTarget*> RenderTargetPtrArray;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<RenderTarget>;
        typedef ZH::UTIL::Cache<RenderTarget> RenderTargetCache;

    } // end namespace Graphics
} // end namespace ZH

#endif
