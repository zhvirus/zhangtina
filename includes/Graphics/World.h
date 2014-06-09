#ifndef WORLD_H
#define WORLD_H

#include "Common/ZHSTD.h"
#include "Graphics/Name.h"
#include "Util/Array.h"
#include "Util/Cache.h"


namespace ZH {
    namespace Graphics{

        class ZH_GRAPHICS_DLL World : public Name
        {
        public:
            virtual ~World();
        public:
            virtual unsigned int numOfNodes()const;
        protected:
            World( const char* const name );

            friend class ResourceManager;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        // Simple world
        class ZH_GRAPHICS_DLL SimpleWorld : public World
        {
        public:
            SimpleWorld( const char* const name );
            virtual ~SimpleWorld();


        private:
        private:
            friend class ResourceManager;
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };


        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<World*>;
        typedef ZH::UTIL::Array<World*> WorldPtrArray;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<World>;
        typedef ZH::UTIL::Cache<World> WorldCache;

    } // namespace Graphics
} // namespace ZH

#endif