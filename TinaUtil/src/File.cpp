#ifndef _ZH_UTIL_DLL_
#define _ZH_UTIL_DLL_
#endif

#include "Util/File.h"
#include <iostream>
#include "boost/filesystem.hpp"

namespace ZH{
    namespace UTIL{

        // Exist
        bool File::exist(const std::string& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::exists(boost_file);
        }

        bool File::exist(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::exists(boost_file);
        }


        // Is directory
        bool File::isDir(const std::string& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::is_directory(boost_file);
        }

        bool File::isDir(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::is_directory(boost_file);
        }

        // Is file, not dir
        bool File::isFile(const std::string& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::is_regular(boost_file);
        }

        bool File::isFile(const std::wstring& file)
        {
            boost::filesystem::path boost_file(file);
            return boost::filesystem::is_regular(boost_file);
        }


        // Collect files
        static void collect_files_helper(
            const boost::filesystem::path& path,
            const std::string& regex,
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
                    files.push_back(it->path().c_str());
                }
            }
        }

        bool File::collect_files(
            const std::wstring& dir,
            const std::string& regex,
            std::vector<std::wstring>& files,
            bool recursive/* = false*/)
        {
            if (!exist(dir) || !isDir(dir)){
                std::wcerr << "Directory '" << dir << "' doesn't exist!" << std::endl;
                return false;
            }

            boost::filesystem::path boost_dir(dir);
            collect_files_helper(boost_dir, regex, files, recursive);
            return true;
        }


        // basename, like "aaa.txt"
        std::string File::basename(const std::string& /*file*/)
        {

            // TODO
            return "";
        }

        std::wstring File::basename(const std::wstring& /*file*/)
        {

            // TODO
            return L"";
        }


        // File size
        int File::fileSize(const std::string& file)
        {
            boost::filesystem::path boost_file(file);
            if (!boost::filesystem::is_regular_file(boost_file)){
                std::cerr << "File '" << file << "' is not a regular file, get file size failed!" << std::endl;
                return -1;
            }
            return (int)boost::filesystem::file_size(boost_file);
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


        // Copy file
        bool File::copyFile(const std::string& /*src*/, const std::string& /*dst*/)
        {

            // TODO
            return true;
        }
        bool File::copyFile(const std::wstring& /*src*/, const std::wstring& /*dst*/)
        {

            // TODO
            return true;
        }


        // Make dir
        bool File::mkdir(const std::string& /*file*/)
        {


            // TODO
            return true;
        }

        bool File::mkdir(const std::wstring& /*file*/)
        {


            // TODO
            return true;
        }


        // Delete file
        bool File::deleteFile(const std::string& /*file*/)
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



