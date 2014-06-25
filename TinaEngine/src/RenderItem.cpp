#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/RenderItem.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        RenderItem::RenderItem( const char* const name ):
            Resource(name),
            m_pEffectInst(NULL),
            m_bufferDirty( true )
        {

        }

        RenderItem::~RenderItem()
        {
        }

        bool RenderItem::isValid()
        {
            bool valid = m_pEffectInst && m_pEffectInst->isValid() && m_geoInst.isValid();
            return valid;
        }

        void RenderItem::effectInst( EffectInstance* inst )
        {
            m_pEffectInst = inst;
            m_bufferDirty = true;
        }

        bool RenderItem::updateStreams()
        {
            if (!m_bufferDirty){
                return true;
            }

            ASSERT_NOT_NULL_RET_FALSE( m_pEffectInst );
            Effect* pEffect = m_pEffectInst->effect();
            ASSERT_NOT_NULL_RET_FALSE(pEffect);

            bool result = m_geoInst.collectStreams( pEffect, name() );
            assert( result );

            return result;
        }


    }
}
