#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Internal/Graphics/ShaderLibrary.h"
#include "Util/Print.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( ShaderLibrary );

        ShaderCodes::ShaderCodes():
            m_key( E_SHADER_KEY_INVALID ),
            m_name(""),
            m_codes(""),
            m_entry(""),
            m_target(""),
            m_blob(NULL)
        {
        }


        void ShaderLibrary::buildShaderMaps()
        {
            ShaderCodes* pShader = NULL;

            //---------------------------------------------
            //
            //    Vertex shaders
            //
            //---------------------------------------------

            // 1. Simple position VS
            pShader = new ShaderCodes;
            pShader->m_key    = E_VS_POS;
            pShader->m_name   = "vs_pos";
            pShader->m_entry  = "VSMain";
            pShader->m_target = "vs_5_0";
            pShader->m_codes  =
                "cbuffer cbMatrix {\n"
                "   float4x4 gWorldViewProjMat : packoffset(c0);\n"
                "};\n"
                "\n"
                "struct VS_OUTPUT{\n"
                "   float4 position : SV_POSITION;\n"
                "};\n"
                "\n"
                "VS_OUTPUT VSMain( float4 pos: POSITION )\n"
                "{\n"
                "   VS_OUTPUT output;\n"
                "   output.position = mul( pos, gWorldViewProjMat);\n"
                "   return output;\n"
                "}\n"
                "\n"
                ;
            m_vsMap.insert( std::make_pair(pShader->m_key, pShader) );

            //---------------------------------------------
            //
            //    Hull shaders
            //
            //---------------------------------------------


            //---------------------------------------------
            //
            //    Domain shaders
            //
            //---------------------------------------------

            //---------------------------------------------
            //
            //    Geometry shaders
            //
            //---------------------------------------------

            //---------------------------------------------
            //
            //    Pixel shaders
            //
            //---------------------------------------------
            pShader = new ShaderCodes;
            pShader->m_key    = E_PS_SOLID_COLOR;
            pShader->m_name   = "ps_solid_color";
            pShader->m_entry  = "PSMain";
            pShader->m_target = "ps_5_0";
            pShader->m_codes  =
                "cbuffer cbColor : register(b0) {\n"
                "   float4 g_solidColor : packoffset(c0);\n"
                "};\n"
                "\n"
                "float PSMain() : SV_TARGET \n"
                "{\n"
                "   return g_solidColor;\n"
                "}\n"
                "\n"
                ;
            m_psMap.insert( std::make_pair( pShader->m_key, pShader ) );

            //---------------------------------------------
            //
            //    Compute shaders
            //
            //---------------------------------------------
        }


        ShaderLibrary::ShaderLibrary():
            m_bInitialized( false )
        {
            buildShaderMaps();
        }

        ShaderLibrary::~ShaderLibrary()
        {
        }

        ID3DBlob* ShaderLibrary::compileShader(
            const char* const name,
            const char* const codes,
            const char* const entryPoint,
            const char* const target)
        {
            // Sanity check
            ASSERT_RET_NULL(name);
            ASSERT_RET_NULL(codes);
            ASSERT_RET_NULL(entryPoint);
            ASSERT_RET_NULL(target);

            // Code size
            const size_t code_size = strlen(codes);

            ID3DBlob* pBlob     = NULL;
            ID3DBlob* pErrBlob  = NULL;

            // http://msdn.microsoft.com/en-us/library/windows/desktop/dd607324(v=vs.85).aspx
            // Compile HLSL code or an effect file into bytecode for a given target.
            HRESULT result = 
                D3DCompile(
                (LPCVOID)codes,
                code_size,
                name,           // pSourceName
                NULL,           // pDefines
                NULL,           // pInclude
                entryPoint,     // pEntrypoint
                target,         // pTarget
                0,              // compile flag
                0,              // effect complile flag
                &pBlob,
                &pErrBlob
                );

            if ( result != S_OK ){
                char* errMsg = (char*)pErrBlob->GetBufferPointer();
                ZH::Util::ENG_ERR("\nShader '%s' compiled failed with output:\n%s\n", name, errMsg);
                SAFE_RELEASE<ID3DBlob*>( pBlob );
                SAFE_RELEASE<ID3DBlob*>( pErrBlob );
                return NULL;
            }

            SAFE_RELEASE<ID3DBlob*>( pErrBlob );
            return pBlob;
        }

        void ShaderLibrary::compileShaderMap( std::map<E_SHADER_KEY, ShaderCodes*>& shaderMap )
        {
            // Compile shaders
            std::map<E_SHADER_KEY, ShaderCodes*>::iterator it = shaderMap.begin();
            ShaderCodes* pShader = NULL;
            for( ;it != shaderMap.end(); ++it ){
                pShader = it->second;
                assert( pShader );
                assert( pShader->m_blob == NULL );
                if ( !pShader ){
                    continue;
                }
                ID3DBlob* pBlob = compileShader(
                    pShader->m_name.c_str(),
                    pShader->m_codes.c_str(),
                    pShader->m_entry.c_str(),
                    pShader->m_target.c_str());
                if ( !pBlob ){
                    ZH::Util::ENG_ERR("Build shader(%s) failed!\n", pShader->m_name.c_str());
                    delete pShader;
                    pShader = NULL;
                    it = shaderMap.erase(it);
                    continue;
                }else{
                    pShader->m_blob = pBlob;
                    pBlob = NULL;
                    ZH::Util::ENG_INF("Build shader(%s) ok\n", pShader->m_name.c_str());
                }
            }
        }

        bool ShaderLibrary::initialize()
        {
            if ( m_bInitialized ){
                ZH::Util::ENG_DBG("ShaderLibrary already initialized, skip re-initialize.\n");
                return true;
            }

            compileShaderMap( m_vsMap );
            compileShaderMap( m_hsMap );
            compileShaderMap( m_dsMap );
            compileShaderMap( m_gsMap );
            compileShaderMap( m_psMap );
            compileShaderMap( m_csMap );

            m_bInitialized = true;
            return true;
        }

        void ShaderLibrary::clearShaderMap( std::map<E_SHADER_KEY, ShaderCodes*>& shaderMap )
        {
            std::map<E_SHADER_KEY, ShaderCodes*>::iterator it = shaderMap.begin();
            ShaderCodes* pShader = NULL;
            for( ;it != shaderMap.end(); ++it ){
                pShader = it->second;
                if ( !pShader ){
                    continue;
                }

                ID3DBlob* pBlob = pShader->m_blob;
                if ( pBlob ){
                    pBlob->Release();
                }

                delete pShader;
            }
            shaderMap.clear();
        }

        bool ShaderLibrary::clear()
        {
            clearShaderMap(m_vsMap);
            clearShaderMap(m_hsMap);
            clearShaderMap(m_dsMap);
            clearShaderMap(m_gsMap);
            clearShaderMap(m_psMap);
            clearShaderMap(m_csMap);

            return true;
        }

        const ShaderCodes* ShaderLibrary::getShader( E_SHADER_TYPE type, E_SHADER_KEY key ) const
        {
            switch ( type ){
            case E_SHADER_TYPE_VERTEX:
                {
                    std::map<E_SHADER_KEY, ShaderCodes*>::const_iterator cIt = m_vsMap.find(key);
                    if( cIt != m_vsMap.end() ){
                        return cIt->second;
                    }
                }
                break;
            case E_SHADER_TYPE_HULL:
                {
                    std::map<E_SHADER_KEY, ShaderCodes*>::const_iterator cIt = m_hsMap.find(key);
                    if( cIt != m_hsMap.end() ){
                        return cIt->second;
                    }
                }
                break;
            case E_SHADER_TYPE_DOMAIN:
                {
                    std::map<E_SHADER_KEY, ShaderCodes*>::const_iterator cIt = m_dsMap.find(key);
                    if( cIt != m_dsMap.end() ){
                        return cIt->second;
                    }
                }
                break;
            case E_SHADER_TYPE_GEOMETRY:
                {
                    std::map<E_SHADER_KEY, ShaderCodes*>::const_iterator cIt = m_gsMap.find(key);
                    if( cIt != m_gsMap.end() ){
                        return cIt->second;
                    }
                }
                break;
            case E_SHADER_TYPE_PIXEL:
                {
                    std::map<E_SHADER_KEY, ShaderCodes*>::const_iterator cIt = m_psMap.find(key);
                    if( cIt != m_psMap.end() ){
                        return cIt->second;
                    }
                }
                break;
            case E_SHADER_TYPE_COMPUTE:
                {
                    std::map<E_SHADER_KEY, ShaderCodes*>::const_iterator cIt = m_csMap.find(key);
                    if( cIt != m_csMap.end() ){
                        return cIt->second;
                    }
                }
                break;
            default:
                {
                    assert(false);
                    ZH::Util::ENG_ERR("ShaderLibrary::getShader() : Wrong shader type!\n");
                }
                break;
            } // Switch

            return NULL;
        }






    }
}
