#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/Print.h"
#include <stdarg.h>
#include <iostream>
#include <assert.h>

#define MAX_PRINT_BUF_SIZE 0x1000

#define PRINT_IMP \
        va_list args; \
        va_start(args, fmt); \
        char buffer[MAX_PRINT_BUF_SIZE]; \
        int ret = vsnprintf_s(buffer, MAX_PRINT_BUF_SIZE, _TRUNCATE, fmt, args); \
        va_end(args); \
        assert(ret != -1);


namespace ZH{
    namespace Util{

        // TinaX print
        void TNX_INF( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cout<<"[TinaX]:  "<<buffer;
        }
        void TNX_ERR( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[TinaX]: ERROR: "<<buffer;
        }
        void TNX_DBG( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[TinaX]:  "<<buffer;
        }

        // Engine print
        void ENG_INF( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cout<<"[Engine]: "<<buffer;
        }
        void ENG_ERR( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[Engine]: ERROR: "<<buffer;
        }
        void ENG_DBG( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[Engine]: "<<buffer;
        }

        // Math print
        void MTH_INF( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cout<<"[Math]:   "<<buffer;
        }
        void MTH_ERR( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[Math]: ERROR: "<<buffer;
        }
        void MTH_DBG( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[Math]:   "<<buffer;
        }

        // Wiget print
        void WGT_INF( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cout<<"[Wiget]:  "<<buffer;
        }
        void WGT_ERR( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[Wiget]: ERROR: "<<buffer;
        }
        void WGT_DBG( const char* const fmt, ... )
        {
            PRINT_IMP
            std::cerr<<"[Wiget]:  "<<buffer;
        }

    }
}

