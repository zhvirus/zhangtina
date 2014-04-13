#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Resource.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{
        Resource::Resource()
        {
        }

        Resource::~Resource()
        {
        }

        Resource::Resource( const Resource& v):
            Name(v)
        {
        }
    }
}
