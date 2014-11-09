#ifndef EXIF_H
#define EXIF_H
#include "Common/ZHSTD.h"
#include <string>

#define MAX_EXIF_STR_LEN 1024

namespace ZH {
    namespace UTIL{
    

        class ZH_UTIL_DLL ExifData{
        public:
            void reset();
            unsigned int year;
            unsigned int month;
            unsigned int day;
        };


        class ZH_UTIL_DLL Exif{
        public:
            static bool getExifData(ExifData&, const std::wstring&);





        };
    }
}

#endif
