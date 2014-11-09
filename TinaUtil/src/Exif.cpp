#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/Exif.h"
#include <boost/regex.hpp>

#pragma warning(disable:4512)
#include "exiv2/exiv2.hpp"

namespace ZH{
    namespace UTIL{

        void ExifData::reset()
        {
            year  = 0;
            month = 0;
            day   = 0;
        }

        bool Exif::getExifData(ExifData& data, const std::wstring& image_file)
        {
            data.reset();

            Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(image_file.c_str());
            if (image.get() == 0){
                return false;
            }
            image->readMetadata();

            Exiv2::ExifData &exifData = image->exifData();
            if (exifData.empty()) {
                return false;
            }

            std::string time("");

            Exiv2::ExifData::const_iterator end = exifData.end();
            for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
                if (i->key() != "Exif.Photo.DateTimeOriginal"){
                    continue;
                }

                time = i->value().toString();
                break;
            }

            if (time == ""){
                return false;
            }

            // Now we got the photo take time from Exif, then parse
            // the year/month/day from it.
            boost::regex reg("(\\d\\d\\d\\d)\\:0*([0-9]+)\\:0*([0-9]+)",
                boost::regex::icase | boost::regex::perl);
            boost::smatch m;
            if (!boost::regex_search(time, m, reg)){
                return false;
            }

            std::string year  = m[1].str();
            std::string month = m[2].str();
            std::string day   = m[3].str();

            data.year  = atoi(year.c_str());
            data.month = atoi(month.c_str());
            data.day   = atoi(day.c_str());

            if (data.year < 1982 || data.year > 3000 ||
                data.month < 1 || data.month > 12 ||
                data.day < 1 || data.day > 31){

                return false;
            }

            return true;
        }


    }
}



