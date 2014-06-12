#ifndef DEVICEDX11_IMP_H
#define DEVICEDX11_IMP_H

#include "Common/ZHSTD.h"
#include "Internal/Graphics/Internal_common_graphics.h"


namespace ZH{
    namespace Widgets{
        class WindowsInfo;
    }
    namespace Graphics{
        class RenderTarget;
    }
    namespace Math{
        class float4;
    }

    namespace Graphics{
        class DeviceDX11Imp{
        public:
            bool startDevice( ZH::Widgets::WindowsInfo* winInfo = NULL );
            void shutdownDevice();
            bool good()const;
            bool setRenderTarget( ZH::Graphics::RenderTarget* );
            bool clearRenderTargetView( ZH::Graphics::RenderTarget*, const ZH::Math::float4& );
            bool present();
        private:
            ID3D11Device        *m_pDevice;
            ID3D11DeviceContext *m_pContext;
            IDXGISwapChain      *m_pSwapChain;
            D3D_FEATURE_LEVEL    m_FeatureLevel;
            DXGI_ADAPTER_DESC    m_sAdapterDesc;
        private:
            friend class DeviceDX11;
            friend class ResourceManager;
            friend class AccessInternal;
            CLASS_COMMON_PROTECTED_DECLEARATION(DeviceDX11Imp)
        };

    } // Graphics
} // ZH

#endif
