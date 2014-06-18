#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/Enums.h"
#include "Util/Cache.h"
#include "Util/Array.h"

namespace ZH{
    namespace Graphics{

        // Forwards
        class IndexBufferImp;

        class ZH_GRAPHICS_DLL IndexBuffer: public Resource
        {
            CLASS_IDENTIFIER( E_CID_INDEX_BUFFER );
        public:

            virtual ~IndexBuffer();
            virtual bool isValid();

            bool operator==(const IndexBuffer&);

        private:
            IndexBufferImp* m_pImp;
            BUFFER_DESC     m_desc;

            friend class DeviceDX11;
            IndexBuffer( const char* const, IndexBufferImp*, const BUFFER_DESC& desc );
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<IndexBuffer>;
        typedef ZH::UTIL::Cache<IndexBuffer> IndexBufferCache;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<IndexBuffer*>;
        typedef ZH::UTIL::Array<IndexBuffer*> IndexBufferPtrArray;

    } // namespace Graphics
} // namespace ZH

#endif
