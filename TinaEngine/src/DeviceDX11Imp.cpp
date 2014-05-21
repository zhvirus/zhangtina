#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Widget/Window.h"
#include "Graphics/RenderTarget.h"
#include "Internal/Graphics/RenderTargetImp.h"
#include "Math/float4.h"

namespace ZH{
    namespace Graphics{

        bool chooseAdapter( IDXGIAdapter*& oAdapter, DXGI_ADAPTER_DESC& adapDesc )
        {
            // Reset the value
            oAdapter     = NULL;

            // Create a DXGIFactory object.
            IDXGIFactory* pFactory = NULL;
            if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&pFactory)))
            {
                return false;
            }

            // Enumuerate the adapters
            IDXGIAdapter *pAdapter      = NULL;
            SIZE_T        lastVideoMemSize = 0;
            for (unsigned int i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
            {
                DXGI_ADAPTER_DESC aDesc;
                if ( pAdapter && ( S_OK == pAdapter->GetDesc( &aDesc ))){
                    ZH_OUT(aDesc.Description);
                    if ( aDesc.DedicatedVideoMemory > lastVideoMemSize ){
                        lastVideoMemSize = aDesc.DedicatedVideoMemory;
                        oAdapter = pAdapter;
                        // Save desc
                        adapDesc.VendorId = aDesc.VendorId;
                        adapDesc.DeviceId = aDesc.DeviceId;
                        adapDesc.SubSysId = aDesc.SubSysId;
                        adapDesc.Revision = aDesc.Revision;
                        adapDesc.DedicatedVideoMemory   = aDesc.DedicatedVideoMemory;
                        adapDesc.DedicatedSystemMemory  = aDesc.DedicatedSystemMemory;
                        adapDesc.SharedSystemMemory     = aDesc.SharedSystemMemory;
                        adapDesc.AdapterLuid            = aDesc.AdapterLuid;
                        wcscpy_s( adapDesc.Description, sizeof(aDesc.Description)/sizeof(WCHAR), aDesc.Description );
                    }
                }
            }

            // Release factory
            if(pFactory)
            {
                pFactory->Release();
                pFactory = NULL;
            }

            return true;
        }

        DeviceDX11Imp::DeviceDX11Imp():
            m_pDevice(NULL),
            m_pContext(NULL),
            m_pSwapChain(NULL),
            m_FeatureLevel(D3D_FEATURE_LEVEL_11_0)
        {
        }

        DeviceDX11Imp::~DeviceDX11Imp()
        {
            shutdownDevice();
        }

        bool DeviceDX11Imp::good()const
        {
            return m_pDevice && m_pContext;
        }

        bool DeviceDX11Imp::startDevice( ZH::Widgets::WindowsInfo* winInfo/* = NULL */)
        {
            HRESULT hr = S_OK;

            // If already started
            if ( good() )
                return true;

            // Check win info
            if ( winInfo ){
                if ( (winInfo->fHeight <= 0) ||
                     (winInfo->fWidth <=0) ||
                     (!winInfo->fWndHandle)){
                         ZH_ERR("DeviceDX11Imp::startDevice()--WindosInfo is invalid!");
                         return false;
                }
            }

            // Device creating flag
            UINT createDeviceFlags = 0;
#ifdef _DEBUG
            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

            // Driver types
            D3D_DRIVER_TYPE driverTypes[] =
            {
                D3D_DRIVER_TYPE_HARDWARE/*,
                D3D_DRIVER_TYPE_WARP,
                D3D_DRIVER_TYPE_REFERENCE*/
            };
            const UINT numDriverTypes = ARRAYSIZE( driverTypes );

            // Feature levels
            D3D_FEATURE_LEVEL featureLevels[] =
            {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0
            };
            const UINT numFeatureLevels = ARRAYSIZE( featureLevels );

            // Choose adapter
            IDXGIAdapter* pAdapter = NULL;
            chooseAdapter( pAdapter, m_sAdapterDesc );

            // Create device only
            if ( !winInfo ){
                // Create device
                for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
                {
                    hr = D3D11CreateDevice(
                        NULL,                                // NULL to use first video adapter
                        driverTypes[driverTypeIndex],        // Hardware -> Warp -> Software
                        NULL,                                // NULL for software implementation
                        createDeviceFlags,                   // Device creating flags
                        featureLevels,                       // Feature levels
                        numFeatureLevels,                    // Number of feature levels
                        D3D11_SDK_VERSION,                   // SDK Version
                        &m_pDevice,                          // Device
                        &m_FeatureLevel,                     // Feature level
                        &m_pContext                          // Context
                        );
                    if( SUCCEEDED( hr ) )
                        break;
                }
            }
            // Create device with swap chain
            else{
                // Swap chain description
                DXGI_SWAP_CHAIN_DESC sd;
                ZeroMemory( &sd, sizeof( sd ) );
                sd.BufferCount = 1;
                sd.BufferDesc.Width = winInfo->fWidth;
                sd.BufferDesc.Height = winInfo->fHeight;
                sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                sd.BufferDesc.RefreshRate.Numerator = 60;
                sd.BufferDesc.RefreshRate.Denominator = 1;
                sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sd.OutputWindow = winInfo->fWndHandle;
                sd.SampleDesc.Count = 1;
                sd.SampleDesc.Quality = 0;
                sd.Windowed = TRUE;

                // Create device and swap chain
                for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
                {
                    hr = D3D11CreateDeviceAndSwapChain(
                        NULL,                                // NULL to use first video adapter
                        driverTypes[driverTypeIndex],        // Hardware -> Warp -> Software
                        NULL,                                // NULL for software implementation
                        createDeviceFlags,                   // Device creating flags
                        featureLevels,                       // Feature levels
                        numFeatureLevels,                    // Number of feature levels
                        D3D11_SDK_VERSION,                   // SDK Version
                        &sd,                                 // Swap chain description
                        &m_pSwapChain,                       // Swap chain
                        &m_pDevice,                          // Device
                        &m_FeatureLevel,                     // Feature level
                        &m_pContext                          // Context
                        );
                    if( SUCCEEDED( hr ) )
                        break;
                }
            }

            if( FAILED( hr ) || !m_pDevice || !m_pDevice || ( winInfo && !m_pSwapChain) ){
                ZH_ERR("Device creating failed.");
                shutdownDevice();
                return false;
            }

            ZH_OUT("Device created successfully.");
            return true;
        }

        void DeviceDX11Imp::shutdownDevice()
        {
            if( m_pSwapChain ){
                m_pSwapChain->Release();
                m_pSwapChain = NULL;
            }

            if( m_pContext ) {
                m_pContext->ClearState();
                m_pContext->Release();
                m_pContext = NULL;
            }

            if( m_pDevice ){
                m_pDevice->Release();
                m_pDevice = NULL;
            }
        }


        bool DeviceDX11Imp::setRenderTarget( RenderTarget* rt )
        {
            if ( !good() || !rt ){
                return false;
            }

            RenderTargetImp* rtImp = rt->m_pRtImp;
            if ( !rtImp ){
                return false;
            }

            ID3D11RenderTargetView* rtView = rtImp->getRenderTargetView_d3d();
            if( !rtView ){
                return false;
            }

            m_pContext->OMGetRenderTargets( 1, &rtView, NULL );

            return true;
        }

        bool DeviceDX11Imp::clearRenderTargetView( ZH::Graphics::RenderTarget* rt, const ZH::Math::float4& col )
        {
            if ( !good() || !rt ){
                return false;
            }

            RenderTargetImp* rtImp = rt->m_pRtImp;
            if ( !rtImp ){
                return false;
            }

            ID3D11RenderTargetView* rtView = rtImp->getRenderTargetView_d3d();
            if( !rtView ){
                return false;
            }

            float ClearColor[4] = { col[0], col[1], col[2], col[3] };
            m_pContext->ClearRenderTargetView( rtView, ClearColor );

            return true;

        }

        bool DeviceDX11Imp::present()
        {
            if ( !good() ){
                return false;
            }

            m_pSwapChain->Present( 0, 0 );

            return true;
        }






    } // Graphics
} // ZH

