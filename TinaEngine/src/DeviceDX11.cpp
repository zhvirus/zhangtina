#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Graphics/Texture2D.h"
#include "Graphics/RenderTarget.h"

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

        bool DeviceDX11::createRenderTarget( Texture2D* tex2d, RenderTarget** rt )
        {
            if( !tex2d ){
                return false;
            }

            ID3D11Texture2D* tex2d_d3d = NULL;
            tex2d_d3d = tex2d->getTex();
            if( !tex2d_d3d ){
                return false;
            }

            ID3D11RenderTargetView* rtv_d3d = NULL;

            ID3D11Device* device_d3d = IMP_PTR->m_pDevice;
            if( !device_d3d ){
                return false;
            }

            HRESULT hr = S_OK;
            hr = device_d3d->CreateRenderTargetView( tex2d_d3d, NULL, &rtv_d3d );

            if ( FAILED(hr) ){
                return false;
            }

            *rt = new RenderTarget( tex2d, rtv_d3d );

            return true;
        }












    }
}
