#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/VertexBuffer.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/Debug.h"

namespace ZH{
    namespace Graphics{

        VertexBuffer::VertexBuffer(Context* ctx) :
            Buffer(ctx)
        {}

        VertexBuffer::~VertexBuffer()
        {}

    }

}