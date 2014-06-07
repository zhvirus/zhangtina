#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/Print.h"
#include "Util/Console.h"
#include <stdarg.h>
#include <iostream>
#include <assert.h>

#define MAX_PRINT_BUF_SIZE 0x1000
static  char buffer[MAX_PRINT_BUF_SIZE];

#define PRINT_IMP( TAG ) \
        va_list args; \
        va_start(args, fmt); \
        int ret = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args); \
        va_end(args); \
        assert(ret != -1); \
        Console::instance().cout(TAG, buffer);

#define PRINT_ERR_IMP( TAG ) \
        va_list args; \
        va_start(args, fmt); \
        int ret = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args); \
        va_end(args); \
        assert(ret != -1); \
        Console::instance().cerr(TAG, buffer);

namespace ZH{
    namespace Util{

        // TinaX print
        void TNX_INF( const char* const fmt, ... )
        {
            PRINT_IMP("[TinaX]:  ")
        }
        void TNX_ERR( const char* const fmt, ... )
        {
            PRINT_ERR_IMP("[TinaX]: ERROR: ")
        }
        void TNX_DBG( const char* const fmt, ... )
        {
            PRINT_IMP("[TinaX]:  ")
        }

        // Engine print
        void ENG_INF( const char* const fmt, ... )
        {
            PRINT_IMP("[Engine]: ")
        }
        void ENG_ERR( const char* const fmt, ... )
        {
            PRINT_ERR_IMP("[Engine]: ERROR: ")
        }
        void ENG_DBG( const char* const fmt, ... )
        {
            PRINT_IMP("[Engine]: ")
        }

        // Math print
        void MTH_INF( const char* const fmt, ... )
        {
            PRINT_IMP("[Math]:   ")
        }
        void MTH_ERR( const char* const fmt, ... )
        {
            PRINT_ERR_IMP("[Math]: ERROR: ")
        }
        void MTH_DBG( const char* const fmt, ... )
        {
            PRINT_IMP("[Math]:   ")
        }

        // Wiget print
        void WGT_INF( const char* const fmt, ... )
        {
            PRINT_IMP("[Wiget]:  ")
        }
        void WGT_ERR( const char* const fmt, ... )
        {
            PRINT_ERR_IMP("[Wiget]: ERROR: ")
        }
        void WGT_DBG( const char* const fmt, ... )
        {
            PRINT_IMP("[Wiget]:  ")
        }

    }
}

