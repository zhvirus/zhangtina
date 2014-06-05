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

    // Default render target
    std::vector<ZH::Graphics::RenderTarget*>* renderTargets = 
        new std::vector<ZH::Graphics::RenderTarget*>();

    ZH::Graphics::Texture2D* backBuffer =
        ZH::Graphics::ResourceManager::instance().acquireBackBuffer(m_pDevice);

    ZH::Graphics::RenderTarget* renderTarget =
        ZH::Graphics::ResourceManager::instance().acquireRenderTarget(
        ZH::Graphics::RenderTarget::m_sDefaultRenderTargetName, m_pDevice, backBuffer );

    renderTargets->push_back(renderTarget);

    // Default render fragment
    m_pRenderFragment = ZH::Graphics::ResourceManager::instance().acquireRenderFragment(
        ZH::Graphics::RenderFragment::m_sDefaultRenderFragmentName, m_pDevice, pDefaultCamera, &defaultWorld, renderTargets);

    return m_pRenderFragment;
}

void Global::applyPrefToRender()
{
    if ( m_pRenderFragment ){
        // Flush clear color
        m_pRenderFragment->clearColor( m_preference.clearColor() );
    }
}



