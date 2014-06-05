#ifndef RENDER_FRAGMENT_H
#define RENDER_FRAGMENT_H

#include <vector>
#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Math/float4.h"


namespace ZH{
    namespace Graphics{

        // forwards
        class Device;
        class Camera;
        class World;
        class RenderTarget;

        class ZH_GRAPHICS_DLL RenderFragment : public Resource {
        public:
            RenderFragment( const char* const );
            RenderFragment( const char* const, Device*, Camera*, World*, std::vector<RenderTarget*>* );
            virtual ~RenderFragment();

            bool operator ==(const RenderFragment&);

            // Camera
            Camera* camera() { return m_cameraPtr; }
            void camera( Camera* cam ){ m_cameraPtr = cam; }

            // World
            World* world() { return m_worldPtr; }
            void world( World* wld ){ m_worldPtr = wld; }

            // Render targets
            void setRenderTarget( RenderTarget*, unsigned int );
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
            std::vector<RenderTarget*>* m_renderTargetsPtr;
            ZH::Math::float4 m_clearCol;


            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };



    } // end namespace Graphics
} // end namespace ZH

#endif
