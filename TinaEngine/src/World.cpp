#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/World.h"

namespace ZH{
    namespace Graphics{

        CLASS_TYPE_NAME_DEFINITION( World )

        World::World( const char* const name ):
            Name( name )
        {

        }

        World::~World()
        {
        }

        unsigned int World::numOfNodes()const{
            return 0;
        }

    }
}
