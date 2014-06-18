#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderNode.h"
#include "Internal/Graphics/Internal_common_graphics.h"


namespace ZH{
    namespace Graphics{

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

        bool RenderNode::addRenderItem( RenderItem* pItem )
        {
            assert( m_pRenderItems );
            if( !m_pRenderItems ){
                return false;
            }

            assert( pItem );
            if( !pItem ){
                return false;
            }

            if ( pItem->nameEqual("") ){
                return false;
            }

            RenderItem* pRenderItem = findRenderItem( pItem->name() );
            if ( pRenderItem ){
                assert( !pRenderItem );
                return false;
            }

            m_pRenderItems->insert( std::make_pair(pItem->name(), pRenderItem) );

            return true;
        }

        RenderItem* RenderNode::findRenderItem( const char* const name )
        {
            assert( name );
            if( !name ){
                return NULL;
            }

            if ( strcmp(name, "") == 0 ){
                return NULL;
            }

            if ( !m_pRenderItems ){
                return NULL;
            }

            RenderItemMap::iterator  it = m_pRenderItems->find( name );
            if ( it != m_pRenderItems->end() ){
                return it->second;
            }

            return NULL;
        }

        bool RenderNode::removeRenderItem( const char* const name )
        {
            assert( name );
            if( !name ){
                return false;
            }

            if ( strcmp(name, "") == 0 ){
                return false;
            }

            RenderItem* pRenderItem = NULL;
            RenderItemMap::iterator  it = m_pRenderItems->begin();
            for(;it != m_pRenderItems->end(); ++it){
                pRenderItem = it->second;
                if ( !pRenderItem || !pRenderItem->nameEqual(name) ){
                    continue;
                }

                delete pRenderItem;
                it = m_pRenderItems->erase( it );
                return true;
            }

            return false;
        }


        void RenderNode::enableRenderItem( const char* const /*name*/ )
        {

        }

        void RenderNode::disableRenderItem( const char* const /*name*/ )
        {

        }
    }
}