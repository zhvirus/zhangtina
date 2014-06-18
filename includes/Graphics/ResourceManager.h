#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Graphics/Texture2D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Effect.h"
#include "Graphics/World.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderFragment.h"
#include "Graphics/World.h"

namespace ZH {
    namespace Graphics{

        class Device;


        class ZH_GRAPHICS_DLL ResourceManager
        {
        public:
            static ResourceManager& instance() { static ResourceManager rm; return rm; }

            void initialize();
            void deinitialize();

            // World
            SimpleWorld* findSimpleWorldByName( const char* const name );
            SimpleWorld* acquireSimpleWorld( const char* const name );

            // Render target
            RenderTarget* findRenderTargetByName( const char* const );
            RenderTarget* acquireRenderTarget( const char* const, Device*, Texture2D* );

            // Back buffer
            Texture2D*    acquireBackBuffer( Device* );

            // Camera
            CameraPersp* findCameraPerspByName( const char* const name );
            CameraPersp* acquireCameraPersp(
                const char* const name,
                const ZH::Math::float3& pos,
                const ZH::Math::float3& lookDir,
                const ZH::Math::float3& upDir,
                float fovy,
                float aspect,
                float nearZ,
                float farZ);

            // Render fragment
            RenderFragment* findRenderFragmentByName( const char* const name );
            RenderFragment* acquireRenderFragment(
                  const char* const name,
                  Device* device,
                  Camera* camera,
                  World* world,
                  const ZH::Graphics::RenderTargetPtrArray& rts
                 );

            // Vertex buffer
            VertexBuffer* findVertexBufferByName( const char* const name );
            VertexBuffer* acquireVertexBuffer(
                const char* const name,
                Device* device,
                const BUFFER_DESC&,
                const SUBRESOURCE_DATA&
                );

            // Index buffer
            IndexBuffer* findIndexBufferByName( const char* const name );
            IndexBuffer* acquireIndexBuffer(
                const char* const name,
                Device* device,
                const BUFFER_DESC&,
                const SUBRESOURCE_DATA&
                );

            // Effect
            Effect* findEffect(
                E_CLASS_ID c_id,
                const char* const name
                );

            // Effect instance
            EffectInstance* findEffectInstanceByName( const char* const name );
            EffectInstance* acquireEffectInstance(
                E_CLASS_ID c_id,
                const char* const name
                );

        private:
            // Shader
            bool buildShaders();
            void clearShaders();

            bool createDefaultWorld();
            bool createDefaultCameras();
            bool createDefaultRenderTargets();
            bool createDefaultRenderFragment();


        private:
            Texture2DCache        m_texture2DCache;
            VertexBufferCache     m_vertexBufferCache;
            IndexBufferCache      m_indexBufferCache;
            EffectCache           m_effectCache;
            EffectInstanceCache   m_effectInstanceCache;
            RenderTargetCache     m_renderTargetCache;
            RenderFragmentCache   m_renderFragmentCache;
            CameraCache           m_cameraCache;
            WorldCache            m_worldCache;

            bool m_bInitialized;

            // Put at last line
            CLASS_COMMON_PROTECTED_DECLEARATION(ResourceManager)
        };
    } // namespace Graphics
} // namespace ZH

#endif
