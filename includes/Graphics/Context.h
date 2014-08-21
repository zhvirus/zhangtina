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


        private:
            ShaderMap*   m_pShaders;
            ProgramMap*  m_pPrograms;

        private:
            bool addShader( Shader* );
            bool addProgram( Program* );

            bool removeShader( Shader* );
            bool removeProgram( Program* );

        protected:
            Context();
            CLASS_COMMON_PROTECTED_DECLEARATION(Context);
            friend class ResourceManager;


        };
    }
}


#endif
