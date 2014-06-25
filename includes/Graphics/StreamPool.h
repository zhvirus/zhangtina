#ifndef STREAM_POOL_H
#define STREAM_POOL_H

#include "Common/ZHSTD.h"
#include "Graphics/Enums.h"
#include <map>

namespace ZH{
    namespace Graphics{

        // Forwards
        class VertexBuffer;
        class IndexBuffer;
        class BUFFER_DESC;
        class SUBRESOURCE_DATA;


        class ZH_GRAPHICS_DLL StreamPool
        {
        public:
            static StreamPool& instance() { static StreamPool sp; return sp; }

            // Clear all the buffers inside
            void clear();

            // Vertex buffer
            VertexBuffer* findVertexBuffer( SEMANTIC_TYPE, const char* const name );
            VertexBuffer* acquireVertexBuffer(
                SEMANTIC_TYPE,
                const char* const name,
                const BUFFER_DESC&,
                const SUBRESOURCE_DATA&
                );

            // Index buffer
            IndexBuffer* findIndexBuffer( const char* const name );
            IndexBuffer* acquireIndexBuffer(
                const char* const name,
                const BUFFER_DESC&,
                const SUBRESOURCE_DATA&
                );


        private:
            // functions
            bool insertStream( SEMANTIC_TYPE, VertexBuffer* );

        private:
            // data
            typedef std::map<const char* const, VertexBuffer*>  CHAR_VB_MAP;
            typedef std::map<SEMANTIC_TYPE, CHAR_VB_MAP* >        STREAM_CHARVB_MAP;
            STREAM_CHARVB_MAP* m_pStreamPool;

            typedef std::map<const char* const, IndexBuffer*> CHAR_IB_MAP;
            CHAR_IB_MAP* m_pIndexBufferPool;



        private:
            StreamPool();
            ~StreamPool();
            StreamPool( const StreamPool& ){}
            StreamPool& operator = ( const StreamPool& ){ return *this; }
        };



    }
}


#endif
