#ifndef RENDER_ITEM_H
#define RENDER_ITEM_H

#include <map>
#include <string>

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/GeometryInstance.h"
#include "Graphics/Effect.h"
#include "Util/Array.h"
#include "Math/Matrix4x4_f.h"

namespace ZH{
    namespace Graphics{

        enum E_RENDER_ITEM_FLAG{
            E_DISABLED  = 0x1
        };

        class ZH_GRAPHICS_DLL RenderItem : public Resource
        {
        public:
            virtual bool isValid();

            // World matrix
            const ZH::Math::matrix4x4_f& worldMatrix() { return m_worldMatrix; }
            void worldMatrix( const ZH::Math::matrix4x4_f& mat ) { m_worldMatrix = mat; }

            // Effect instance
            const EffectInstance* effectInst()const { return m_effectInst; }
            void effectInst( EffectInstance* inst ) { m_effectInst = inst; }

        private:
            // World matrix
            ZH::Math::matrix4x4_f   m_worldMatrix;

            // Geomtry instance
            GeometryInstance        m_geoInst;

            // Effect instance
            EffectInstance*         m_effectInst;
        private:
            RenderItem( const char* const );
            virtual ~RenderItem();

            friend class RenderNode;
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Array<RenderItem*>;
        typedef ZH::UTIL::Array<RenderItem*> RenderItemPtrArray;

        typedef std::map<std::string, RenderItem*> RenderItemMap;

    } // namespace Graphics
} // namespace ZH

#endif