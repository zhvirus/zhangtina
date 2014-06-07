#ifndef RENDER_NODE_H
#define RENDER_NODE_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Array.h"

namespace ZH{
    namespace Graphics{

        class IndexBuffer;
        class VertexBuffer;
        class EffectInstance;

        class ZH_GRAPHICS_DLL RenderNode : public Resource
        {
        public:
            RenderNode( const char* const );
            virtual ~RenderNode();

            virtual bool isValid();

            void          indexBuffer( IndexBuffer* );
            IndexBuffer*  indexBuffer()   { return m_pIndexBuffer; }

            void          vertexBuffer( VertexBuffer* );
            VertexBuffer* vertexBuffer() { return m_pVertexBuffer; }

            void                effectInstance( EffectInstance* );
            EffectInstance*     effectInstance(){ return m_pEffectInst; }

        private:
            IndexBuffer*        m_pIndexBuffer;
            VertexBuffer*       m_pVertexBuffer;
            EffectInstance*     m_pEffectInst;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderNode*>;
        typedef ZH::UTIL::Array<RenderNode*> RenderNodePtrArray;

    } // namespace Graphics
} // namespace ZH

#endif
