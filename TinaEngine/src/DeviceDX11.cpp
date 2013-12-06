#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"

#define IMP_PTR ((ZH::Graphics::DeviceDX11Imp*)m_pImp)

namespace ZH{
    namespace Graphics{

        CLASS_TYPE_NAME_DEFINITION(DeviceDX11,"DeviceDX11")

        DeviceDX11::DeviceDX11():
            Device(),
            m_pImp(NULL)
        {
        }

        DeviceDX11::~DeviceDX11()
        {
            shutdown();

            if( IMP_PTR ){
                delete IMP_PTR;
                m_pImp = NULL;
            }
        }

        bool DeviceDX11::start( ZH::Widgets::WindowsInfo* winInfo/* = NULL */)
        {
            if ( !IMP_PTR ){
                m_pImp = new DeviceDX11Imp();
            }

            bool startResult = IMP_PTR->startDevice( winInfo );
            if ( startResult ){
                m_status = DEVICE_STATUS_RUNNING;
                ZH_OUT("Device started successfully.");
            }else{
                m_status = DEVICE_STATUS_SHUTDOWN;
                ZH_OUT("Device start failed.");
            }
            return startResult;
        }

        bool DeviceDX11::shutdown()
        {
            if ( IMP_PTR ){
                IMP_PTR->shutdownDevice();
            }

            ZH_OUT("Device shutdown.");

            return true;
        }
    }
}
