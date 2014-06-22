#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderNodeGrid.h"
#include "Graphics/ResourceFactory.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/EffectSolid.h"
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

            const char* const renderItemName = "RenderItem_grid";
            const char* const effectInstName = "effectInstSolid_grid";
            const char* const pos_VBName     = "POS_VB_grid";
            const char* const IBName         = "IB_grid";

            // Create render item
            RenderItem* pItem = ResourceFactory::createRenderItem(
                E_CID_RENDER_ITEM, renderItemName);

            ASSERT_NOT_NULL_RET_FALSE( pItem );

            // Add render item
            bool result = RenderNode::addRenderItem( pItem );
            ASSERT_BOOL_RET_FALSE( result );

            // Acquire effect instance
            EffectInstance* pEffectInstance = ResourceManager::instance().acquireEffectInstance(
                E_CID_EFFECT_INSTANCE_SOLID,
                effectInstName);
            ASSERT_NOT_NULL_RET_FALSE( pEffectInstance );

            // Set effect instance
            pItem->effectInst( pEffectInstance );

            // Vertex buffer
            float vertices[] =
            {
                -0.5f, 0.0f,  0.5f,
                 0.5f, 0.0f,  0.5f,
                 0.5f, 0.0f, -0.5f,
                -0.5f, 0.0f, -0.5f
            };
            BUFFER_DESC buff_desc_vb;
            buff_desc_vb.buffSizeInByte = sizeof(float)*3*4;
            buff_desc_vb.usage = USAGE_IMMUTABLE;
            buff_desc_vb.bind_flag = BIND_VERTEX_BUFFER;
            buff_desc_vb.cpu_access_flag = CPU_ACCESS_NO;
            buff_desc_vb.misc_flag = RESOURCE_MISC_UNUSED;

            SUBRESOURCE_DATA sub_res_data_vb;
            sub_res_data_vb.pSysMem = (void*)vertices;

            if ( !ResourceManager::instance().acquireVertexBuffer(
                pos_VBName,
                buff_desc_vb,
                sub_res_data_vb))
            {
                ZH::Util::ENG_ERR("Create VB '%s' failed!\n", pos_VBName );
                return false;
            }

            // Index buffer
            unsigned int indices[] =
            {
                0,1,
                1,2,
                2,3,
                3,1
            };
            BUFFER_DESC buff_desc_ib;
            buff_desc_ib.buffSizeInByte = sizeof(unsigned int)*2*4;
            buff_desc_ib.usage = USAGE_IMMUTABLE;
            buff_desc_ib.bind_flag = BIND_INDEX_BUFFER;
            buff_desc_ib.cpu_access_flag = CPU_ACCESS_NO;
            buff_desc_ib.misc_flag = RESOURCE_MISC_UNUSED;

            SUBRESOURCE_DATA sub_res_data_ib;
            sub_res_data_ib.pSysMem = (void*)indices;

            if( !ResourceManager::instance().acquireIndexBuffer(
                IBName,
                buff_desc_ib,
                sub_res_data_ib))
            {
                ZH::Util::ENG_ERR("Create IB '%s' failed!\n", IBName );
                return false;
            }

            // Topo
            pItem->primitiveType( PRIMITIVE_TOPOLOGY_LINELIST );

            // Update streams
            pItem->updateStreams();

            return true;
        }


    }
}
