#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/GeometryInstance.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Graphics/Effect.h"
#include "Graphics/InputLayout.h"
#include "Graphics/StreamPool.h"

namespace ZH{
    namespace Graphics{

        GeometryInstance::GeometryInstance():
            m_pIndexBuffer(NULL),
            m_primitiveType(PRIMITIVE_TOPOLOGY_TRIANGLELIST)
        {

        }

        GeometryInstance::~GeometryInstance()
        {
        }

        bool GeometryInstance::isValid()
        {
            bool valid = m_pIndexBuffer && m_pIndexBuffer->isValid();
            valid = valid && (m_vertexBufferArr.size() > 0);
            return valid;
        }

        bool GeometryInstance::collectStreams( Effect* pEffect, const char* const name )
        {
            ASSERT_NOT_NULL_RET_FALSE(pEffect);
            ASSERT_NOT_NULL_RET_FALSE(name);


            const InputLayout& inputLayout = pEffect->inputLayout();

            const std::vector<Semantic*>& pSlotVec = inputLayout.getSlotInputVec();
            const unsigned int slotSize = (unsigned int)(pSlotVec.size());
            ASSERT_NOT_NULL_RET_FALSE( slotSize > 0 );

            m_vertexBufferArr.clear();

            bool result = true;
            for(unsigned int i=0; i<slotSize; ++i){
                Semantic* pSem = pSlotVec[i];
                if ( !pSem ){
                    result = false;
                    assert(pSem);
                    continue;
                }

                VertexBuffer* pVB = StreamPool::instance().findVertexBuffer( pSem->m_type, name );
                assert(pVB);
                if ( !pVB ){
                    result = false;
                    ZH::Util::ENG_ERR("Can't find Stream named '%s'!\n", name);
                    continue;
                }

                m_vertexBufferArr.push_back(pVB);
            }

            return result;
        }

    }
}
