#ifndef RENDER_NODE_H
#define RENDER_NODE_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/RenderItem.h"
#include "Util/Array.h"
#include "Math/Matrix4x4_f.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL RenderNode : public Resource
        {
        public:
            virtual bool isValid();

            // World matrix
            const ZH::Math::matrix4x4_f& worldMatrix() { return m_worldMatrix; }
            void worldMatrix( const ZH::Math::matrix4x4_f& mat ) { m_worldMatrix = mat; }

            bool        removeRenderItem( const char* const );
            RenderItem* addRenderItem( const char* const );
            RenderItem* findRenderItem( const char* const );

            void enableRenderItem( const char* const );
            void disableRenderItem( const char* const );

            void clear();

        private:
            // World matrix
            ZH::Math::matrix4x4_f m_worldMatrix;

            // Render items of this node
            RenderItemPtrArray    m_renderItems;

        private:
            RenderNode( const char* const );
            virtual ~RenderNode();

            friend class SimpleWorld;
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderNode*>;
        typedef ZH::UTIL::Array<RenderNode*> RenderNodePtrArray;

    } // namespace Graphics
} // namespace ZH

#endif
