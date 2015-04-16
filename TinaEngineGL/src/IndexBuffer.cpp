#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/IndexBuffer.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/Debug.h"

namespace ZH{
    namespace Graphics{

        IndexBuffer::IndexBuffer( Context* ctx ):
            Buffer(ctx)
        {}

        IndexBuffer::~IndexBuffer()
        {}

    }

}