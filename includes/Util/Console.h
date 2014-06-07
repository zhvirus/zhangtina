#ifndef CONSOLE_H
#define CONSOLE_H
#include "Common/ZHSTD.h"
#include "windows.h"
#include <string>

namespace ZH
{
    namespace Util{

        class ZH_UTIL_DLL Console
        {
        public:
            static Console& instance() { static Console t; return t;}
            void cout(const char* const pre_tag, const char* const msg);
            void cerr(const char* const pre_tag, const char* const msg);
            void cout(const char* const str );
            void cerr(const char* const str );
            void cout(const std::string& str);
            void cerr(const std::string& str);

        private:
            Console();
            ~Console();
            HANDLE m_hConsoleOutput;
            HANDLE m_hConsoleOutputErr;


        };


    }
}

#endif
