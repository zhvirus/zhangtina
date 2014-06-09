#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Resource.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        BUFFER_DESC::BUFFER_DESC( const BUFFER_DESC& desc ):
            widthInByte( desc.widthInByte ),
            usage( desc.usage ),
            bind_flag( desc.bind_flag ),
            cpu_access_flag( desc.cpu_access_flag ),
            misc_flag( desc.misc_flag )
        {
        }



        Resource::Resource( const char* const name ):
            Name( name )
        {
        }

        Resource::~Resource()
        {
        }
    }
}
