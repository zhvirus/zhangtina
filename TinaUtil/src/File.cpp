#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/File.h"
#include "Util/Exif.h"
#include <iostream>
#include "boost/filesystem.hpp"
#include <boost/regex.hpp>
#include <fstream>
#include <string>

namespace ZH{
    namespace UTIL{

        bool File::exist(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::exists(boost_file);
        }

        bool File::isDir(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::is_directory(boost_file);
        }

        bool File::isFile(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::is_regular(boost_file);
        }


        // Collect files
        static void collect_files_helper(
            const boost::filesystem::path& path,
            const boost::wregex& regex,
            std::vector<std::wstring>& files,
            bool recursive )
        {
            boost::filesystem::directory_iterator it(path);
            boost::filesystem::directory_iterator end_it;
            for (; it != end_it; ++it){

                if (recursive && boost::filesystem::is_directory(*it)){
                    collect_files_helper(it->path(), regex, files, recursive);
                }

                if (boost::filesystem::is_regular_file(*it)){
                    std::wstring file_name(it->path().c_str());
                    if (boost::regex_search(file_name, regex)){
                        files.push_back(file_name);
                    }
                }
            }
        }

        bool File::collect_files(
            const std::wstring& dir,
            const std::wstring& regex,
            std::vector<std::wstring>*& files,
            bool recursive/* = false*/)
        {
            if (!exist(dir) || !isDir(dir)){
                std::wcerr << "Directory '" << dir << "' doesn't exist!" << std::endl;
                return false;
            }

            files = new std::vector<std::wstring>();

            boost::filesystem::path boost_dir(dir);
            boost::wregex regex_boost(regex, boost::regex::perl | boost::regex::icase);
            collect_files_helper(boost_dir, regex_boost, *files, recursive);
            return true;
        }

        void File::basename(const std::wstring& file, wchar_t* name)
        {
            boost::wregex reg(L"([^\\\\\\\/]+)$",
                boost::wregex::icase | boost::regex::perl);

            boost::wsmatch m;
            if (!boost::regex_search(file, m, reg)){
                assert(false);
                return;
            }

            wcsncpy_s(name, 4096, m[1].str().c_str(), m[1].length());
        }

        int File::fileSize(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            if (!boost::filesystem::is_regular_file(boost_file)){
                std::wcerr << "File '" << file << "' is not a regular file, get file size failed!" << std::endl;
                return -1;
            }
            return (int)boost::filesystem::file_size(boost_file);
        }

        bool File::copyFile(const std::wstring& src, const std::wstring& dst)
        {
            if (!exist(src))
            {
                return false;
            }

            if (exist(dst))
            {
                return false;
            }

            boost::filesystem::copy_file(src, dst);

            return exist(dst);
        }

        bool File::mkdir(const std::wstring& dir)
        {
            if (exist(dir)){
                return true;
            }

            boost::filesystem::create_directories(dir);

            return exist(dir);
        }


        bool File::deleteFile(const std::wstring& file)
        {
            if (!exist(file))
            {
                return false;
            }

            boost::filesystem::remove_all(file);

            return !exist(file);
        }

        bool File::getPhotoTakenTime(
            const std::wstring& image_name, unsigned int& y, unsigned int& m, unsigned int& d)
        {
            // Reset year/month/date
            y = 0;
            m = 0;
            d = 0;

            // Check file name
            boost::wregex reg(L"\.(jpg|jpeg|JPG|JPEG)$");
            if (!boost::regex_search(image_name, reg)){
                return false;
            }

            // Check existence
            if (!exist(image_name)){
                return false;
            }

            ExifData exifData;
            if (!Exif::getExifData(exifData, image_name)){
                return false;
            }

            y = exifData.year;
            m = exifData.month;
            d = exifData.day;

            return true;
        }


        bool File::getLastWriteTime(const std::wstring& file, unsigned int& y, unsigned int& m, unsigned int& d)
        {
            std::time_t time = boost::filesystem::last_write_time(file);
            struct tm tm;
            localtime_s(&tm, &time);

            y = tm.tm_year+1900;
            m = tm.tm_mon+1;
            d = tm.tm_mday;

            return true;
        }

    }
}



