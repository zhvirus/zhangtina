#ifndef RENDER_FRAGMENT_H
#define RENDER_FRAGMENT_H

#include <vector>
#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"


namespace ZH{
    namespace Graphics{

        // forwards
        class Camera;
        class World;
        class RenderTarget;

        class ZH_GRAPHICS_DLL RenderFragment : public Resource {
        public:
            RenderFragment();
            RenderFragment( Camera*, World*, std::vector<RenderTarget*>* );
            ~RenderFragment();

            Camera* camera() { return m_cameraPtr; }
            void camera( Camera* cam ){ m_cameraPtr = cam; }

            World* world() { return m_worldPtr; }
            void world( World* wld ){ m_worldPtr = wld; }

            void setRenderTarget( RenderTarget*, unsigned int );
            RenderTarget* getRenderTarget( unsigned int );
            void pushRenderTarget( RenderTarget* );

            void render();

        protected:
            Camera* m_cameraPtr;
            World*  m_worldPtr;
            std::vector<RenderTarget*>* m_renderTargetsPtr;



            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };



    } // end namespace Graphics
} // end namespace ZH

#endif
