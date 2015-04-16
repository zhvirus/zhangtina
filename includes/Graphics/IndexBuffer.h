#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"
#include "Graphics/Resource.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL IndexBuffer : public Buffer
        {
            CLASS_IDENTIFIER(E_CID_INDEXBUFFER);
        public:


        protected:
            IndexBuffer(Context* ctx);
            CLASS_COMMON_PROTECTED_DECLEARATION(IndexBuffer);
            friend class Context;
        };


    }
}


#endif