#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Context.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Graphics/Shader.h"


namespace ZH{
    namespace Graphics{


        Context::Context():
            m_pShaders(NULL),
            m_pPrograms(NULL)
        {
            m_pShaders  = new ShaderMap;
            m_pPrograms = new ProgramMap;
        }


        Context::~Context()
        {
            // Release all resources
            releaseAll();
        }


        template<class S, class T>
        bool Context::releaseMapPtr( S*& pResMap )
        {
            S::iterator it = pResMap->begin();
            for(; it != pResMap->end(); ++it){
                T* pRes = it->second;
                if ( pRes ){
                    delete pRes;
                }
            }

            delete pResMap;
            pResMap = NULL;
            return true;
        }


        void Context::releaseAll()
        {
            // Programs
            releaseMapPtr<ProgramMap, Program>( m_pPrograms );

            // Shaders
            releaseMapPtr<ShaderMap, Shader>( m_pShaders );
        }


        Shader* Context::createShader( SHADER_TYPE type, const char* const pSrc )
        {
            ASSERT_NOT_NULL_RET_NULL(pSrc);

            Shader* pShader = new Shader( this, type, pSrc );
            if ( !addShader(pShader) ){
                delete pShader;
                return NULL;
            }

            return pShader;
        }

        Program* Context::createProgram()
        {
            Program* pProg = new Program( this );
            if ( !addProgram(pProg) ){
                delete pProg;
                return NULL;
            }

            return pProg;
        }


        bool Context::destroyShader( Shader* shader )
        {
            ASSERT_NOT_NULL_RET_FALSE(shader);

            removeShader( shader );

            delete shader;
            return true;
        }

        bool Context::destroyProgram( Program* prog )
        {
            ASSERT_NOT_NULL_RET_FALSE(prog);

            removeProgram( prog );

            delete prog;
            return true;
        }

        // Add
        template<class S, class T>
        static bool addCommon( S* pResMap, T* pRes )
        {
            ASSERT_NOT_NULL_RET_FALSE( pResMap );
            ASSERT_NOT_NULL_RET_FALSE( pRes );

            if ( pResMap->find( pRes->name() ) != pResMap->end() ){
                assert(false);
                return false;
            }

            pResMap->insert(std::make_pair(pRes->name(), pRes));
            return true;
        }

        bool Context::addShader( Shader* pShader )
        {
            return addCommon<ShaderMap, Shader>( m_pShaders, pShader );
        }

        bool Context::addProgram( Program* pProgram )
        {
            return addCommon<ProgramMap, Program>( m_pPrograms, pProgram );
        }

        // Remove
        template<class S, class T>
        static bool removeCommon( S* pResMap, T* pRes )
        {
            ASSERT_NOT_NULL_RET_FALSE( pResMap );
            ASSERT_NOT_NULL_RET_FALSE( pRes );

            S::iterator it = pResMap->find( pRes->name() );

            if ( it == pResMap->end() ){
                assert(false);
                return false;
            }

            pResMap->erase(it);

            return true;
        }

        bool Context::removeProgram( Program* pProgram )
        {
            return removeCommon<ProgramMap, Program>( m_pPrograms, pProgram );
        }

        bool Context::removeShader( Shader* pShader )
        {
            return removeCommon<ShaderMap, Shader>( m_pShaders, pShader );
        }

    }
}