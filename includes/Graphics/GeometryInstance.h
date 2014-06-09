#ifndef GEOMETRY_INSTANCE_H
#define GEOMETRY_INSTANCE_H

#include "Common/ZHSTD.h"
#include "Util/Array.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"


namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL GeometryInstance
        {
        public:
            virtual bool isValid();

            // Index buffer
            IndexBuffer* indexBuffer() { return m_pIndexBuffer; }
            void         indexBuffer( IndexBuffer* ib ) { m_pIndexBuffer = ib; }

            // Vertex buffer array
            VertexBufferPtrArray& vertexBufferArray() { return m_vertexBufferArr; }

        private:
            VertexBufferPtrArray m_vertexBufferArr;
            IndexBuffer*         m_pIndexBuffer;

        private:
            GeometryInstance();
            virtual ~GeometryInstance();

            friend class RenderItem;
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

    } // namespace Graphics
} // namespace ZH

#endif
