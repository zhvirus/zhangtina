#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/World.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        CLASS_TYPE_NAME_DEFINITION( World )
        CLASS_TYPE_NAME_DEFINITION( SimpleWorld )

        //------------------------------------------------
        //
        //        World
        //
        //------------------------------------------------
        World::World( const char* const name ):
            Name( name )
        {

        }

        World::~World()
        {
        }



        //------------------------------------------------
        //
        //        SimpleWorld
        //
        //------------------------------------------------
        const char* const SimpleWorld::m_sDefaultSimpleWorldName = "DefaultSimpleWolrd";

        SimpleWorld::SimpleWorld( const char* const name):
            World(name),
            m_pRenderNodes(NULL)
        {
            m_pRenderNodes = new RenderNodeMap;
        }

        SimpleWorld::~SimpleWorld()
        {
            clear();
            delete m_pRenderNodes;
        }

        void SimpleWorld::clear()
        {
            assert( m_pRenderNodes );
            if( !m_pRenderNodes ){
                return;
            }

            RenderNode* pRenderNode = NULL;
            RenderNodeMap::iterator  it = m_pRenderNodes->begin();
            for(;it != m_pRenderNodes->end(); ++it){
                pRenderNode = it->second;
                delete pRenderNode;
            }

            m_pRenderNodes->clear();
        }

        unsigned int SimpleWorld::numOfNodes()const
        {
            return m_pRenderNodes?(unsigned int)m_pRenderNodes->size():0;
        }

        RenderNode* SimpleWorld::addRenderNode( const char* const name )
        {
            assert( name );
            if( !name ){
                return NULL;
            }

            if ( strcmp(name, "") == 0 ){
                return NULL;
            }

            RenderNode* pRenderNode = findRenderNode( name );
            if ( pRenderNode ){
                assert( !pRenderNode );
                return NULL;
            }

            pRenderNode = new RenderNode( name );
            m_pRenderNodes->insert( std::make_pair(name, pRenderNode) );

            return pRenderNode;
        }

        RenderNode* SimpleWorld::findRenderNode( const char* const name )
        {
            assert( name );
            if( !name ){
                return NULL;
            }

            if ( strcmp(name, "") == 0 ){
                return NULL;
            }

            if ( !m_pRenderNodes ){
                return NULL;
            }

            RenderNodeMap::iterator  it = m_pRenderNodes->find( name );
            if ( it != m_pRenderNodes->end() ){
                return it->second;
            }

            return NULL;
        }

        bool SimpleWorld::removeRenderNode( const char* const name )
        {
            assert( name );
            if( !name ){
                return false;
            }

            if ( strcmp(name, "") == 0 ){
                return false;
            }

            RenderNode* pRenderNode = NULL;
            RenderNodeMap::iterator  it = m_pRenderNodes->begin();
            for(;it != m_pRenderNodes->end(); ++it){
                pRenderNode = it->second;
                if ( !pRenderNode || !pRenderNode->nameEqual(name) ){
                    continue;
                }

                delete pRenderNode;
                it = m_pRenderNodes->erase( it );
                return true;
            }

            return false;
        }


    }
}
