#ifndef RENDERER_H
#define RENDERER_H
#include "Common/ZHSTD.h"
#include "Graphics/RenderFragment.h"



namespace ZH
{
    // Forwards
    namespace Graphics{
        class Device;
    }
    namespace Widgets{
        class WindowsInfo;
    }

    namespace Bridge{

        class ZH_BRIDGE_DLL Renderer
        {
        public:
            static Renderer& instance() { static Renderer r; return r;}

            // Device
            bool startEngine( ZH::Widgets::WindowsInfo& );
            bool shutdownEngine();

            ZH::Graphics::RenderFragment* defaultRenderFragment();

            void doRender();
        private:
            ZH::Graphics::Device* m_pDevice;
            ZH::Graphics::RenderFragment* m_pDefaultRenderFrag;

        private:
            Renderer();
            ~Renderer();
            Renderer( const Renderer& ){}
            Renderer& operator = ( const Renderer& ) { return *this; }
        };

    }
}

#endif
