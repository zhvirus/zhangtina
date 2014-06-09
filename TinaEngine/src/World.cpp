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

        unsigned int World::numOfNodes()const{
            return 0;
        }

        //------------------------------------------------
        //
        //        SimpleWorld
        //
        //------------------------------------------------

        SimpleWorld::SimpleWorld( const char* const name):
            World(name)
        {

        }

        SimpleWorld::~SimpleWorld()
        {
        }

    }
}
