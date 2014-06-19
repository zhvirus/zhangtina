#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Internal/Graphics/DataToD3D.h"
#include "Graphics/Resource.h"

namespace ZH{
    namespace Graphics{

        void toD3DBufferDesc( D3D11_BUFFER_DESC& d3d_desc, const BUFFER_DESC& desc )
        {
            d3d_desc.ByteWidth = desc.buffSizeInByte;
            d3d_desc.Usage     = (D3D11_USAGE)desc.usage;
            d3d_desc.BindFlags = desc.bind_flag;
            d3d_desc.CPUAccessFlags = desc.cpu_access_flag;
            d3d_desc.MiscFlags      = desc.misc_flag;
            d3d_desc.StructureByteStride = sizeof( D3D11_BUFFER_DESC );
        }

        void toD3DSubResourceData( D3D11_SUBRESOURCE_DATA & d3d_data, const SUBRESOURCE_DATA& data )
        {
            d3d_data.pSysMem            = data.pSysMem;
            d3d_data.SysMemPitch        = data.sysMemPitch;
            d3d_data.SysMemSlicePitch   = data.sysMemSlicePitch;
        }


    }
}

