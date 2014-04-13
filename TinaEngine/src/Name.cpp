#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Graphics/Name.h"
#include <string>

namespace ZH{
    namespace Graphics{

        Name::Name():m_name( NULL ){
            m_name = new std::string( "" );
        }

        Name::Name( const Name& v ){
            m_name = new std::string( *v.m_name );
        }

        Name::~Name(){
            delete m_name;
        }

        void Name::constructName( const std::string& type, const std::string& key )
        {
            *m_name = type;
            *m_name += "_";
            *m_name += key;
            *m_name += "_";

            char uniqueTrail[50];
            sprintf_s(uniqueTrail,"%llx", this);
            *m_name += uniqueTrail;
        }

    }
}
