#ifndef PRINT_H
#define PRINT_H
#include "Common/ZHSTD.h"

namespace ZH
{
    namespace Util{

        ZH_UTIL_DLL void TNX_INF( const char* const, ... );
        ZH_UTIL_DLL void TNX_ERR( const char* const, ... );
        ZH_UTIL_DLL void TNX_DBG( const char* const, ... );

        ZH_UTIL_DLL void ENG_INF( const char* const, ... );
        ZH_UTIL_DLL void ENG_ERR( const char* const, ... );
        ZH_UTIL_DLL void ENG_DBG( const char* const, ... );

        ZH_UTIL_DLL void WGT_INF( const char* const, ... );
        ZH_UTIL_DLL void WGT_ERR( const char* const, ... );
        ZH_UTIL_DLL void WGT_DBG( const char* const, ... );

        ZH_UTIL_DLL void MTH_INF( const char* const, ... );
        ZH_UTIL_DLL void MTH_ERR( const char* const, ... );
        ZH_UTIL_DLL void MTH_DBG( const char* const, ... );


    }
}




#endif


