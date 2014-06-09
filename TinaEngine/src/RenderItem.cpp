#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderItem.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderItem )

        RenderItem::RenderItem( const char* const name ):
            Resource(name),
            m_effectInst(NULL)
        {

        }

        RenderItem::~RenderItem()
        {
        }

        bool RenderItem::isValid()
        {
            bool valid = m_effectInst && m_effectInst->isValid() && m_geoInst.isValid();
            return valid;
        }

    }
}
