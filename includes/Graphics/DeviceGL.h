#ifndef DEVICEGL_H
#define DEVICEGL_H

#include "Common/ZHSTD.h"
#include "Graphics/Device.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL DeviceGL : public Device
        {
            CLASS_IDENTIFIER( E_CID_DEVICE_GL );
        public:
            static DeviceGL& instance(){static DeviceGL d; return d;}
        public:
            virtual bool start( ZH::Widgets::WindowsInfo* );
            virtual bool shutdown();
            virtual bool setRenderTarget( RenderTarget* );
            virtual bool clearRenderTargetView( RenderTarget*, const ZH::Math::float4& );
            virtual bool present();

            virtual bool createInputLayout( InputLayout&, const ShaderCodes* );
            virtual RenderTarget* createRenderTarget( const char* const, Texture2D* );
            virtual VertexBuffer* createVertexBuffer( SEMANTIC_TYPE, const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&);
            virtual IndexBuffer*  createIndexBuffer ( const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&);
            virtual bool getBackBuffer( Texture2D*& );
        private:

        private:
            CLASS_COMMON_PROTECTED_DECLEARATION(DeviceGL)
        };
    }
}
#endif
