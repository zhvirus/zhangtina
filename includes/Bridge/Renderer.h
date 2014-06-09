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
            bool startDevice( ZH::Widgets::WindowsInfo& );

            ZH::Graphics::RenderFragment* defaultRenderFragment();
            ZH::Graphics::RenderFragment* createDefaultRenderFragment( ZH::Widgets::WindowsInfo& );

            void doRender();
        private:
            ZH::Graphics::Device* m_pDevice;
            ZH::Graphics::RenderFragmentPtrArray m_renderFragArray;

        private:
            Renderer();
            ~Renderer();
            Renderer( const Renderer& ){}
            Renderer& operator = ( const Renderer& ) { return *this; }
        };

    }
}

#endif
