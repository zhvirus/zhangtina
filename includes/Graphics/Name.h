#ifndef NAME_H
#define NAME_H

#include "Common/ZHSTD.h"
#include <string>

namespace ZH{
    namespace Graphics{
        class ZH_GRAPHICS_DLL Name
        {
        public:
            Name();
            Name( const Name& );
            virtual ~Name();
            const std::string& name()const { return *m_name; }

        protected:
            void constructName( const std::string& type, const std::string& key );
            std::string* m_name;
        };
    } // namespace Graphics
} // nameapce ZH

#endif
