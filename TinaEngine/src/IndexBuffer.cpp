#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/IndexBuffer.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( IndexBuffer )

        IndexBuffer::IndexBuffer( const char* const name ):
            Resource( name )
        {

        }

        IndexBuffer::~IndexBuffer()
        {

        }

        bool IndexBuffer::isValid()
        {
            return true;
        }

        bool IndexBuffer::operator== ( const IndexBuffer& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }
    }
}
