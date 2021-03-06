#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/EffectSolid.h"
#include "Graphics/ResourceManager.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/ShaderLibrary.h"


namespace ZH{
    namespace Graphics{

        const char* const EffectSolid::m_sDefaultName = "SolidColor";

        EffectSolid::EffectSolid( const char* const name ):
            Effect(name)
        {
            buildEffect();
        }

        EffectSolid::~EffectSolid()
        {

        }

        bool EffectSolid::isValid()
        {
            return m_pVertexShader && m_pPixelShader;
        }

        bool EffectSolid::buildEffect()
        {
            bool result = true;
            if ( !createShader( E_SHADER_TYPE_VERTEX, E_VS_POS ) ){
                result = false;
                ZH::Util::ENG_ERR("EffectSolid: create vertex shader failed!\n");
            }

            if ( !createShader( E_SHADER_TYPE_PIXEL, E_PS_SOLID_COLOR ) ){
                result = false;
                ZH::Util::ENG_ERR("EffectSolid: create pixel shader failed!\n");
            }

            if (!result){
                ZH::Util::ENG_ERR("EffectSolid: build effect failed!\n");
            }

            return result;
        }

        bool EffectSolid::setColor( const ZH::Math::float4& /*col*/ )
        {
            assert(isValid());
            if ( !isValid() ){
                return false;
            }

            return true;
        }


        //-----------------------------------
        //  EffectInstanceSolid
        //-----------------------------------

        EffectInstanceSolid::EffectInstanceSolid( const char* const name ):
            EffectInstance( name )
        {
            m_pEffect = ResourceManager::instance().findEffect( E_CID_EFFECT_SOLID, EffectSolid::m_sDefaultName );
        }

        EffectInstanceSolid::~EffectInstanceSolid()
        {
        }

        bool EffectInstanceSolid::isValid() const
        {
            return m_pEffect && m_pEffect->isValid();
        }

    }
}
