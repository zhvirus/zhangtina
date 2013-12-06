#ifndef NAME_H
#define NAME_H

#include "Common/ZHSTD.h"
#include <string>

namespace ZH{
    namespace Graphics{
        class ZH_DLL Name
        {
        public:
            Name();
            virtual ~Name();
            const std::string& name()const;
            void name( const std::string& n );

        private:
            std::string* m_name;
        };
    } // namespace Graphics
} // nameapce ZH

#endif
