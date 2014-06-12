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

            // Shader
            bool buildShaders();
            void clearShaders();

            // World
            World* findWorldByName( const char* const name );
            World* acquireWorld( const char* const name );

            // Render target
            RenderTarget* findRenderTargetByName( const char* const );
            RenderTarget* acquireRenderTarget( const char* const, Device*, Texture2D* );

            // Back buffer
            Texture2D*    acquireBackBuffer( Device* );

            // Camera
            CameraPersp* findCameraPerspByName( const char* const name );
            CameraPersp* acquireDefaultCameraPersp();
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


        private:
            Texture2DCache        m_texture2DCache;
            VertexBufferCache     m_vertexBufferCache;
            IndexBufferCache      m_indexBufferCache;
            EffectInstanceCache   m_effectInstanceCache;
            RenderTargetCache     m_renderTargetCache;
            RenderFragmentCache   m_renderFragmentCache;
            CameraOrthoCache      m_cameraOrthoCache;
            CameraPerspCache      m_cameraPerspCache;
            WorldCache            m_worldCache;

            // Put at last line
            CLASS_COMMON_PROTECTED_DECLEARATION(ResourceManager)
        };
    } // namespace Graphics
} // namespace ZH

#endif
