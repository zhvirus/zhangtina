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

            return true;
        }

        bool EffectSolid::buildEffect()
        {
            const ShaderLibrary& shaderLib = ShaderLibrary::instance();
            if ( !shaderLib.ready() ){
                ZH::Util::ENG_ERR("EffectSolid::buildEffect(): shader lib not ready!\n");
                return false;
            }

            // Create vertex shader
            const ShaderCodes* pShaderCode = shaderLib.getShader( E_SHADER_TYPE_VERTEX, E_VS_POS );
            if ( !pShaderCode ){
                ZH::Util::ENG_ERR("Can't find vertex shader for EffectSolid!\n");
                return false;
            }else{
                ID3D11VertexShader* pVertexShader = NULL;
                ID3D11Device* pDevice = AccessInternal::device();


            }



            return true;
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
