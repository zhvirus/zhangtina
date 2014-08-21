#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Context.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Graphics/ResourceManager.h"


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
            // Delete maps
            SAFE_DELETE( m_pShaders );
            SAFE_DELETE( m_pPrograms );

            // Release context
        }

        void Context::releaseAllShaders()
        {
            // Programs
            if ( m_pPrograms ){
                ProgramMap::iterator it = m_pPrograms->begin();
                for(; it != m_pPrograms->end(); ++it){
                    Program* prog = it->second;
                    ResourceManager::destroyProgram( prog );
                }
                m_pPrograms->clear();
            }

            // Shaders
            if ( m_pShaders ){
                ShaderMap::iterator it = m_pShaders->begin();
                for(; it != m_pShaders->end(); ++it){
                    Shader* shader = it->second;
                    ResourceManager::destroyShader( shader );
                }
                m_pShaders->clear();
            }
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


    }
}