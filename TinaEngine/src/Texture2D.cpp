#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Texture2D.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Texture2D, "Texture2D" )

        Texture2D::Texture2D()
        {

        }

        Texture2D::~Texture2D()
        {

        }

        bool Texture2D::isValid()
        {
            return true;
        }


    }
}
