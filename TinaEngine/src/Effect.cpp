#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Effect.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Effect )
        CLASS_TYPE_NAME_DEFINITION( EffectInstance )
        //-----------------------------------
        // Effect
        //-----------------------------------
        Effect::Effect( const char* const name, E_EFFECT_KEY key):
            Resource( name ),
            m_pVertexShader(NULL),
            m_pHullShader(NULL),
            m_pDomainShader(NULL),
            m_pGeometryShader(NULL),
            m_pPixelShader(NULL),
            m_pComputeShader(NULL),
            m_eEffectKey(key)
        {
        }

        Effect::~Effect()
        {
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

