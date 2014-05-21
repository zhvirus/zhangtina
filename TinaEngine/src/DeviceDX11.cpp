#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Graphics/Texture2D.h"
#include "Internal/Graphics/Texture2DImp.h"
#include "Graphics/RenderTarget.h"
#include "Internal/Graphics/RenderTargetImp.h"
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

        bool DeviceDX11::createRenderTarget( Texture2D* pTex2d, RenderTarget*& pRt, const char* const name )
        {
            // Check device
            if ( !isRunning() ){
                std::cerr<<"ERROR: Device is not ready, can't create render target!"<<std::endl;
                return false;
            }

            // Check texture
            if ( !pTex2d || !pTex2d->isValid() ){
                std::cerr<<"ERROR: tex is not valid, can't create render target!"<<std::endl;
                return false;
            }

            ID3D11Texture2D* pTex2d_d3d =
                pTex2d->m_pTex2DImp?pTex2d->m_pTex2DImp->getTex2D_d3d():NULL;
            if( !pTex2d_d3d ){
                std::cerr<<"ERROR: d3d tex is not valid, can't create render target!"<<std::endl;
                return false;
            }

            ID3D11RenderTargetView* pRtv_d3d = NULL;

            ID3D11Device* pDevice_d3d = IMP_PTR->m_pDevice;
            if( !pDevice_d3d ){
                std::cerr<<"ERROR: d3d device is not valid, can't create render target!"<<std::endl;
                return false;
            }

            HRESULT hr = S_OK;
            hr = pDevice_d3d->CreateRenderTargetView( pTex2d_d3d, NULL, &pRtv_d3d );

            if ( FAILED(hr) ){
                std::cerr<<"ERROR: d3d create render target view failed!"<<std::endl;
                return false;
            }

            RenderTargetImp* pRTImp = new RenderTargetImp( pRtv_d3d );

            pRt = new RenderTarget( pTex2d, pRTImp, name );

            return true;
        }


        bool DeviceDX11::getBackBuffer( Texture2D*& pBf_ret )
        {
            // Check device
            if ( !isRunning() ){
                std::cerr<<"ERROR: Device is not ready, can't get back buffer!"<<std::endl;
                return false;
            }

            // Initialize it to NULL at the beginning
            pBf_ret = NULL;

            // Check swapchain
            IDXGISwapChain* pSwapchain = IMP_PTR->m_pSwapChain;
            if ( !pSwapchain ){
                std::cerr<<"ERROR: Swap chain is not ready, can't get back buffer from it!!"<<std::endl;
                return false;
            }

            // Get back buffer from swapchain
            ID3D11Texture2D *pBackBuffer = NULL;
            if( FAILED( pSwapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer ) ) ){
                std::cerr<<"ERROR: Get back buffer from swap chain failed!"<<std::endl;
                return false;
            }

            // Create a Texture2D object to hold the back buffer
            Texture2DImp* pTex2DImp = new Texture2DImp( pBackBuffer );
            pBf_ret = new Texture2D( pTex2DImp, Texture2D::m_sBackBufferName );

            return true;

        }

        bool DeviceDX11::setRenderTarget( RenderTarget* rt )
        {
            if ( IMP_PTR ){
                return IMP_PTR->setRenderTarget( rt );
            }
            return false;
        }


        bool DeviceDX11::clearRenderTargetView( RenderTarget* rt, const ZH::Math::float4& col )
        {
            if ( IMP_PTR ){
                return IMP_PTR->clearRenderTargetView( rt, col );
            }
            return false;
        }

        bool DeviceDX11::present()
        {
            if ( IMP_PTR ){
                return IMP_PTR->present();
            }
            return false;
        }





    }
}
