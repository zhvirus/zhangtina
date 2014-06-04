#ifndef NAME_H
#define NAME_H

#include "Common/ZHSTD.h"
#include <string>

namespace ZH{
    namespace Graphics{
        class ZH_GRAPHICS_DLL Name
        {
        public:
            Name( const char* const );
            virtual ~Name();
            const char* const name()const { return m_name; }

        protected:
            void makeDefaultName( const std::string& type, const std::string& key );
            char m_name[200];
        };
    } // namespace Graphics
} // nameapce ZH

#endif
