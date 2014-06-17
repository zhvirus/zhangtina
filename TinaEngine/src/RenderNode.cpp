#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderNode.h"
#include "Internal/Graphics/Internal_common_graphics.h"


namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderNode )

        RenderNode::RenderNode( const char* const name ):
            Resource(name),
            m_pRenderItems(NULL)
        {
            m_pRenderItems = new RenderItemMap;
        }

        RenderNode::~RenderNode()
        {
            clear();
            delete m_pRenderItems;
        }

        bool RenderNode::isValid()
        {
            bool valid = true;

            return valid;
        }

        void RenderNode::clear()
        {
            assert( m_pRenderItems );
            if( !m_pRenderItems ){
                return;
            }

            RenderItem* pRenderItem = NULL;
            RenderItemMap::iterator  it = m_pRenderItems->begin();
            for(;it != m_pRenderItems->end(); ++it){
                pRenderItem = it->second;
                delete pRenderItem;
            }

            m_pRenderItems->clear();
        }

        bool RenderNode::removeRenderItem( const char* const name )
        {
            RenderItem* item =  findRenderItem( name );
            if ( !item ){
                return false;
            }

            if( m_renderItems.remove( item ) ){
                delete item;
                return true;
            }

            return false;
        }

        RenderItem* RenderNode::addRenderItem( const char* const name )
        {
            assert( name );
            if ( !name ){
                return NULL;
            }

            RenderItem* item = findRenderItem( name );
            if ( item ){
                ZH::Util::ENG_ERR("RenderNode::addRenderItem(), name '%s' already exists!\n", name);
                return NULL;
            }

            item = new RenderItem( name );
            m_renderItems.push_back( item );

            return item;
        }

        RenderItem* RenderNode::findRenderItem( const char* const name )
        {
            assert( name );
            if ( !name ){
                return NULL;
            }

            RenderItem* renderItem = NULL;
            const unsigned int size = m_renderItems.size();
            for( unsigned int i = 0; i<size; ++i ){
                renderItem = m_renderItems[i];
                if ( !renderItem )
                    continue;
                if ( strcmp( renderItem->name(), name) == 0 )
                    return renderItem;
            }

            return NULL;
        }

        void RenderNode::enableRenderItem( const char* const /*name*/ )
        {

        }

        void RenderNode::disableRenderItem( const char* const /*name*/ )
        {

        }
    }
}