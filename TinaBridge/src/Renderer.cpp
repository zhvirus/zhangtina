#ifndef _ZH_BRIDGE_DLL_
#define _ZH_BRIDGE_DLL_
#endif

#include <string>
#include "Bridge/Renderer.h"
#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"
#include "Graphics/EffectSolid.h"

namespace ZH
{
    namespace Bridge{

        Renderer::Renderer():
            m_pDevice(NULL)
        {
        }


        Renderer::~Renderer()
        {
            if ( m_pDevice ){
                m_pDevice->shutdown();
            }
        }

        bool Renderer::startDevice( ZH::Widgets::WindowsInfo& winInfo )
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

            // Test code
            ZH::Graphics::EffectSolid solid;

            return true;
        }

        ZH::Graphics::RenderFragment* Renderer::defaultRenderFragment()
        {
            for ( unsigned int i=0; i< m_renderFragArray.size(); ++i ){
                ZH::Graphics::RenderFragment* pRenderFrag = m_renderFragArray[i];
                if ( pRenderFrag &&
                    ( strcmp(pRenderFrag->name(), ZH::Graphics::RenderFragment::m_sDefaultRenderFragmentName)==0))
                {
                    return pRenderFrag;
                }
            }

            return NULL;
        }

        ZH::Graphics::RenderFragment* Renderer::createDefaultRenderFragment( ZH::Widgets::WindowsInfo& winInfo )
        {
            ZH::Graphics::RenderFragment* pDefaultRenderFragment = defaultRenderFragment();
            if ( pDefaultRenderFragment ){
                return pDefaultRenderFragment;
            }

            // Default camera
            ZH::Graphics::CameraPersp* pDefaultCamera = ZH::Graphics::ResourceManager::instance().acquireDefaultCameraPersp();
            if ( pDefaultCamera ){
                float aspect = (float)winInfo.fWidth/winInfo.fHeight;
                pDefaultCamera->aspect( aspect );
            }

            // Default world
            ZH::Graphics::World* defaultWorld = ZH::Graphics::ResourceManager::instance().acquireWorld("defaultWorld");

            // Default render target
            ZH::Graphics::RenderTargetPtrArray renderTargets;

            ZH::Graphics::Texture2D* backBuffer =
                ZH::Graphics::ResourceManager::instance().acquireBackBuffer(m_pDevice);

            ZH::Graphics::RenderTarget* renderTarget =
                ZH::Graphics::ResourceManager::instance().acquireRenderTarget(
                ZH::Graphics::RenderTarget::m_sDefaultRenderTargetName, m_pDevice, backBuffer );

            renderTargets.push_back(renderTarget);

            // Default render fragment
            pDefaultRenderFragment = ZH::Graphics::ResourceManager::instance().acquireRenderFragment(
                ZH::Graphics::RenderFragment::m_sDefaultRenderFragmentName, m_pDevice, pDefaultCamera, defaultWorld, renderTargets);

            // Save it in the array
            m_renderFragArray.push_back( pDefaultRenderFragment );

            return pDefaultRenderFragment;
        }


        void Renderer::doRender()
        {
            ZH::Graphics::RenderFragment* pDefaultRenderFrag = defaultRenderFragment();
            if ( pDefaultRenderFrag ){
                pDefaultRenderFrag->render();
            }
        }
    }

}
