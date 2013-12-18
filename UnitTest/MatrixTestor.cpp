#include "MatrixTestor.h"
#include <iostream>
#include "Math/Matrix.h"
#include "Math/MathCommon.h"

using namespace ZH::Math;

namespace ZH{
    namespace TEST{

        void MatrixTestor::beginTest(const std::string& s)
        {
            BaseTestor::beginTest( s );

            // TODO
        }
        void MatrixTestor::endTest(const std::string& s)
        {
            //TODO


            BaseTestor::endTest(s);
        }
        void MatrixTestor::doTest(const std::string& s)
        {
            BaseTestor::doTest(s);

            testMatrix();
        }

        void MatrixTestor::testMatrix()
        {
            std::cout<<"Testing 'Matrix'..."<<std::endl;

            // Vector2();
            {
                matrix_f a;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],1) && VALUE_EQUAL<float>(a.v[1],0) && VALUE_EQUAL<float>(a.v[2],0) && VALUE_EQUAL<float>(a.v[3],0) &&
                    VALUE_EQUAL<float>(a.v[4],0) && VALUE_EQUAL<float>(a.v[5],1) && VALUE_EQUAL<float>(a.v[6],0) && VALUE_EQUAL<float>(a.v[7],0) &&
                    VALUE_EQUAL<float>(a.v[8],0) && VALUE_EQUAL<float>(a.v[9],0) && VALUE_EQUAL<float>(a.v[10],1) && VALUE_EQUAL<float>(a.v[11],0) &&
                    VALUE_EQUAL<float>(a.v[12],0) && VALUE_EQUAL<float>(a.v[13],0) && VALUE_EQUAL<float>(a.v[14],0) && VALUE_EQUAL<float>(a.v[15],1);

                printResult(result,"matrix_f a;");
            }


        }

    }
}
