#ifndef VECTOR_TESTOR_H
#define VECTOR_TESTOR_H

#include "BaseTestor.h"

namespace ZH{
    namespace TEST{

        class VectorTestor : public BaseTestor
        {
        public:
            virtual void beginTest(const std::string&);
            virtual void endTest(const std::string&);
            virtual void doTest(const std::string&);
        private:
            void testVector2();
            void testVector3();
            void testVector4();


        };




    }
}

#endif