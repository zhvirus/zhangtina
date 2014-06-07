#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/Console.h"
#include <stdarg.h>
#include <iostream>

#define MAX_PRINT_BUF_SIZE 0x1000

namespace ZH
{
    namespace Util{
        Console::Console():
            m_hConsoleOutput(NULL),
            m_hConsoleOutputErr(NULL)
        {
            ::AllocConsole();

            SetConsoleTitle("TinaX-cmd");

            HANDLE conBuffer = CreateFile(
                "CONOUT$",
                GENERIC_WRITE|GENERIC_READ,
                FILE_SHARE_WRITE ,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
                );

            COORD con_buffer_size= {110,32766};
            BOOL result = SetConsoleScreenBufferSize( conBuffer, con_buffer_size);
            SMALL_RECT s_rect={0,0,109,50};
            result = SetConsoleWindowInfo( conBuffer, true, &s_rect);
            result = SetConsoleTextAttribute( conBuffer, FOREGROUND_BLUE | FOREGROUND_GREEN );

            m_hConsoleOutput    = GetStdHandle(STD_OUTPUT_HANDLE);
            m_hConsoleOutputErr = GetStdHandle(STD_ERROR_HANDLE);
            char str[] = "Start Console Log...\n";
            WriteConsole(m_hConsoleOutput, str, (DWORD)strlen(str),NULL, NULL);
        }

        Console::~Console()
        {
            char str[] = "Destroy console...\n";
            WriteConsole(m_hConsoleOutput, str, (DWORD)strlen(str),NULL, NULL);
            CloseHandle(m_hConsoleOutput);
            CloseHandle(m_hConsoleOutputErr);
            ::FreeConsole();
        }

        void Console::cout(const char* const pre_tag, const char* const msg)
        {
            char buffer[MAX_PRINT_BUF_SIZE] = {0};
            sprintf_s(buffer, MAX_PRINT_BUF_SIZE, "%s%s", pre_tag, msg);
            WriteConsole(m_hConsoleOutput, buffer, (DWORD)strlen(buffer), NULL, NULL);
        }

        void Console::cerr(const char* const pre_tag, const char* const msg)
        {
            char buffer[MAX_PRINT_BUF_SIZE] = {0};
            sprintf_s(buffer, MAX_PRINT_BUF_SIZE, "%s%s", pre_tag, msg);
            WriteConsole(m_hConsoleOutputErr, buffer, (DWORD)strlen(buffer), NULL, NULL);
        }

        void Console::cout( const char* const str )
        {
            if ( str ){
                WriteConsole(m_hConsoleOutput, str, (DWORD)strlen(str), NULL, NULL);
            }
        }

        void Console::cerr( const char* const str )
        {
            if ( str ){
                WriteConsole(m_hConsoleOutputErr, str, (DWORD)strlen(str), NULL, NULL);
            }
        }

        void Console::cout( const std::string& str )
        {
            WriteConsole(m_hConsoleOutput, str.c_str(), (DWORD)str.length(), NULL, NULL);
        }

        void Console::cerr( const std::string& str )
        {
            WriteConsole(m_hConsoleOutputErr, str.c_str(), (DWORD)str.length(), NULL, NULL);
        }



    }
}




