#ifndef RENDER_NODE_H
#define RENDER_NODE_H

#include <map>
#include <string>

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/RenderItem.h"
#include "Util/Array.h"
#include "Math/Matrix4x4_f.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL RenderNode : public Resource
        {
            CLASS_IDENTIFIER( E_CID_RENDER_NODE );
        public:
            virtual ~RenderNode();
            virtual bool isValid();

            // World matrix
            const ZH::Math::matrix4x4_f& worldMatrix() { return m_worldMatrix; }
            void worldMatrix( const ZH::Math::matrix4x4_f& mat ) { m_worldMatrix = mat; }

            virtual bool    addRenderItem( RenderItem* );
            virtual bool    removeRenderItem( const char* const );
            RenderItem*     findRenderItem( const char* const );

            void enableRenderItem( const char* const );
            void disableRenderItem( const char* const );

            void clear();

        protected:
            // World matrix
            ZH::Math::matrix4x4_f m_worldMatrix;

            // Render items of this node
            RenderItemMap*        m_pRenderItems;

        protected:
            virtual bool prepareDefaultData();
            RenderNode( const char* const );

            friend class ResourceFactory;
        };

        typedef std::map<std::string, RenderNode*> RenderNodeMap;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderNode*>;
        typedef ZH::UTIL::Array<RenderNode*> RenderNodePtrArray;

    } // namespace Graphics
} // namespace ZH

#endif
