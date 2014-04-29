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

            Cache<Texture2D>&      Texture2Ds()      { return m_texture2DCache; }
            Cache<VertexBuffer>&   VertexBuffers()   { return m_vertexBufferCache; }
            Cache<IndexBuffer>&    IndexBuffers()    { return m_indexBufferCache; }
            Cache<EffectInstance>& EffectInstances() { return m_effectInstanceCache; }
            Cache<RenderTarget>&   RenderTargets()   { return m_renderTargetCache; }
            Cache<RenderFragment>& RenderFragments() { return m_renderFragmentCache; }
            Cache<CameraOrtho>&    CameraOrthos()    { return m_cameraOrthoCache; }
            Cache<CameraPersp>&    CameraPersps()    { return m_cameraPerspCache; }

            RenderTarget* acquireRenderTarget( Device*, Texture2D*, const char* const);
            Texture2D*    acquireBackBuffer( Device* );

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
