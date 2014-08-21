#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/Debug.h"

namespace ZH{
    namespace Graphics{

        void Debug::dump_gl_errors()
        {
            GLenum err;
            while( (err=glGetError()) != GL_NO_ERROR ){
                ZH::Util::ENG_ERR((const char* const)glewGetErrorString(err));
            };
        }

    }
}


