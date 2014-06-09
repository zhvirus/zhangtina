#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/Enums.h"
#include "Util/Cache.h"
#include "Util/Array.h"


namespace ZH{
    namespace Graphics{

        // Forwards
        class VertexBufferImp;

        class ZH_GRAPHICS_DLL VertexBuffer: public Resource
        {
        public:
            virtual ~VertexBuffer();
            virtual bool isValid();

            bool operator == ( const VertexBuffer& );

        private:
            VertexBufferImp* m_pImp;
            BUFFER_DESC      m_desc;

            friend class DeviceDX11;
            VertexBuffer( const char* const, VertexBufferImp*, const BUFFER_DESC& desc );

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<VertexBuffer>;
        typedef ZH::UTIL::Cache<VertexBuffer> VertexBufferCache;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<VertexBuffer*>;
        typedef ZH::UTIL::Array<VertexBuffer*> VertexBufferPtrArray;

    } // namespace Graphics
} // namespace ZH

#endif
