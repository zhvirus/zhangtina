#ifndef WORLD_H
#define WORLD_H


#include "Common/ZHSTD.h"
#include "Graphics/Name.h"
#include "Graphics/RenderNode.h"
#include "Util/Array.h"
#include "Util/Cache.h"



namespace ZH {
    namespace Graphics{

        class ZH_GRAPHICS_DLL World : public Name
        {
            CLASS_IDENTIFIER( E_CID_WORLD );
        public:
            virtual ~World();
        public:
            virtual unsigned int numOfNodes()const = 0;
            virtual void clear() = 0;
        protected:
            World( const char* const name );

            friend class ResourceManager;
        };

        // Simple world
        class ZH_GRAPHICS_DLL SimpleWorld : public World
        {
            CLASS_IDENTIFIER( E_CID_SIMPLE_WORLD );
        public:
            virtual ~SimpleWorld();
            virtual unsigned int numOfNodes()const;
            virtual void clear();
            bool        addRenderNode( RenderNode* );
            RenderNode* findRenderNode( const char* const );
            bool        removeRenderNode( const char* const );

            static const char* const m_sDefaultSimpleWorldName;
        private:
            RenderNodeMap* m_pRenderNodes;

        private:
            SimpleWorld( const char* const name );
            friend class ResourceManager;
        };


        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<World*>;
        typedef ZH::UTIL::Array<World*> WorldPtrArray;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<World>;
        typedef ZH::UTIL::Cache<World> WorldCache;

    } // namespace Graphics
} // namespace ZH

#endif