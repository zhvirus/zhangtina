#ifndef FILE_H
#define FILE_H
#include "Common/ZHSTD.h"
#include <string>
#include <vector>

namespace ZH {
    namespace UTIL {
    
        class ZH_UTIL_DLL File{
        public:
            static bool exist(const std::string&);
            static bool exist(const std::wstring&);
            static bool isDir(const std::string&);
            static bool isDir(const std::wstring&);
            static bool isFile(const std::string&);
            static bool isFile(const std::wstring&);

            static bool collect_files(
                const std::wstring& dir, // Directory to search file from
                const std::string& regex, // File basename should match this reg expr
                std::vector<std::wstring>&, // Returned string array for searched file names
                bool recursive = false  // Search recursively ?
                );


            static std::string basename(const std::string&);
            static std::wstring basename(const std::wstring&);

            static int fileSize(const std::string&);
            static int fileSize(const std::wstring&);

            static bool copyFile(const std::string& src, const std::string& dst);
            static bool copyFile(const std::wstring& src, const std::wstring& dst);

            static bool mkdir(const std::string&);
            static bool mkdir(const std::wstring&);

            static bool deleteFile(const std::string&);
            static bool deleteFile(const std::wstring&);


        };
    }
}



#endif
