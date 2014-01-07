#ifndef BASE_TESTOR_H
#define BASE_TESTOR_H
#include <string>

namespace ZH{
    namespace TEST{

        class BaseTestor{
        public:
            virtual void beginTest(const std::string&);
            virtual void endTest(const std::string&);
            virtual void doTest(const std::string&);

            void printResult(bool, const unsigned int);
        };




    }
}

#endif
