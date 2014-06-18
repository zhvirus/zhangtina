#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/DeviceDX11.h"

#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Internal/Graphics/Internal_common_graphics.h"

#include "Internal/Graphics/Texture2DImp.h"
#include "Internal/Graphics/RenderTargetImp.h"
#include "Internal/Graphics/DataToD3D.h"
#include "Internal/Graphics/VertexBufferImp.h"
#include "Internal/Graphics/IndexBufferImp.h"

#include "Graphics/Texture2D.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/ResourceManager.h"



#define IMP_PTR ((ZH::Graphics::DeviceDX11Imp*)m_pImp)

namespace ZH{
    namespace Graphics{

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
                ZH::Util::ENG_DBG("Device started successfully.\n");

            }else{
                m_status = DEVICE_STATUS_SHUTDOWN;
                ZH::Util::ENG_ERR("Device start failed!\n");
            }
            return startResult;
        }

        bool DeviceDX11::shutdown()
        {
            if ( IMP_PTR ){
                IMP_PTR->shutdownDevice();
            }

            m_status = DEVICE_STATUS_SHUTDOWN;

            ZH::Util::ENG_DBG("Device shutdown.\n");

            return true;
        }

        RenderTarget* DeviceDX11::createRenderTarget( const char* const name, Texture2D* pTex2d )
        {
            // Check device
            if ( !isRunning() ){
                ZH::Util::ENG_ERR("DeviceDX11::createVertexBuffer():Device is not ready!\n");
                return NULL;
            }
            ID3D11Device* pDevice_d3d = IMP_PTR->m_pDevice;
            if( !pDevice_d3d ){
                ZH::Util::ENG_ERR("DeviceDX11::createVertexBuffer(): D3D device is not valid!\n");
                return NULL;
            }

            // Check texture
            if ( !pTex2d || !pTex2d->isValid() ){
                ZH::Util::ENG_ERR("Texure2D is not valid, can't create render target!\n");
                return NULL;
            }

            ID3D11Texture2D* pTex2d_d3d =
                pTex2d->m_pTex2DImp?pTex2d->m_pTex2DImp->getTex2D_d3d():NULL;
            if( !pTex2d_d3d ){
                ZH::Util::ENG_ERR("D3D texture2D is not valid, can't create render target!\n");
                return NULL;
            }

            ID3D11RenderTargetView* pRtv_d3d = NULL;

            HRESULT hr = S_OK;
            hr = pDevice_d3d->CreateRenderTargetView( pTex2d_d3d, NULL, &pRtv_d3d );

            if ( FAILED(hr) ){
                ZH::Util::ENG_ERR("D3D create render target view failed!\n");
                return NULL;
            }

            RenderTargetImp* pRTImp = new RenderTargetImp( pRtv_d3d );
            RenderTarget* ret_rt    = new RenderTarget( name, pRTImp, pTex2d );

            ZH::Util::ENG_INF("Render target(\"%s\") created successfully.\n", name);

            return ret_rt;
        }

        // Local buffer create helper function
        static ID3D11Buffer* createBufferImp(
            ID3D11Device* pDevice_d3d,
            const BUFFER_DESC& desc,
            const SUBRESOURCE_DATA& data)
        {
            if( !pDevice_d3d ){
                ZH::Util::ENG_ERR("DeviceDX11::createBufferImp(): D3D device is not valid!\n");
                return NULL;
            }

            // BUFFER DESC
            D3D11_BUFFER_DESC d3d_buff_desc;
            toD3DBufferDesc( d3d_buff_desc, desc );

            // SUBRESOURCE_DATA
            D3D11_SUBRESOURCE_DATA d3d_subresource_data;
            toD3DSubResourceData( d3d_subresource_data, data );

            // Create buffer
            ID3D11Buffer* d3d_buffer = NULL;
            HRESULT hr = pDevice_d3d->CreateBuffer( &d3d_buff_desc, &d3d_subresource_data, &d3d_buffer );
            if ( (S_OK == hr) && d3d_buffer ){
                return d3d_buffer;
            }else{
                return NULL;
            }
        }

        VertexBuffer* DeviceDX11::createVertexBuffer(
            const char* const name,
            const BUFFER_DESC& desc,
            const SUBRESOURCE_DATA& data)
        {
            // Check device
            if ( !isRunning() ){
                ZH::Util::ENG_ERR("DeviceDX11::createVertexBuffer():Device is not ready!\n");
                return NULL;
            }

            // Check buffer type
            assert( desc.bind_flag == BIND_VERTEX_BUFFER );
            if ( desc.bind_flag != BIND_VERTEX_BUFFER ){
                ZH::Util::ENG_ERR("DeviceDX11::createVertexBuffer():Bind_flag type does not match!\n");
                return NULL;
            }

            // Create buffer
            ID3D11Buffer* pBuffer_d3d = createBufferImp( IMP_PTR->m_pDevice, desc, data );
            if ( !pBuffer_d3d ){
                ZH::Util::ENG_ERR("Vertex Buffer(\"%s\") creating failed!\n", name);
                return NULL;
            }

            ZH::Util::ENG_DBG("Vertex Buffer(\"%s\") created successfully.\n", name);

            // Create Vertex buffer
            VertexBufferImp* pVBImp = new VertexBufferImp( pBuffer_d3d );
            VertexBuffer*    ret_vb = new VertexBuffer( name, pVBImp, desc );

            return ret_vb;
        }


        IndexBuffer* DeviceDX11::createIndexBuffer(
            const char* const name,
            const BUFFER_DESC& desc,
            const SUBRESOURCE_DATA& data)
        {
            // Check device
            if ( !isRunning() ){
                ZH::Util::ENG_ERR("DeviceDX11::createIndexBuffer():Device is not ready!\n");
                return NULL;
            }

            // Check buffer type
            assert( desc.bind_flag == BIND_INDEX_BUFFER );
            if ( desc.bind_flag != BIND_INDEX_BUFFER ){
                ZH::Util::ENG_ERR("DeviceDX11::createIndexBuffer():Bind_flag type does not match!\n");
                return NULL;
            }

            // Create buffer
            ID3D11Buffer* pBuffer_d3d = createBufferImp( IMP_PTR->m_pDevice, desc, data );
            if ( !pBuffer_d3d ){
                ZH::Util::ENG_ERR("Index Buffer(\"%s\") creating failed!\n", name);
                return NULL;
            }

            ZH::Util::ENG_DBG("Index Buffer(\"%s\") created successfully.\n", name);

            // Create Index buffer
            IndexBufferImp* pIBImp = new IndexBufferImp( pBuffer_d3d );
            IndexBuffer*    ret_ib = new IndexBuffer( name, pIBImp, desc );

            return ret_ib;
        }


        bool DeviceDX11::getBackBuffer( Texture2D*& pBf_ret )
        {
            // Check device
            if ( !isRunning() ){
                ZH::Util::ENG_ERR("ERROR: Device is not ready, can't get back buffer!\n");
                return false;
            }

            // Initialize it to NULL at the beginning
            pBf_ret = NULL;

            // Check swapchain
            IDXGISwapChain* pSwapchain = IMP_PTR->m_pSwapChain;
            if ( !pSwapchain ){
                ZH::Util::ENG_ERR("Swap chain is not ready, can't get back buffer from it!\n");
                return false;
            }

            // Get back buffer from swapchain
            ID3D11Texture2D *pBackBuffer = NULL;
            if( FAILED( pSwapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer ) ) ){
                ZH::Util::ENG_ERR("Get back buffer from swap chain failed!\n");
                return false;
            }

            // Create a Texture2D object to hold the back buffer
            Texture2DImp* pTex2DImp = new Texture2DImp( pBackBuffer );
            pBf_ret = new Texture2D( Texture2D::m_sBackBufferName, pTex2DImp );

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
