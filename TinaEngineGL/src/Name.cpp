#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Graphics/Name.h"
#include "Internal/Graphics/Internal_common_graphics.h"

#include <string>
#include <assert.h>

namespace ZH{
    namespace Graphics{

        Name::Name( const char* const name ){
            assert( name );
            if ( name ){
                strcpy_s( m_name, name);
            }
        }

        Name::~Name(){
        }

        void Name::makeDefaultName( const std::string& type, const std::string& key )
        {
            std::string temp_name = type;
            temp_name += "_";
            temp_name += key;
            temp_name += "_";

            char uniqueTrail[50];
            sprintf_s(uniqueTrail,"%llx", this);
            temp_name += uniqueTrail;

            strcpy_s( m_name, temp_name.c_str() );
        }

        bool Name::nameEqual( const char* const name ) const
        {
            if ( !name || (strcmp(name, m_name) != 0) ){
                return false;
            }

            return true;
        }


    }
}
