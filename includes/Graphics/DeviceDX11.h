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
            CLASS_IDENTIFIER( E_CID_DEVICE_DX11 );
        public:
            static DeviceDX11* instance(){static DeviceDX11 d; return &d;}
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
            void* m_pImp;
        private:
            friend class AccessInternal;
            friend class ResourceManager;
            friend class StreamPool;
            CLASS_COMMON_PROTECTED_DECLEARATION(DeviceDX11)
        };

    }
}

#endif

