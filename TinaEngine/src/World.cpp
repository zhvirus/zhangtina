#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/World.h"
#include "Internal/Graphics/WorldImp.h"

#define IMP_PTR ((ZH::Graphics::WorldImp*)m_pImp)

namespace ZH{
    namespace Graphics{

        CLASS_TYPE_NAME_DEFINITION( World )

        World::World():
            m_pImp(NULL)
        {
            m_pImp = new WorldImp();
        }

        World::~World()
        {
        }

        unsigned int World::numOfNodes()const{
            return IMP_PTR?IMP_PTR->numOfNodes():0;
        }

    }
}
