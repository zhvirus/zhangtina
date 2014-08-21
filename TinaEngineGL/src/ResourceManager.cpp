#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/ResourceManager.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/Debug.h"
#include "Graphics/Context.h"


namespace ZH{
    namespace Graphics{

        Shader* ResourceManager::createShader( Context* ctx, SHADER_TYPE type, const char* const pSrc )
        {
            ASSERT_NOT_NULL_RET_NULL(ctx);

            Shader* pShader = new Shader( ctx, type, pSrc );
            if ( !ctx->addShader(pShader) ){
                delete pShader;
                return NULL;
            }

            return pShader;
        }

        Program* ResourceManager::createProgram( Context* ctx )
        {
            ASSERT_NOT_NULL_RET_NULL(ctx);

            Program* pProg = new Program( ctx );
            if ( !ctx->addProgram(pProg) ){
                delete pProg;
                return NULL;
            }

            return pProg;
        }


        bool ResourceManager::destroyShader( Shader* shader )
        {
            ASSERT_NOT_NULL_RET_FALSE(shader);

            Context* pCtx = shader->context();
            ASSERT_NOT_NULL_RET_FALSE(pCtx);

            pCtx->removeShader( shader );

            delete shader;
            return true;
        }

        bool ResourceManager::destroyProgram( Program* prog )
        {
            ASSERT_NOT_NULL_RET_FALSE(prog);

            Context* pCtx = prog->context();
            ASSERT_NOT_NULL_RET_FALSE(pCtx);

            pCtx->removeProgram( prog );

            delete prog;
            return true;
        }


    }
}

