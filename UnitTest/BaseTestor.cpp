#include "BaseTestor.h"
#include <iostream>
#include <stdio.h>

namespace ZH{
    namespace TEST{

        void BaseTestor::beginTest(const std::string& s)
        {
            std::cout<<"--------------------------------------------"<<std::endl;
            std::cout<<std::endl<<"Start <"<<s.c_str()<<"> test..."<<std::endl;
        }
        void BaseTestor::endTest(const std::string& s)
        {
            std::cout<<std::endl<<"End <"<<s.c_str()<<"> test..."<<std::endl;
            std::cout<<"--------------------------------------------"<<std::endl;
        }
        void BaseTestor::doTest(const std::string& s)
        {
            std::cout<<std::endl<<"Doing <"<<s.c_str()<<"> test..."<<std::endl;

        }

        void BaseTestor::printResult(bool b, const unsigned int index)
        {
            char index_str[50];
            sprintf_s(index_str,50,"%d",index);

            if ( b ){
                std::cout<<"["<<index_str<<"]"<<"   ......   PASSED."<<std::endl;
            }else{
                std::cout<<"["<<index_str<<"]"<<"   ......   FAILED========>!!!."<<std::endl;
            }
        }




    }



}

