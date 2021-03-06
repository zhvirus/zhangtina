#ifndef GEOMETRY_INSTANCE_H
#define GEOMETRY_INSTANCE_H

#include "Common/ZHSTD.h"
#include "Util/Array.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"


namespace ZH{
    namespace Graphics{

        class Effect;

        class ZH_GRAPHICS_DLL GeometryInstance
        {
            CLASS_IDENTIFIER( E_CID_GEOMETRY_INSTANCE );
        public:
            virtual bool isValid();

            // Index buffer
            IndexBuffer* indexBuffer() { return m_pIndexBuffer; }
            void         indexBuffer( IndexBuffer* ib ) { m_pIndexBuffer = ib; }

            // Vertex buffer array
            VertexBufferPtrArray& vertexBufferArray() { return m_vertexBufferArr; }

            // Topo
            void primitiveType( PRIMITIVE_TYPE type ) { m_primitiveType = type; }
            PRIMITIVE_TYPE primitiveType() const { return m_primitiveType; }

            // Collect streams
            bool collectStreams( Effect*, const char* const );

        private:
            VertexBufferPtrArray m_vertexBufferArr;
            IndexBuffer*         m_pIndexBuffer;
            PRIMITIVE_TYPE       m_primitiveType;

        private:
            GeometryInstance();
            virtual ~GeometryInstance();

            friend class RenderItem;
        };

    } // namespace Graphics
} // namespace ZH

#endif
