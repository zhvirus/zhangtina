#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/EffectSolid.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( EffectSolid )
        const char* const EffectSolid::m_sEffectName = "Solid";

        const char* const EffectSolid::m_sShaderCodes =
            "float4x4 gWorldViewProjMat;\n"
            "float4   gSolidColor;\n"
            "\n"
            "struct VS_OUTPUT{\n"
            "   float4 position : SV_POSITION;\n"
            "};\n"
            "\n"
            "VS_OUTPUT RenderSceneVS( float4 pos: POSITION )\n"
            "{\n"
            "   VS_OUTPUT output;\n"
            "   output.position = mul( pos, gWorldViewProjMat);\n"
            "   return output;\n"
            "}\n"
            "\n"
            "struct PS_OUTPUT{\n"
            "   float4 color : SV_TARGET;\n"
            "};\n"
            "\n"
            "PS_OUTPUT RenderScenePS( VS_OUTPUT inData )\n"
            "{\n"
            "   PS_OUTPUT output;\n"
            "   output.color = gSolidColor;\n"
            "   return output;\n"
            "}\n"
            "\n"
            "technique11 RenderScene\n"
            "{\n"
            "    pass P0\n"
            "    {\n"
            "        SetVertexShader( CompileShader( vs_5_0, RenderSceneVS() ));\n"
            "        SetGeometryShader( NULL );\n"
            "        SetPixelShader( CompileShader( ps_5_0, RenderScenePS() ));\n"
            "    }\n"
            "}\n"
            ;

        EffectSolid::EffectSolid():
            Effect(EffectSolid::m_sEffectName)
        {
            bool result = buildEffect( m_sEffectName, m_sShaderCodes, strlen(m_sShaderCodes) );
            assert( result );
        }

        EffectSolid::~EffectSolid()
        {
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
