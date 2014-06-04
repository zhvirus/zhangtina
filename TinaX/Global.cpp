#include "stdafx.h"
#include "Global.h"
#include <stdarg.h>



#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"


Global::Global():
    m_pDevice( NULL ),
    m_pRenderFragment( NULL )
{
}


Global::~Global()
{
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

    return true;
}

ZH::Graphics::RenderFragment* Global::createDefaultRenderFragment( ZH::Widgets::WindowsInfo& winInfo )
{
    if ( m_pRenderFragment ){
        return m_pRenderFragment;
    }

    // Default camera
    ZH::Graphics::CameraPersp* pDefaultCamera = ZH::Graphics::ResourceManager::instance().acquireDefaultCameraPersp();
    if ( pDefaultCamera ){
        float aspect = (float)winInfo.fWidth/winInfo.fHeight;
        pDefaultCamera->aspect( aspect );
    }

    // Default world
    ZH::Graphics::World& defaultWorld = ZH::Graphics::World::instance();

    // Render target
    std::vector<ZH::Graphics::RenderTarget*>* renderTargets = 
        new std::vector<ZH::Graphics::RenderTarget*>();

    ZH::Graphics::Texture2D* backBuffer =
        ZH::Graphics::ResourceManager::instance().acquireBackBuffer(m_pDevice);

    ZH::Graphics::RenderTarget* renderTarget =
        ZH::Graphics::ResourceManager::instance().acquireRenderTarget( "DefaultRenderTarget", m_pDevice, backBuffer );

    renderTargets->push_back(renderTarget);

    m_pRenderFragment = ZH::Graphics::ResourceManager::instance().acquireRenderFragment(
        "DefaultRenderFragment", m_pDevice, pDefaultCamera, &defaultWorld, renderTargets);

    // clear color
    m_pRenderFragment->clearColor( 0.25f, 0.5f, 0.5f );

    return m_pRenderFragment;
}

void Global::applyPrefToRender()
{
    if ( m_pRenderFragment ){
        // Flush clear color
        m_pRenderFragment->clearColor( m_preference.clearColor() );
    }
}



