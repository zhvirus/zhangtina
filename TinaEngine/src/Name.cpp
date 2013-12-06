#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif

#include "Graphics/Name.h"
#include <string>

namespace ZH{
    namespace Graphics{
        Name::Name():m_name(NULL){
            m_name = new std::string();
        }
        Name::~Name(){
            delete m_name;
        }

        const std::string& Name::name()const
        {
            return *m_name;
        }

        void Name::name( const std::string& n )
        {
            *m_name = n;
        }

    }
}
