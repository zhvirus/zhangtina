#include "Internal/Graphics/IndexBufferImp.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        IndexBufferImp::IndexBufferImp( ID3D11Buffer* pBuffer ):
            m_pIndexBuffer_d3d( pBuffer )
        {
            assert( pBuffer );
        }

        IndexBufferImp::~IndexBufferImp()
        {
            SAFE_RELEASE<ID3D11Buffer*>( m_pIndexBuffer_d3d );
        }

        bool IndexBufferImp::isValid()
        {
            return ( NULL != m_pIndexBuffer_d3d );

        }

    }
}

