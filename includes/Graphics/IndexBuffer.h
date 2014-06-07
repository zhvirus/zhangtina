#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Cache.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL IndexBuffer: public Resource
        {
        public:
            IndexBuffer( const char* const );
            virtual ~IndexBuffer();
            virtual bool isValid();

            bool operator==(const IndexBuffer&);

        private:
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<IndexBuffer>;
        typedef ZH::UTIL::Cache<IndexBuffer> IndexBufferCache;

    } // namespace Graphics
} // namespace ZH

#endif
