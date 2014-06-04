#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Effect.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Effect )
        CLASS_TYPE_NAME_DEFINITION( EffectInstance )
        //-----------------------------------
        // Effect
        //-----------------------------------
        Effect::Effect( const char* const name):
            Resource(name)
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
        EffectInstance::EffectInstance( const char* const name ):
            Resource( name ),
            m_pEffect( NULL )
        {

        }

        EffectInstance::~EffectInstance()
        {
        }

        bool EffectInstance::operator== ( const EffectInstance& v)
        {
            // TODO
            return (*(v.m_name) == *m_name);
        }


    }
}

