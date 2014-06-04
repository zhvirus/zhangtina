#ifndef CONSOLE_H
#define CONSOLE_H
#include "Common/ZHSTD.h"
#include "windows.h"

namespace ZH
{
    namespace Util{

        class ZH_UTIL_DLL Console
        {
        public:
            static Console& instance() { static Console t; return t;}
            void cout(const char* const tag, const char* const fmt, ...);
            void cerr(const char* const tag, const char* const fmt, ...);


        private:
            Console();
            ~Console();
            HANDLE m_hConsoleOutput;
            HANDLE m_hConsoleOutputErr;


        };


    }
}

#endif
