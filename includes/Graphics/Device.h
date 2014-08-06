#ifndef DEVICE_H
#define DEVICE_H

#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"

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
            DEVICE_STATUS_SHUTDOWN
        };

        class ZH_GRAPHICS_DLL Device
        {
            CLASS_IDENTIFIER(E_CID_DEVICE);
        public:
            static Device& instance(){static Device d; return d;}
            virtual bool isRunning()const{return m_status == DEVICE_STATUS_RUNNING;};
            virtual bool isShutdown()const{ return m_status == DEVICE_STATUS_SHUTDOWN; }
            virtual bool start( const ZH::Widgets::WindowsInfo& );
            virtual bool shutdown();

        private:
            DEVICE_STATUS m_status;
        private:
            Device();
            ~Device();
            Device(const Device&){}
            Device& operator=(Device&){return *this;}
        };

    }

}

#endif
