#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Internal/Graphics/ResourceFactory.h"
#include "Internal/Graphics/Internal_common_graphics.h"

#include "Graphics/Camera.h"
#include "Graphics/RenderFragment.h"
#include "Graphics/VertexBuffer.h"
#include "Math/float3.h"


namespace ZH{
    namespace Graphics{

        CameraPersp* ResourceFactory::createCameraPersp(
                const char* const name,
                const ZH::Math::float3& pos,
                const ZH::Math::float3& lookDir,
                const ZH::Math::float3& upDir,
                float fovy,
                float aspect,
                float nearZ,
                float farZ)
        {
            assert( name );
            CameraPersp* pCam = new CameraPersp(
                name, pos, lookDir, upDir, fovy, aspect, nearZ, farZ);
            return pCam;
        }

        RenderFragment* ResourceFactory::createRenderFragment(
                  const char* const name,
                  Device* device,
                  Camera* camera,
                  World* world,
                  const ZH::Graphics::RenderTargetPtrArray& rts)
        {
            // Check name
            assert( name );
            if ( !name || ( strcmp( name, "")==0 ) ){
                return NULL;
            }

            // Check device
            assert( device );
            if ( !device ){
                return NULL;
            }

            // Check camera
            assert( camera );
            if ( !camera ){
                return NULL;
            }

            // Check world
            assert( world );
            if ( !world ){
                return NULL;
            }

            // Check render targets
            if ( rts.size() == 0 ){
                return NULL;
            }

            RenderFragment* pObj = new ZH::Graphics::RenderFragment(
                name, device, camera, world, rts);

            return pObj;
        }

    }
}

