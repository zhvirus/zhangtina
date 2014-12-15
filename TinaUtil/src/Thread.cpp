#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/Thread.h"
#include <process.h>
#include <windows.h>

namespace ZH{
    namespace UTIL{

        bool Thread::runSingleThread(ThreadFunc pFunc, void* param, bool waitToFinish)
        {
            HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, pFunc, param, 0, NULL);
            if (waitToFinish){
                WaitForSingleObject(handle, INFINITE);
            }
            return true;
        }

    }

}
