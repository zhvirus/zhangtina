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
            SAFE_RELEASE( m_pVertexBuffer_d3d );
        }

        bool VertexBufferImp::isValid()
        {
            return ( NULL != m_pVertexBuffer_d3d );

        }

    }
}



