#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/GeometryInstance.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( GeometryInstance )

        GeometryInstance::GeometryInstance():
            m_pIndexBuffer(NULL)
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
