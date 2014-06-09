#ifndef DEVICE_H
#define DEVICE_H

#include "Common/ZHSTD.h"
#include "Graphics/Name.h"

// Forwards
namespace ZH{
    namespace Widgets{
        class WindowsInfo;
    }
}

namespace ZH{
    namespace Math{
        class float4;
    }
    namespace Graphics{

        class Texture2D;
        class RenderTarget;
        class VertexBuffer;
        class IndexBuffer;
        class BUFFER_DESC;
        class SUBRESOURCE_DATA;


        enum DEVICE_STATUS{
            DEVICE_STATUS_RUNNING,
            DEVICE_STATUS_PAUSED,
            DEVICE_STATUS_SHUTDOWN
        };

        class ZH_GRAPHICS_DLL Device
        {
        public:
            virtual bool isRunning()const{return m_status == DEVICE_STATUS_RUNNING;};
            virtual bool isPaused()const{ return m_status == DEVICE_STATUS_PAUSED; }
            virtual bool isShutdown()const{ return m_status == DEVICE_STATUS_SHUTDOWN; }
            virtual bool start( ZH::Widgets::WindowsInfo* ) = 0;
            virtual bool shutdown() = 0;

            virtual bool setRenderTarget( RenderTarget* ) = 0;
            virtual bool clearRenderTargetView( RenderTarget*, const ZH::Math::float4& ) = 0;
            virtual bool present() = 0;

        private:
            virtual RenderTarget* createRenderTarget( const char* const, Texture2D* ) = 0;
            virtual VertexBuffer* createVertexBuffer( const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&) = 0;
            virtual IndexBuffer*  createIndexBuffer ( const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&) = 0;
            virtual bool getBackBuffer( Texture2D*& ) = 0;


        protected:
            Device();
            DEVICE_STATUS m_status;
            friend class ResourceManager;
        };

    }

}

#endif
