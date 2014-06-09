#ifndef DEVICEDX11_H
#define DEVICEDX11_H

#include "Common/ZHSTD.h"
#include "Graphics/Device.h"

namespace ZH{
    namespace Graphics{

        class Texture2D;
        class RenderTarget;

        class ZH_GRAPHICS_DLL DeviceDX11 : public Device
        {
        public:
            static DeviceDX11* instance(){static DeviceDX11 d; return &d;}
        public:
            virtual bool start( ZH::Widgets::WindowsInfo* );
            virtual bool shutdown();
            virtual bool setRenderTarget( RenderTarget* );
            virtual bool clearRenderTargetView( RenderTarget*, const ZH::Math::float4& );
            virtual bool present();
        private:
            virtual RenderTarget* createRenderTarget( const char* const, Texture2D* );
            virtual VertexBuffer* createVertexBuffer( const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&);
            virtual IndexBuffer*  createIndexBuffer ( const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&);
            virtual bool getBackBuffer( Texture2D*& );
        private:
            void* m_pImp;
        private:
            CLASS_COMMON_PROTECTED_DECLEARATION(DeviceDX11)

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

    }
}

#endif

