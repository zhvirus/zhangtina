#ifndef MATRIX_TESTOR_H
#define MATRIX_TESTOR_H

#include "BaseTestor.h"

namespace ZH{
    namespace TEST{

        class MatrixTestor : public BaseTestor
        {
        public:
            virtual void beginTest(const std::string&);
            virtual void endTest(const std::string&);
            virtual void doTest(const std::string&);
        private:
            void testMatrix4x4_f();
            void testMatrix4x4_d();
        };
    }
}

#endif