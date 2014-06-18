#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/VertexBuffer.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/VertexBufferImp.h"

namespace ZH{
    namespace Graphics{

        VertexBuffer::VertexBuffer( const char* const name, VertexBufferImp* imp, const BUFFER_DESC& desc ):
            Resource( name ),
            m_pImp( imp ),
            m_desc(desc)
        {
        }

        VertexBuffer::~VertexBuffer()
        {
            delete m_pImp;
        }

        bool VertexBuffer::isValid()
        {
            return m_pImp && m_pImp->isValid();
        }

        bool VertexBuffer::operator== ( const VertexBuffer& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }

    }
}
