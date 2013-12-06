#ifndef DEVICEDX11_IMP_H
#define DEVICEDX11_IMP_H

#include "Common/ZHSTD.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Widgets{
        class WindowsInfo;
    }

    namespace Graphics{
        class DeviceDX11Imp{
        public:
            bool startDevice( ZH::Widgets::WindowsInfo* winInfo = NULL );
            void shutdownDevice();
            bool good()const;
        private:
            ID3D11Device        *m_pDevice;
            ID3D11DeviceContext *m_pContext;
            IDXGISwapChain      *m_pSwapChain;
            D3D_FEATURE_LEVEL    m_FeatureLevel;
            DXGI_ADAPTER_DESC    m_sAdapterDesc;
        private:
            friend class DeviceDX11;
            CLASS_COMMON_PROTECTED_DECLEARATION(DeviceDX11Imp)
        };

    } // Graphics
} // ZH

#endif
