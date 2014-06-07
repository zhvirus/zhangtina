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
            unsigned int numOfNodes()const;
        private:
            World( const char* const name );

            friend class ResourceManager;

            CLASS_TYPE_NAME_DECLEARATION
        };


        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<World*>;
        typedef ZH::UTIL::Array<World*> WorldPtrArray;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<World>;
        typedef ZH::UTIL::Cache<World> WorldCache;

    } // namespace Graphics
} // namespace ZH

#endif