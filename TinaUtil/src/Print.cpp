#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/Print.h"
#include <stdarg.h>
#include <iostream>
#include <assert.h>

#define MAX_PRINT_BUF_SIZE 0x1000

namespace ZH{
    namespace Util{

        void INF( const char* const fmt, ... )
        {
            va_list args;
            va_start(args, fmt);
            char buffer[MAX_PRINT_BUF_SIZE];
            int ret = vsnprintf_s(buffer, MAX_PRINT_BUF_SIZE, _TRUNCATE, fmt, args);
            va_end(args);
            assert(ret != -1);
            std::cout<<"[INF]: "<<buffer;
        }

        void ERR( const char* const fmt, ... )
        {
            va_list args;
            va_start(args, fmt);
            char buffer[MAX_PRINT_BUF_SIZE];
            int ret = vsnprintf_s(buffer, MAX_PRINT_BUF_SIZE, _TRUNCATE, fmt, args);
            va_end(args);
            assert(ret != -1);
            std::cerr<<"[ERROR]: "<<buffer;
        }

    }
}

