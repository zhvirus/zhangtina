#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/StreamPool.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Graphics/VertexBuffer.h"
#include "GRaphics/IndexBuffer.h"
#include "Graphics/DeviceDX11.h"


namespace ZH{
    namespace Graphics{

        StreamPool::StreamPool()
        {
            m_pStreamPool = new STREAM_CHARVB_MAP;
            m_pIndexBufferPool = new CHAR_IB_MAP;
        }


        StreamPool::~StreamPool()
        {
            clear();
        }

        void StreamPool::clear()
        {
            // Clean up vertex buffers
            if ( m_pStreamPool ){

                STREAM_CHARVB_MAP::iterator it1 = m_pStreamPool->begin();
                for( ;it1 != m_pStreamPool->end(); ++it1){
                    CHAR_VB_MAP* pCharVBMap = it1->second;
                    if ( !pCharVBMap ){
                        continue;
                    }

                    CHAR_VB_MAP::iterator it2 = pCharVBMap->begin();
                    for(; it2 != pCharVBMap->end(); ++it2 ){
                        VertexBuffer* pVB = it2->second;
                        if ( !pVB ){
                            continue;
                        }
                        delete pVB;
                    }
                    pCharVBMap->clear();
                    delete pCharVBMap;
                }

                m_pStreamPool->clear();
                delete m_pStreamPool;
                m_pStreamPool = NULL;
            }

            // Clean up index buffers
            if ( m_pIndexBufferPool ){
                CHAR_IB_MAP::iterator it = m_pIndexBufferPool->begin();
                for(;it != m_pIndexBufferPool->end(); ++it){
                    IndexBuffer* pIndexBuf = it->second;
                    if ( !pIndexBuf ){
                        continue;
                    }
                    delete pIndexBuf;
                }
                m_pIndexBufferPool->clear();
                delete m_pIndexBufferPool;
                m_pIndexBufferPool = NULL;
            }

        }

        // Insert stream
        bool StreamPool::insertStream(
            SEMANTIC_TYPE type,
            VertexBuffer* pVB)
        {
            ASSERT_NOT_NULL_RET_FALSE(m_pStreamPool);
            ASSERT_NOT_NULL_RET_FALSE(pVB);

            CHAR_VB_MAP* pVBMap = NULL;

            STREAM_CHARVB_MAP::iterator it = m_pStreamPool->find(type);
            if ( m_pStreamPool->end() != it ){
                pVBMap = it->second;
                assert(pVBMap);
                if ( !pVBMap ){
                    m_pStreamPool->erase(it);
                }
            }

            if ( !pVBMap ){
                pVBMap = new CHAR_VB_MAP;
                m_pStreamPool->insert(std::make_pair( type, pVBMap));
            }

            pVBMap->insert(std::make_pair(pVB->name(), pVB));

            return true;
        }

        // Find vertex buffer
        VertexBuffer* StreamPool::findVertexBuffer(
            SEMANTIC_TYPE type,
            const char* const name)
        {
            ASSERT_NOT_NULL_RET_NULL(m_pStreamPool);
            ASSERT_NOT_NULL_RET_NULL(name);

            STREAM_CHARVB_MAP::const_iterator cIt = m_pStreamPool->find(type);
            if ( m_pStreamPool->end() == cIt ){
                return NULL;
            }

            CHAR_VB_MAP* pVBMapOnThisType = cIt->second;
            ASSERT_NOT_NULL_RET_NULL( pVBMapOnThisType );

            CHAR_VB_MAP::const_iterator cIt2 = pVBMapOnThisType->find(name);
            if ( pVBMapOnThisType->end() == cIt2 ){
                return NULL;
            }

            VertexBuffer* pVB = cIt2->second;
            if ( pVB ){
                assert(pVB->type() == type);
            }

            return cIt2->second;
        }

        // Acquire vertex buffer
        VertexBuffer* StreamPool::acquireVertexBuffer(
                SEMANTIC_TYPE type,
                const char* const name,
                const BUFFER_DESC& desc,
                const SUBRESOURCE_DATA& data )
        {
            // Find in cache first
            VertexBuffer* pRes = findVertexBuffer( type, name );
            if ( pRes ){
                return pRes;
            }

            // Create a new vertex buffer
            DeviceDX11* pDevice = DeviceDX11::instance();
            ASSERT_NOT_NULL_RET_NULL(pDevice);

            VertexBuffer* pVB = pDevice->createVertexBuffer( type, name, desc, data );
            ASSERT_NOT_NULL_RET_NULL(pVB);

            // Save the new vertex buffer
            if ( pVB ){
                bool result = insertStream( type, pVB );
                assert(result);
            }

            return pVB;
        }


        // Find index buffer by name
        IndexBuffer* StreamPool::findIndexBuffer( const char* const name )
        {
            ASSERT_NOT_NULL_RET_NULL(name);
            ASSERT_NOT_NULL_RET_NULL(m_pIndexBufferPool);

            CHAR_IB_MAP::const_iterator cIt = m_pIndexBufferPool->find( name );
            if ( m_pIndexBufferPool->end() == cIt){
                return NULL;
            }

            return cIt->second;
        }

        // Acquire index buffer
        IndexBuffer* StreamPool::acquireIndexBuffer(
                const char* const name,
                const BUFFER_DESC& desc,
                const SUBRESOURCE_DATA& data )
        {
            // Find in cache first
            IndexBuffer* pRes = findIndexBuffer( name );
            if ( pRes ){
                return pRes;
            }

            // Create a new index buffer
            DeviceDX11* pDevice = DeviceDX11::instance();
            ASSERT_NOT_NULL_RET_NULL(pDevice);

            IndexBuffer* ib = pDevice->createIndexBuffer( name, desc, data );
            if ( ib ){
                m_pIndexBufferPool->insert(std::make_pair(name, ib));
            }

            return ib;
        }

    }

}
