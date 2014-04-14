#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderFragment.h"
#include "Internal/Common/internal_common.h"
#include "Graphics/Camera.h"
#include "Graphics/World.h"
#include "Graphics/RenderTarget.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderFragment )

        RenderFragment::RenderFragment():
        m_cameraPtr(NULL),
        m_worldPtr(NULL),
        m_renderTargetsPtr(NULL)
        {
            m_renderTargetsPtr = new std::vector<RenderTarget*>;
        }

        RenderFragment::RenderFragment( Camera* cam, World* wld, std::vector<RenderTarget*>* rts):
        m_cameraPtr(cam),
        m_worldPtr(wld),
        m_renderTargetsPtr(rts)
        {
            m_renderTargetsPtr = new std::vector<RenderTarget*>;
        }

        RenderFragment::~RenderFragment()
        {
            if( m_renderTargetsPtr ){
                delete m_renderTargetsPtr;
                m_renderTargetsPtr = NULL;
            }
        }

        void RenderFragment::setRenderTarget( RenderTarget* rt, unsigned int idx )
        {
            if ( m_renderTargetsPtr->size() >idx ){
                (*m_renderTargetsPtr)[idx] = rt;
            }
        }

        RenderTarget* RenderFragment::getRenderTarget( unsigned int idx )
        {
            if ( m_renderTargetsPtr->size() >idx ){
                return (*m_renderTargetsPtr)[idx];
            }

            return NULL;
        }

        void RenderFragment::pushRenderTarget( RenderTarget* rt )
        {
            if ( rt ){
                m_renderTargetsPtr->push_back( rt );
            }
        }

        void RenderFragment::render()
        {

        }

        bool RenderFragment::operator== ( const RenderFragment& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }

    }
}



