#ifndef MODULE_FREE_H
#define MODULE_FREE_H
#include "Common/ZHSTD.h"
#include <vector>
#include <string>

namespace ZH {
    namespace UTIL {

        template<class T>
        void FreeVector(T*& t);

        ZH_UTIL_EXTERN template ZH_UTIL_DLL void FreeVector(std::vector<std::wstring>*& t);


    }
}
#endif
