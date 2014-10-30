#ifndef FILE_H
#define FILE_H
#include "Common/ZHSTD.h"
#include <string>
#include <vector>

namespace ZH {
    namespace UTIL {
    
        class ZH_UTIL_DLL File{
        public:
            static bool exist(const std::wstring&);
            static bool isDir(const std::wstring&);
            static bool isFile(const std::wstring&);

            static bool collect_files(
                const std::wstring& dir, // Directory to search file from
                const std::wstring& regex, // File basename should match this reg expr
                std::vector<std::wstring>*& files, // Returned string array for searched file names
                bool recursive = false  // Search recursively ?
                );

            static std::wstring basename(const std::wstring&);
            static int fileSize(const std::wstring&);

            static bool copyFile(const std::wstring& src, const std::wstring& dst);

            static bool mkdir(const std::wstring&);

            static bool deleteFile(const std::wstring&);

            static bool getPhotoTakenTime(const std::wstring& image_name, char* oTime, const unsigned int max_char_len);


        };
    }
}



#endif
