#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/File.h"
#include <iostream>
#include "boost/filesystem.hpp"
#include <boost/regex.hpp>

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

        std::wstring File::basename(const std::wstring& /*file*/)
        {

            // TODO
            return L"";
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

        bool File::copyFile(const std::wstring& /*src*/, const std::wstring& /*dst*/)
        {

            // TODO
            return true;
        }

        bool File::mkdir(const std::wstring& /*file*/)
        {


            // TODO
            return true;
        }


        bool File::deleteFile(const std::wstring& /*file*/)
        {



            // TODO
            return true;
        }

    }
}



