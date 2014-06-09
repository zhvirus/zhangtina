#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Cache.h"

namespace ZH{
    namespace Graphics{

        // Forwards
        class IndexBufferImp;

        class ZH_GRAPHICS_DLL IndexBuffer: public Resource
        {
        public:

            virtual ~IndexBuffer();
            virtual bool isValid();

            bool operator==(const IndexBuffer&);

        private:
            IndexBufferImp* m_pImp;
            BUFFER_DESC     m_desc;

            friend class DeviceDX11;
            IndexBuffer( const char* const, IndexBufferImp*, const BUFFER_DESC& desc );

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<IndexBuffer>;
        typedef ZH::UTIL::Cache<IndexBuffer> IndexBufferCache;

    } // namespace Graphics
} // namespace ZH

#endif
