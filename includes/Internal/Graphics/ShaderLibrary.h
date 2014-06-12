#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H

#include "Internal/Graphics/Internal_common_graphics.h"
#include "Common/ZHSTD.h"
#include <map>
#include <string>


namespace ZH{
    namespace Graphics{

        enum E_SHADER_KEY{
            E_SHADER_KEY_INVALID,
            E_VS_POS,
            E_PS_SOLID_COLOR

        };

        class ShaderCodes{
        public:
            ShaderCodes();
            E_SHADER_KEY    m_key;
            std::string     m_name;
            std::string     m_codes;
            std::string     m_entry;
            std::string     m_target;
            ID3DBlob*       m_blob;
        };

        class ShaderLibrary{
        public:


            static ShaderLibrary& instance() { static ShaderLibrary r; return r;}

            bool initialize();
            bool clear();


            ID3DBlob* compileShader(
                const char* const name,
                const char* const codes,
                const char* const entryPoint,
                const char* const target);


        private:
            void buildShaderMaps();
            void compileShaderMap( std::map<E_SHADER_KEY, ShaderCodes*>& );
            void clearShaderMap( std::map<E_SHADER_KEY, ShaderCodes*>& );
            std::map<E_SHADER_KEY, ShaderCodes*> m_vsMap;
            std::map<E_SHADER_KEY, ShaderCodes*> m_hsMap;
            std::map<E_SHADER_KEY, ShaderCodes*> m_dsMap;
            std::map<E_SHADER_KEY, ShaderCodes*> m_gsMap;
            std::map<E_SHADER_KEY, ShaderCodes*> m_psMap;
            std::map<E_SHADER_KEY, ShaderCodes*> m_csMap;

            bool m_bInitialized;
        private:
            ShaderLibrary();
            ~ShaderLibrary();
            ShaderLibrary( const ShaderLibrary& ){}
            ShaderLibrary& operator = ( const ShaderLibrary& ) { return *this; }
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };
    }
}
#endif
