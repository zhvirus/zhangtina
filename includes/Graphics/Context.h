#ifndef CONTEXT_H
#define CONTEXT_H
#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"
#include <map>

#include "Graphics/Shader.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL Context
        {
        public:
            Shader*  createShader( SHADER_TYPE type, const char* const pSrc );
            Program* createProgram();


            bool destroyShader( Shader* );
            bool destroyProgram( Program* );

        private:
            ShaderMap*   m_pShaders;
            ProgramMap*  m_pPrograms;

        private:
            template<class S, class T>
            bool releaseMapPtr( S*& );

            bool addShader( Shader* );
            bool addProgram( Program* );

            bool removeShader( Shader* );
            bool removeProgram( Program* );

            void releaseAll();

        protected:
            Context();
            CLASS_COMMON_PROTECTED_DECLEARATION(Context);
            friend class ResourceManager;


        };
    }
}


#endif
