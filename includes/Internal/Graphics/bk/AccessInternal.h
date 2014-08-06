#ifndef ACCESS_INTERNAL_H
#define ACCESS_INTERNAL_H

#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        class AccessInternal{
        public:
            static ID3D11Device* device();
            static ID3D11DeviceContext* context();
        };
    }
}

#endif
