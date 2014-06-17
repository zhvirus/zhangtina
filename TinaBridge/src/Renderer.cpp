#ifndef _ZH_BRIDGE_DLL_
#define _ZH_BRIDGE_DLL_
#endif

#include <string>
#include "Bridge/Renderer.h"
#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"

namespace ZH
{
    namespace Bridge{

        Renderer::Renderer():
            m_pDevice(NULL),
            m_pDefaultRenderFrag(NULL)
        {
        }


        Renderer::~Renderer()
        {
            if ( m_pDevice ){
                m_pDevice->shutdown();
            }
        }

        bool Renderer::startEngine( ZH::Widgets::WindowsInfo& winInfo )
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

            // Initialize resource manager, like build shaders
            // create all default internal build-in resources
            ZH::Graphics::ResourceManager::instance().initialize();

            return true;
        }

        bool Renderer::shutdownEngine()
        {
            // Clear shaders
            ZH::Graphics::ResourceManager::instance().deinitialize();

            // Release resources


            // Shutdown device
            if( m_pDevice ){
                m_pDevice->shutdown();
                m_pDevice = NULL;
            }

            return true;
        }

        ZH::Graphics::RenderFragment* Renderer::defaultRenderFragment()
        {
            if ( !m_pDefaultRenderFrag ){
                m_pDefaultRenderFrag = ZH::Graphics::ResourceManager::instance().findRenderFragmentByName( ZH::Graphics::RenderFragment::m_sDefaultRenderFragmentName );
            }

            assert( m_pDefaultRenderFrag );
            return m_pDefaultRenderFrag;
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
