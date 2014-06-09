#ifndef INDEXBUFFER_IMP_H
#define INDEXBUFFER_IMP_H

#include "Common/ZHSTD.h"
#include "Internal/Graphics/Internal_common_graphics.h"


namespace ZH{
    namespace Graphics{
        class IndexBufferImp{
        public:
            IndexBufferImp( ID3D11Buffer* );
            ~IndexBufferImp();
            bool isValid();

            ID3D11Buffer* getIndexBuffer_d3d() { return m_pIndexBuffer_d3d; }

        private:
            ID3D11Buffer* m_pIndexBuffer_d3d;
        };
    }
}


#endif
