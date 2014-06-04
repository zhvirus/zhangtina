#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Graphics/Caches.h"

namespace ZH {
    namespace Graphics{

        class RenderTarget;
        class Texture2D;
        class Device;

        class ZH_GRAPHICS_DLL ResourceManager
        {
        public:
            static ResourceManager& instance() { static ResourceManager rm; return rm; }

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
                  std::vector<RenderTarget*>* renderTargets
                 );

        private:
            Cache<Texture2D>        m_texture2DCache;
            Cache<VertexBuffer>     m_vertexBufferCache;
            Cache<IndexBuffer>      m_indexBufferCache;
            Cache<EffectInstance>   m_effectInstanceCache;
            Cache<RenderTarget>     m_renderTargetCache;
            Cache<RenderFragment>   m_renderFragmentCache;
            Cache<CameraOrtho>      m_cameraOrthoCache;
            Cache<CameraPersp>      m_cameraPerspCache;

            // Put at last line
            CLASS_COMMON_PROTECTED_DECLEARATION(ResourceManager)
        };
    } // namespace Graphics
} // namespace ZH

#endif
