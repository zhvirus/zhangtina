#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderNodeGrid.h"
#include "Graphics/ResourceFactory.h"
#include "Internal/Graphics/Internal_common_graphics.h"



namespace ZH{
    namespace Graphics{

        const char* const RenderNodeGrid::m_sRenderNodeGridName = "Grid";


        RenderNodeGrid::RenderNodeGrid():
            RenderNode( m_sRenderNodeGridName )
        {
            prepareDefaultData();
        }

        RenderNodeGrid::~RenderNodeGrid()
        {
        }

        bool RenderNodeGrid::addRenderItem( RenderItem* )
        {
            // Build-in render node grid is not allowed to change render item
            assert( false );
            return true;
        }

        bool RenderNodeGrid::removeRenderItem( const char* const )
        {
            // Build-in render node grid is not allowed to change render item
            assert( false );
            return true;
        }

        bool RenderNodeGrid::prepareDefaultData()
        {
            ASSERT_NOT_NULL_RET_FALSE( m_pRenderItems );

            if ( m_pRenderItems->size() > 0 ){
                assert(false);
                return false;
            }

            RenderItem* pItem = ResourceFactory::createRenderItem(
                E_CID_RENDER_ITEM, "RenderItem_grid");

            ASSERT_NOT_NULL_RET_FALSE( pItem );

            bool result = RenderNode::addRenderItem( pItem );

            ASSERT_BOOL_RET_FALSE( result );






            return true;
        }


    }
}
