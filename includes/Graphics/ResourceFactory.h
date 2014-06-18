#ifndef RESOURCE_FACTORY_H
#define RESOURCE_FACTORY_H

#include "Graphics/RenderTarget.h"
#include "Graphics/ClassIdentifier.h"

namespace ZH{
    namespace Math{
        class float3;
    }

    namespace Graphics{

        // forwards
        class CameraPersp;
        class RenderFragment;
        class Device;
        class Camera;
        class World;
        class RenderTarget;
        class VertexBuffer;
        class RenderNode;
        class RenderItem;
        class EffectInstance;

        class ZH_GRAPHICS_DLL ResourceFactory
        {
        public:
            // Create CameraPersp
            static CameraPersp* createCameraPersp(
                const char* const name,
                const ZH::Math::float3& pos,
                const ZH::Math::float3& lookDir,
                const ZH::Math::float3& upDir,
                float fovy,
                float aspect,
                float nearZ,
                float farZ
                );

            // Create RenderFragment
            static RenderFragment* createRenderFragment(
                  const char* const name,
                  Device* device,
                  Camera* camera,
                  World* world,
                  const ZH::Graphics::RenderTargetPtrArray&
                );

            // Create RenderNode
            static RenderNode* createRenderNode(
                E_CLASS_ID c_id,
                const char* const name
                );

            // Create RenderItem
            static RenderItem* createRenderItem(
                E_CLASS_ID c_id,
                const char* const name
                );

            // Create EffectInstance
            static EffectInstance* createEffectInstance(
                E_CLASS_ID c_id,
                const char* const name
                );
        };
    }
}

#endif
