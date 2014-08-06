#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Shader.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{

        Shader::Shader(SHADER_TYPE type, const char* const pSrc, unsigned int len):
            m_name(0),
            m_type(type),
            m_pCode(NULL),
            m_compiled(false)
        {
            assert(pSrc);
            assert(len>0);

            GLenum shader_type = GL_VERTEX_SHADER;
            switch (type){
            case E_VERTEX_SHADER:
                shader_type = GL_VERTEX_SHADER;
                break;
            case E_FRAGMENT_SHADER:
                shader_type = GL_FRAGMENT_SHADER;
                break;
            default:
                assert(false);
                ZH::Util::ENG_ERR("Unsupported shader type %d!", type);
                return;
            };


            glCreateShader(GL_GEOMETRY_SHADER);

        }









    }
}

#endif
