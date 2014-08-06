#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Internal/Graphics/AccessInternal.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"

namespace ZH{
    namespace Graphics{

        ID3D11Device* AccessInternal::device()
        {
            DeviceDX11* pDeviceDX11 = DeviceDX11::instance();
            if( !pDeviceDX11 ){
                return NULL;
            }

            DeviceDX11Imp* pDeviceDX11Imp = (DeviceDX11Imp*)pDeviceDX11->m_pImp;
            if( !pDeviceDX11Imp ){
                return NULL;
            }

            return pDeviceDX11Imp->m_pDevice;
        }
        ID3D11DeviceContext* AccessInternal::context()
        {
            DeviceDX11* pDeviceDX11 = DeviceDX11::instance();
            if( !pDeviceDX11 ){
                return NULL;
            }

            DeviceDX11Imp* pDeviceDX11Imp = (DeviceDX11Imp*)pDeviceDX11->m_pImp;
            if( !pDeviceDX11Imp ){
                return NULL;
            }

            return pDeviceDX11Imp->m_pContext;
        }


    }
}
