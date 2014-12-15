#ifndef THREAD_H
#define THREAD_H

#include "Common/ZHSTD.h"

namespace ZH{
    namespace UTIL{

        typedef unsigned int(__stdcall *ThreadFunc)(void*);

        class ZH_UTIL_DLL Thread
        {
        public:
            static bool runSingleThread(ThreadFunc pFunc, void* param, bool waitToFinish);



        };


    }
}



#endif
