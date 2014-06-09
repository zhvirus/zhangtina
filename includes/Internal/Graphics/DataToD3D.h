#ifndef DATA_TO_D3D_H
#define DATA_TO_D3D_H

#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        // Forwards
        class BUFFER_DESC;
        class SUBRESOURCE_DATA;

        void toD3DBufferDesc( D3D11_BUFFER_DESC&, const BUFFER_DESC& );
        void toD3DSubResourceData( D3D11_SUBRESOURCE_DATA &, const SUBRESOURCE_DATA& );

    }
}



#endif
