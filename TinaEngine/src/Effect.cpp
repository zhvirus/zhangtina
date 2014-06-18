#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Effect.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/ShaderLibrary.h"

namespace ZH{
    namespace Graphics{
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


        bool Effect::createShader( E_SHADER_TYPE type, E_SHADER_KEY key )
        {
            // Check shader library
            const ShaderLibrary& shaderLib = ShaderLibrary::instance();
            if ( !shaderLib.ready() ){
                ZH::Util::ENG_ERR("Effect::createShader(): shader lib not ready!\n");
                return false;
            }

            // Find the cached compiled shader blob
            const ShaderCodes* pShaderCode = shaderLib.getShader( type, key );
            if ( !pShaderCode ){
                ZH::Util::ENG_ERR("Can't find shader type:%d, key:%d!\n", type, key);
                return false;
            }
            ID3DBlob* pBlob = pShaderCode->m_blob;
            assert(pBlob);
            if( !pBlob ){
                return false;
            }

            // Get device
            ID3D11Device* pDevice = AccessInternal::device();
            assert( pDevice );
            if( !pDevice ){
                return false;
            }

            switch ( type ){
            case E_SHADER_TYPE_VERTEX:
                {
                    assert( m_pVertexShader == NULL );
                    ID3D11VertexShader* pVertexShader = NULL;
                    HRESULT result =
                        pDevice->CreateVertexShader(
                            pBlob->GetBufferPointer(),
                            pBlob->GetBufferSize(),
                            NULL, &pVertexShader);

                    if ( S_OK != result ){
                        ZH::Util::ENG_ERR("Effect::createShader() - Create vertex shader failed!\n");
                        return false;
                    }

                    m_pVertexShader = (void*)pVertexShader;
                    return true;
                }
                break;
            case E_SHADER_TYPE_HULL:
                {
                    assert( m_pHullShader == NULL );
                    ID3D11HullShader* pHullShader = NULL;
                    HRESULT result =
                        pDevice->CreateHullShader(
                            pBlob->GetBufferPointer(),
                            pBlob->GetBufferSize(),
                            NULL, &pHullShader);

                    if ( S_OK != result ){
                        ZH::Util::ENG_ERR("Effect::createShader() - Create hull shader failed!\n");
                        return false;
                    }

                    m_pHullShader = (void*)pHullShader;
                    return true;
                }
                break;
            case E_SHADER_TYPE_DOMAIN:
                {
                    assert( m_pDomainShader == NULL );
                    ID3D11DomainShader* pDomainShader = NULL;
                    HRESULT result =
                        pDevice->CreateDomainShader(
                            pBlob->GetBufferPointer(),
                            pBlob->GetBufferSize(),
                            NULL, &pDomainShader);

                    if ( S_OK != result ){
                        ZH::Util::ENG_ERR("Effect::createShader() - Create domain shader failed!\n");
                        return false;
                    }

                    m_pDomainShader = (void*)pDomainShader;
                    return true;
                }
                break;
            case E_SHADER_TYPE_GEOMETRY:
                {
                    assert( m_pGeometryShader == NULL );
                    ID3D11GeometryShader* pGeometryShader = NULL;
                    HRESULT result =
                        pDevice->CreateGeometryShader(
                            pBlob->GetBufferPointer(),
                            pBlob->GetBufferSize(),
                            NULL, &pGeometryShader);

                    if ( S_OK != result ){
                        ZH::Util::ENG_ERR("Effect::createShader() - Create geometry shader failed!\n");
                        return false;
                    }

                    m_pGeometryShader = (void*)pGeometryShader;
                    return true;
                }
                break;
            case E_SHADER_TYPE_PIXEL:
                {
                    assert( m_pPixelShader == NULL );
                    ID3D11PixelShader* pPixelShader = NULL;
                    HRESULT result =
                        pDevice->CreatePixelShader(
                            pBlob->GetBufferPointer(),
                            pBlob->GetBufferSize(),
                            NULL, &pPixelShader);

                    if ( S_OK != result ){
                        ZH::Util::ENG_ERR("Effect::createShader() - Create pixel shader failed!\n");
                        return false;
                    }

                    m_pPixelShader = (void*)pPixelShader;
                    return true;
                }
                break;
            case E_SHADER_TYPE_COMPUTE:
                {
                    assert( m_pComputeShader == NULL );
                    ID3D11ComputeShader* pComputeShader = NULL;
                    HRESULT result =
                        pDevice->CreateComputeShader(
                            pBlob->GetBufferPointer(),
                            pBlob->GetBufferSize(),
                            NULL, &pComputeShader);

                    if ( S_OK != result ){
                        ZH::Util::ENG_ERR("Effect::createShader() - Create compute shader failed!\n");
                        return false;
                    }

                    m_pComputeShader = (void*)pComputeShader;
                    return true;
                }
                break;
            default:
                {
                    assert(false);
                    ZH::Util::ENG_ERR("Effect::createShader() : Wrong shader type!\n");
                }
                break;
            } // Switch

            return false;

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

