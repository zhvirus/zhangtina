#ifndef RESOURCE_FACTORY_H
#define RESOURCE_FACTORY_H

#include <vector>

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

        class ResourceFactory
        {
        private:
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
                  std::vector<RenderTarget*>* renderTargets
                );




            friend class ResourceManager;
        };
    }
}

#endif
