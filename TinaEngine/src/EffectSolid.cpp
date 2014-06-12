#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/EffectSolid.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/ShaderLibrary.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( EffectSolid )

        EffectSolid::EffectSolid():
            Effect("SolidColor", E_EFFECT_SOLID_COLOR)
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

    }
}
