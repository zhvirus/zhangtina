#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"

namespace ZH{
    namespace Graphics{

        class ZH_DLL VertexBuffer: public Resource
        {
        public:
            VertexBuffer();
            virtual ~VertexBuffer();

            virtual bool isValid();
        private:

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };


    } // namespace Graphics
} // namespace ZH

#endif
