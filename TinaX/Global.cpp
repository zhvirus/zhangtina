#include "stdafx.h"
#include "Global.h"
#include <stdarg.h>



#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"

#define MAX_PRINT_BUF_SIZE 0x1000

Global::Global():
    m_pDevice( NULL ),
    m_pRenderFragment( NULL )
{
}


Global::~Global()
{
}

void Global::print( const char* const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buffer[MAX_PRINT_BUF_SIZE];
    int ret = vsnprintf_s(buffer, MAX_PRINT_BUF_SIZE, _TRUNCATE, fmt, args);
    va_end(args);
    assert(ret != -1);
    std::cout<<"[Tina]: "<<buffer;
}

void Global::error( const char* const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buffer[MAX_PRINT_BUF_SIZE];
    int ret = vsnprintf_s(buffer, MAX_PRINT_BUF_SIZE, _TRUNCATE, fmt, args);
    va_end(args);
    assert(ret != -1);
    std::cerr<<"[Tina]: ERROR"<<buffer;
}


bool Global::startDevice( ZH::Widgets::WindowsInfo& winInfo )
{
    // Get device if not get
    if ( !m_pDevice ){
        // Get device
        m_pDevice = ZH::Graphics::DeviceDX11::instance();
    }

    // Start device if not started
    if ( m_pDevice->isRunning() ){
        return true;
    }

    // Start device
    if ( !m_pDevice->start( &winInfo ) ){
        return false;
    }

    //createDefaultRenderFragment();

        //// Create a render target view
        //ID3D11Texture2D* pBackBuffer = NULL;
        //HRESULT hr = gDXSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
        //if( FAILED( hr ) )
        //    return;

        //hr = gDXDevice->CreateRenderTargetView( pBackBuffer, NULL, &gDXRenderTargetView );
        //pBackBuffer->Release();
        //if( FAILED( hr ) )
        //    return;




    return true;

}

ZH::Graphics::RenderFragment* Global::createDefaultRenderFragment( ZH::Widgets::WindowsInfo& winInfo )
{
    if ( m_pRenderFragment ){
        return m_pRenderFragment;
    }

    // Default camera
    ZH::Math::float3 pos(0.0f,0.0f, -10.0f);
    ZH::Math::float3 look(0.0f,0.0f,1.0f);
    ZH::Math::float3 up(0.0f,1.0f,0.0f);
    float fovy = ZH::Math::PI/3.0f;

    float aspect = (float)winInfo.fWidth/winInfo.fHeight;
    ZH::Graphics::CameraPersp tmpCam( pos, look, up, fovy, aspect, 0.1f, 5000.0f, "DefaultPersp" );
    ZH::Graphics::CameraPersp* defaultCamera =
        ZH::Graphics::ResourceManager::instance().CameraPersps().acquire( tmpCam );

    // Default world
    ZH::Graphics::World& defaultWorld = ZH::Graphics::World::instance();

    // Render target
    std::vector<ZH::Graphics::RenderTarget*>* renderTargets = 
        new std::vector<ZH::Graphics::RenderTarget*>();

    ZH::Graphics::Texture2D* backBuffer =
        ZH::Graphics::ResourceManager::instance().acquireBackBuffer(m_pDevice);

    ZH::Graphics::RenderTarget* renderTarget =
        ZH::Graphics::ResourceManager::instance().acquireRenderTarget( m_pDevice, backBuffer, "defaultRenderTarget");

    renderTargets->push_back(renderTarget);


    m_pRenderFragment = new ZH::Graphics::RenderFragment( m_pDevice, defaultCamera, &defaultWorld, renderTargets);

    return m_pRenderFragment;
}



