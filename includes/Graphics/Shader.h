#ifndef SHADER_H
#define SHADER_H

#include "Common\ZHSTD.h"
#include "Graphics/ClassIdentifier.h"
#include <string>

namespace ZH{
    namespace Graphics{

        enum SHADER_TYPE{
            E_UNKNOWN_SHADER,
            E_VERTEX_SHADER,
            E_HULL_SHADER,
            E_DOMAIN_SHADER,
            E_GEOMETRY_SHADER,
            E_FRAGMENT_SHADER
        };

        class ZH_GRAPHICS_DLL Shader
        {
            CLASS_IDENTIFIER(E_CID_SHADER);
        public:
            Shader(SHADER_TYPE type, const char* const pSrc, unsigned int len);
            bool valid() const;
            bool compile();


        private:
            unsigned int m_name;
            SHADER_TYPE  m_type;
            std::string* m_pCode;
            bool         m_compiled;
        };
    }
}
#endif
