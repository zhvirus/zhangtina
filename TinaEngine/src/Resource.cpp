#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Resource.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        Resource::Resource( const char* const name ):
            Name( name )
        {
        }

        Resource::~Resource()
        {
        }
    }
}
