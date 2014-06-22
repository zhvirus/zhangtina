#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/StreamPool.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Graphics/VertexBuffer.h"



namespace ZH{
    namespace Graphics{

        StreamPool::StreamPool()
        {
            m_streamPool = new STREAM_CHARVB_MAP;
        }


        StreamPool::~StreamPool()
        {
            clear();
        }

        void StreamPool::clear()
        {
            if ( !m_streamPool ){
                return;
            }

            STREAM_CHARVB_MAP::iterator it1 = m_streamPool->begin();
            for( ;it1 != m_streamPool->end(); ++it1){
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
            
            m_streamPool->clear();
            delete m_streamPool;
            m_streamPool = NULL;
        }


    }

}
