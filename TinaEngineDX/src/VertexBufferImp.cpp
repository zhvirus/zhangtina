#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Internal/Graphics/VertexBufferImp.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        VertexBufferImp::VertexBufferImp( ID3D11Buffer* pBuffer ):
            m_pVertexBuffer_d3d( pBuffer )
        {
            assert( pBuffer );
        }

        VertexBufferImp::~VertexBufferImp()
        {
            SAFE_RELEASE<ID3D11Buffer*>( m_pVertexBuffer_d3d );
        }

        bool VertexBufferImp::isValid()
        {
            return ( NULL != m_pVertexBuffer_d3d );

        }

    }
}



