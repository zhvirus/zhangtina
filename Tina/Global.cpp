#include "stdafx.h"
#include "Global.h"
#include <stdarg.h>



#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"

#define MAX_PRINT_BUF_SIZE 0x1000

Global::Global():
    m_pDevice( NULL )
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



