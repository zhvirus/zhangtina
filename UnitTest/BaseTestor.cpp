#include "BaseTestor.h"
#include <iostream>

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

        void BaseTestor::printResult(bool b, const std::string& s)
        {
            if ( b ){
                std::cout<<"["<<s<<"]"<<"   ......   PASSED."<<std::endl;
            }else{
                std::cout<<"'"<<s<<"'"<<"   ......   FAILED=========>!!"<<std::endl;
            }
        }




    }



}

