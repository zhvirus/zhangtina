#ifndef RENDER_FRAGMENT_H
#define RENDER_FRAGMENT_H

#include <vector>
#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/RenderTarget.h"
#include "Math/float4.h"
#include "Util/Array.h"
#include "Util/Cache.h"


namespace ZH{
    namespace Graphics{

        // forwards
        class Device;
        class Camera;
        class World;

        class ZH_GRAPHICS_DLL RenderFragment : public Resource {
        public:
            RenderFragment( const char* const );
            RenderFragment( const char* const, Device*, Camera*, World*, const ZH::Graphics::RenderTargetPtrArray& );
            virtual ~RenderFragment();

            bool operator ==(const RenderFragment&);

            // Camera
            Camera* camera() { return m_cameraPtr; }
            void camera( Camera* cam ){ m_cameraPtr = cam; }

            // World
            World* world() { return m_worldPtr; }
            void world( World* wld ){ m_worldPtr = wld; }

            // Render targets
            RenderTarget* getRenderTarget( unsigned int );
            void pushRenderTarget( RenderTarget* );

            // Clear color
            void clearColor( const ZH::Math::float4& col ) { m_clearCol = col; }
            void clearColor( float r, float g, float b ) { m_clearCol.r=r; m_clearCol.g=g; m_clearCol.b=b; }

            void render();

            static const char m_sDefaultRenderFragmentName[30];
        protected:
            Device* m_devicePtr;
            Camera* m_cameraPtr;
            World*  m_worldPtr;
            ZH::Graphics::RenderTargetPtrArray m_renderTargetArray;
            ZH::Math::float4 m_clearCol;


            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderFragment*>;
        typedef ZH::UTIL::Array<RenderFragment*> RenderFragmentPtrArray;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<RenderFragment>;
        typedef ZH::UTIL::Cache<RenderFragment> RenderFragmentCache;


    } // end namespace Graphics
} // end namespace ZH

#endif
