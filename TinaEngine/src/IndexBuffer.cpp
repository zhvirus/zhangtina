#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/IndexBuffer.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/IndexBufferImp.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( IndexBuffer )

        IndexBuffer::IndexBuffer( const char* const name, IndexBufferImp* imp, const BUFFER_DESC& desc ):
            Resource( name ),
            m_pImp( imp ),
            m_desc(desc)
        {
        }

        IndexBuffer::~IndexBuffer()
        {
            delete m_pImp;
        }

        bool IndexBuffer::isValid()
        {
            return m_pImp && m_pImp->isValid();
        }

        bool IndexBuffer::operator== ( const IndexBuffer& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }
    }
}
