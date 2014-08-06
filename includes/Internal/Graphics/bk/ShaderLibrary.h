#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H

#include "Internal/Graphics/Internal_common_graphics.h"
#include "Common/ZHSTD.h"
#include "Graphics/Enums.h"
#include <map>
#include <vector>
#include <string>



namespace ZH{
    namespace Graphics{

        enum E_SHADER_KEY{
            E_SHADER_KEY_INVALID,
            E_VS_POS,
            E_PS_SOLID_COLOR

        };

        enum E_SHADER_TYPE{
            E_SHADER_TYPE_NONE,
            E_SHADER_TYPE_VERTEX,
            E_SHADER_TYPE_HULL,
            E_SHADER_TYPE_DOMAIN,
            E_SHADER_TYPE_GEOMETRY,
            E_SHADER_TYPE_PIXEL,
            E_SHADER_TYPE_COMPUTE
        };

        class Semantic{
        public:
            Semantic( const std::string& name, SEMANTIC_TYPE type, unsigned short i, DXGI_FORMAT fmt );
            Semantic( const Semantic& );
            Semantic& operator=( const Semantic& );

            std::string     m_name;
            SEMANTIC_TYPE   m_type;
            unsigned short  m_index;
            DXGI_FORMAT     m_format;
        };

        class ShaderCodes{
        public:
            ShaderCodes();
            E_SHADER_KEY    m_key;
            E_SHADER_TYPE   m_type;
            std::string     m_name;
            std::string     m_codes;
            std::string     m_entry;
            std::string     m_target;
            ID3DBlob*       m_blob;
            std::vector<Semantic> m_semantics;
        };

        class ShaderLibrary{
        public:


            static ShaderLibrary& instance() { static ShaderLibrary r; return r;}

            bool initialize();
            bool clear();
            bool ready()const { return m_bInitialized; }
            const ShaderCodes* getShader( E_SHADER_TYPE, E_SHADER_KEY ) const;


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
