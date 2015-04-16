#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"
#include "Graphics/Resource.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL VertexBuffer : public Buffer
        {
            CLASS_IDENTIFIER(E_CID_VERTEXBUFFER);
        public:


        protected:
            VertexBuffer(Context* ctx);
            CLASS_COMMON_PROTECTED_DECLEARATION(VertexBuffer);
            friend class Context;
        };


    }
}


#endif