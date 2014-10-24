#ifndef SHADER_H
#define SHADER_H

#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"
#include "Graphics/Resource.h"
#include <string>
#include <map>

namespace ZH{
    namespace Graphics{

        class Context;

        enum SHADER_TYPE{
            E_UNKNOWN_SHADER,
            E_VERTEX_SHADER,
            E_HULL_SHADER,
            E_DOMAIN_SHADER,
            E_GEOMETRY_SHADER,
            E_FRAGMENT_SHADER,
            E_COMPUTE_SHADER
        };

        class ZH_GRAPHICS_DLL Shader : public Resource
        {
            CLASS_IDENTIFIER(E_CID_SHADER);
        public:
            bool valid() const;
            bool compile();
            SHADER_TYPE type() const { return m_type; }

        private:
            SHADER_TYPE  m_type;
            std::string* m_pCode;
            bool         m_compiled;

        protected:
            Shader( Context* ctx, SHADER_TYPE type, const char* const pSrc );
            CLASS_COMMON_PROTECTED_DECLEARATION(Shader);
            friend class Context;
        };

        typedef std::map<unsigned int, Shader*> ShaderMap;

        class ZH_GRAPHICS_DLL Program : public Resource
        {
            CLASS_IDENTIFIER(E_CID_PROGRAM);
        public:
            bool setVertexShader( Shader* );
            bool setHullShader( Shader* );
            bool setDomainShader( Shader* );
            bool setGeometryShader( Shader* );
            bool setFragmentShader( Shader* );
            bool setComputeShader( Shader* );

            bool link();

        private:
            bool setShaderCommon( Shader*, SHADER_TYPE);

        private:
            bool         m_linked;
            std::map<SHADER_TYPE, Shader*> * m_pShaders;


        protected:
            Program( Context* ctx );
            CLASS_COMMON_PROTECTED_DECLEARATION(Program);
            friend class Context;
        };

        typedef std::map<unsigned int, Program*> ProgramMap;


    }
}
#endif
