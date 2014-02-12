#ifndef DEVICE_H
#define DEVICE_H

#include "Common/ZHSTD.h"
#include "Graphics/Name.h"

// Forwards
namespace ZH{
    namespace Widgets{
        class WindowsInfo;
    }
}

namespace ZH{
    namespace Graphics{

        enum DEVICE_STATUS{
            DEVICE_STATUS_RUNNING,
            DEVICE_STATUS_PAUSED,
            DEVICE_STATUS_SHUTDOWN
        };

        class ZH_GRAPHICS_DLL Device : public Name
        {
        public:
            virtual bool isRunning()const{return m_status == DEVICE_STATUS_RUNNING;};
            virtual bool isPaused()const{ return m_status == DEVICE_STATUS_PAUSED; }
            virtual bool isShutdown()const{ return m_status == DEVICE_STATUS_SHUTDOWN; }
            virtual bool start( ZH::Widgets::WindowsInfo* ) = 0;
            virtual bool shutdown() = 0;

        protected:
            Device();
            DEVICE_STATUS m_status;
        };

    }

}

#endif
