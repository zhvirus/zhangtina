#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Device.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        Device::Device():
            m_status(DEVICE_STATUS_SHUTDOWN)
        {
        }

    }
}
