#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Graphics/Texture2D.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/ResourceManager.h"

#define IMP_PTR ((ZH::Graphics::DeviceDX11Imp*)m_pImp)

namespace ZH{
    namespace Graphics{

        CLASS_TYPE_NAME_DEFINITION( DeviceDX11 )

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

        bool DeviceDX11::createRenderTarget( Texture2D* tex2d, RenderTarget** rt, const std::string& name )
        {
            // Check device
            if ( !isRunning() ){
                std::cerr<<"ERROR: Device is not ready, can't create render target!"<<std::endl;
                return false;
            }

            // Check texture
            if ( !tex2d || !tex2d->isValid() ){
                std::cerr<<"ERROR: tex is not valid, can't create render target!"<<std::endl;
                return false;
            }

            ID3D11Texture2D* tex2d_d3d = NULL;
            tex2d_d3d = tex2d->getTex();
            if( !tex2d_d3d ){
                std::cerr<<"ERROR: d3d tex is not valid, can't create render target!"<<std::endl;
                return false;
            }

            ID3D11RenderTargetView* rtv_d3d = NULL;

            ID3D11Device* device_d3d = IMP_PTR->m_pDevice;
            if( !device_d3d ){
                std::cerr<<"ERROR: d3d device is not valid, can't create render target!"<<std::endl;
                return false;
            }

            HRESULT hr = S_OK;
            hr = device_d3d->CreateRenderTargetView( tex2d_d3d, NULL, &rtv_d3d );

            if ( FAILED(hr) ){
                std::cerr<<"ERROR: d3d create render target view failed!"<<std::endl;
                return false;
            }

            *rt = new RenderTarget( tex2d, rtv_d3d, name );

            return true;
        }












    }
}
