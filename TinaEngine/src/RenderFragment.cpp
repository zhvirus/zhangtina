#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderFragment.h"
#include "Internal/Graphics/Internal_common_graphics.h"

#include "Graphics/Camera.h"
#include "Graphics/World.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DeviceDX11.h"
#include "Math/float4.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderFragment )

        const char RenderFragment::m_sDefaultRenderFragmentName[] = "DefaultRenderFragment";

        RenderFragment::RenderFragment( const char* const name ):
            Resource( name ),
            m_devicePtr(NULL),
            m_cameraPtr(NULL),
            m_worldPtr(NULL),
            m_clearCol(0.0f,0.0f,0.0f,1.0f)
        {
        }

        RenderFragment::RenderFragment(  const char* const name, Device* device, Camera* cam, World* wld, const ZH::Graphics::RenderTargetPtrArray& rts ):
            Resource( name ),
            m_devicePtr( device ),
            m_cameraPtr(cam),
            m_worldPtr(wld),
            m_renderTargetArray(rts),
            m_clearCol(0.0f,0.0f,0.0f,1.0f)
        {
        }

        RenderFragment::~RenderFragment()
        {
        }

        RenderTarget* RenderFragment::getRenderTarget( unsigned int idx )
        {
            assert( idx < m_renderTargetArray.size() );
            if ( idx < m_renderTargetArray.size() ){
                return m_renderTargetArray[idx];
            }else{
                return NULL;
            }
        }

        void RenderFragment::pushRenderTarget( RenderTarget* rt )
        {
            assert(rt);
            if ( rt ){
                m_renderTargetArray.push_back( rt );
            }
        }

        void RenderFragment::render()
        {
            if ( m_renderTargetArray.size() == 0 ){
                return;
            }

            RenderTarget* pRenderTarget = m_renderTargetArray[0];
            if ( !pRenderTarget ){
                return;
            }

            // Set render target
            m_devicePtr->setRenderTarget( pRenderTarget );

            // Clear colors
            m_devicePtr->clearRenderTargetView( pRenderTarget, m_clearCol);


            // Present
            m_devicePtr->present();
        }

        bool RenderFragment::operator== ( const RenderFragment& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }

    }
}



