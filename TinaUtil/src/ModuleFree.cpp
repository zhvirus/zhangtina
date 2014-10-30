#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/ModuleFree.h"


namespace ZH{
    namespace UTIL{

        template<class T>
        void FreeVector(T*& t){
            delete t;
            t = NULL;
        }
    }
}

