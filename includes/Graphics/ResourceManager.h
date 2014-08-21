#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"
#include "Graphics/Shader.h"

namespace ZH{
    namespace Graphics{

        class Context;

        class ZH_GRAPHICS_DLL ResourceManager
        {
        public:
            static Shader* createShader( Context* ctx, SHADER_TYPE type, const char* const pSrc );
            static Program* createProgram( Context* ctx );


            static bool destroyShader( Shader* );
            static bool destroyProgram( Program* );


        private:


        };



    }
}
#endif
