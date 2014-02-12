#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderNode.h"
#include "Internal/Common/internal_common.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Effect.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( RenderNode, "RenderNode" )

        RenderNode::RenderNode():
            m_pIndexBuffer(NULL),
            m_pVertexBuffer(NULL),
            m_pEffectInst(NULL)
        {

        }

        RenderNode::~RenderNode()
        {
        }

        bool RenderNode::isValid()
        {
            bool valid = m_pIndexBuffer && m_pVertexBuffer && m_pEffectInst &&
                m_pIndexBuffer->isValid() && m_pVertexBuffer->isValid() &&
                m_pEffectInst->isValid();

            return valid;
        }

        void RenderNode::indexBuffer( IndexBuffer* ib )
        {
            m_pIndexBuffer = ib;
        }
        void RenderNode::vertexBuffer( VertexBuffer* vb )
        {
            m_pVertexBuffer = vb;
        }
        void RenderNode::effectInstance( EffectInstance* ef )
        {
            m_pEffectInst = ef;
        }

    }
}