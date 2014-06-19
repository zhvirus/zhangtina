#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/GeometryInstance.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        GeometryInstance::GeometryInstance():
            m_pIndexBuffer(NULL),
            m_primitiveType(PRIMITIVE_TOPOLOGY_TRIANGLELIST)
        {

        }

        GeometryInstance::~GeometryInstance()
        {
        }

        bool GeometryInstance::isValid()
        {
            bool valid = m_pIndexBuffer && m_pIndexBuffer->isValid();
            valid = valid && (m_vertexBufferArr.size() > 0);
            return valid;
        }

    }
}
