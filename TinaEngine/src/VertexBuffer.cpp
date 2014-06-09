#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/VertexBuffer.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( VertexBuffer )

        VertexBuffer::VertexBuffer( const char* const name ):
            Resource( name )
        {

        }

        VertexBuffer::~VertexBuffer()
        {

        }

        bool VertexBuffer::isValid()
        {
            return true;
        }

        bool VertexBuffer::operator== ( const VertexBuffer& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }

    }
}
