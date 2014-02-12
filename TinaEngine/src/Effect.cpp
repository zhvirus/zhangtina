#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Effect.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Effect, "Effect" )
        CLASS_TYPE_NAME_DEFINITION( EffectInstance, "EffectInstance" )
        //-----------------------------------
        // Effect
        //-----------------------------------
        Effect::Effect()
        {

        }

        Effect::~Effect()
        {

        }

        bool Effect::isValid()
        {
            return true;
        }

        //-----------------------------------
        // EffectInstance
        //-----------------------------------
        EffectInstance::EffectInstance():
            m_pEffect( NULL )
        {

        }

        EffectInstance::~EffectInstance()
        {
        }
    }
}

