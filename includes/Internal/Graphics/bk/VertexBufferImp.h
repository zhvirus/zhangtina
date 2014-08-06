#ifndef VERTEXBUFFER_IMP_H
#define VERTEXBUFFER_IMP_H

#include "Common/ZHSTD.h"
#include "Internal/Graphics/Internal_common_graphics.h"


namespace ZH{
    namespace Graphics{
        class VertexBufferImp{
        public:
            VertexBufferImp( ID3D11Buffer* );
            ~VertexBufferImp();
            bool isValid();

            ID3D11Buffer* getVertexBuffer_d3d() { return m_pVertexBuffer_d3d; }

        private:
            ID3D11Buffer* m_pVertexBuffer_d3d;
        };
    }
}


#endif
