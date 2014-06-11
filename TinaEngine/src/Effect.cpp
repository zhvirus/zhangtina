#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Effect.h"
#include "Internal/Graphics/Internal_common_graphics.h"

#define IMP_PTR ((ID3DBlob*)m_pShader)

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Effect )
        CLASS_TYPE_NAME_DEFINITION( EffectInstance )
        //-----------------------------------
        // Effect
        //-----------------------------------
        Effect::Effect( const char* const name):
            Resource( name ),
            m_pShader( NULL )
        {

        }

        Effect::~Effect()
        {
            ID3DBlob* blob = IMP_PTR;
            SAFE_RELEASE<ID3DBlob*>( blob );
        }

        bool Effect::isValid()
        {
            return ( NULL != m_pShader );
        }

        bool Effect::buildEffect( const char* const sourceName, const char* const codes, size_t size )
        {
            if ( m_pShader ){
                ZH::Util::ENG_ERR("Shader of '%s' already compiled!\n", sourceName );
                return false;
            }

            // ------------------------------
            // 1. Compile the shader
            // ------------------------------
            ID3DBlob* blob;
            ID3DBlob* err_blob;
            // http://msdn.microsoft.com/en-us/library/windows/desktop/dd607324(v=vs.85).aspx
            // Compile HLSL code or an effect file into bytecode for a given target.
            HRESULT result = 
            D3DCompile(
                (LPCVOID)codes,
                (SIZE_T)size,
                sourceName,     // pSourceName
                NULL,           // pDefines
                NULL,           // pInclude
                "",             // pEntrypoint
                "fx_5_0",       // pTarget
                0,              // compile flag
                0,              // effect complile flag
                &blob,
                &err_blob
                );

            if ( result != S_OK ){
                char* errMsg = (char*)err_blob->GetBufferPointer();
                ZH::Util::ENG_ERR("Shader '%s' compiled failed with output:\n%s\n", sourceName, errMsg);
            } else{

                //D3DX11CompileEffectFromMemory

            }

            m_pShader = (void*)blob;

            SAFE_RELEASE<ID3DBlob*>( blob );
            SAFE_RELEASE<ID3DBlob*>( blob );
                //return false;
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

