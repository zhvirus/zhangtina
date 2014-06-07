#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Cache.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL VertexBuffer: public Resource
        {
        public:
            VertexBuffer( const char* const );
            virtual ~VertexBuffer();

            virtual bool isValid();

            bool operator == ( const VertexBuffer& );
        private:

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<VertexBuffer>;
        typedef ZH::UTIL::Cache<VertexBuffer> VertexBufferCache;

    } // namespace Graphics
} // namespace ZH

#endif
