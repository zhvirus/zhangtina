#include "MatrixTestor.h"
#include <iostream>
#include "Math/Matrix4x4_f.h"
#include "Math/Matrix4x4_d.h"
#include "Math/Float4.h"
#include "Math/Double4.h"
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

            testMatrix4x4_f();
            testMatrix4x4_d();
        }

        void MatrixTestor::testMatrix4x4_f()
        {
            std::cout<<"Testing 'matrix4x4_f'..."<<std::endl;

            {
                matrix4x4_f a;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],1) && VALUE_EQUAL<float>(a.v[1],0) && VALUE_EQUAL<float>(a.v[2],0) && VALUE_EQUAL<float>(a.v[3],0) &&
                    VALUE_EQUAL<float>(a.v[4],0) && VALUE_EQUAL<float>(a.v[5],1) && VALUE_EQUAL<float>(a.v[6],0) && VALUE_EQUAL<float>(a.v[7],0) &&
                    VALUE_EQUAL<float>(a.v[8],0) && VALUE_EQUAL<float>(a.v[9],0) && VALUE_EQUAL<float>(a.v[10],1) && VALUE_EQUAL<float>(a.v[11],0) &&
                    VALUE_EQUAL<float>(a.v[12],0) && VALUE_EQUAL<float>(a.v[13],0) && VALUE_EQUAL<float>(a.v[14],0) && VALUE_EQUAL<float>(a.v[15],1);

                printResult(result,0);
            }


            {
                matrix4x4_f a( 3.5f );

                bool result =
                    VALUE_EQUAL<float>(a.v[0],3.5f) && VALUE_EQUAL<float>(a.v[1],3.5f) && VALUE_EQUAL<float>(a.v[2],3.5f) && VALUE_EQUAL<float>(a.v[3],3.5f) &&
                    VALUE_EQUAL<float>(a.v[4],3.5f) && VALUE_EQUAL<float>(a.v[5],3.5f) && VALUE_EQUAL<float>(a.v[6],3.5f) && VALUE_EQUAL<float>(a.v[7],3.5f) &&
                    VALUE_EQUAL<float>(a.v[8],3.5f) && VALUE_EQUAL<float>(a.v[9],3.5f) && VALUE_EQUAL<float>(a.v[10],3.5f) && VALUE_EQUAL<float>(a.v[11],3.5f) &&
                    VALUE_EQUAL<float>(a.v[12],3.5f) && VALUE_EQUAL<float>(a.v[13],3.5f) && VALUE_EQUAL<float>(a.v[14],3.5f) && VALUE_EQUAL<float>(a.v[15],3.5f);

                printResult(result,1);
            }


            {
                matrix4x4_f a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );

                bool result =
                    VALUE_EQUAL<float>(a.v[0],1) && VALUE_EQUAL<float>(a.v[1],2) && VALUE_EQUAL<float>(a.v[2],3) && VALUE_EQUAL<float>(a.v[3],4) &&
                    VALUE_EQUAL<float>(a.v[4],5) && VALUE_EQUAL<float>(a.v[5],6) && VALUE_EQUAL<float>(a.v[6],7) && VALUE_EQUAL<float>(a.v[7],8) &&
                    VALUE_EQUAL<float>(a.v[8],9) && VALUE_EQUAL<float>(a.v[9],10) && VALUE_EQUAL<float>(a.v[10],11) && VALUE_EQUAL<float>(a.v[11],12) &&
                    VALUE_EQUAL<float>(a.v[12],13) && VALUE_EQUAL<float>(a.v[13],14) && VALUE_EQUAL<float>(a.v[14],15) && VALUE_EQUAL<float>(a.v[15],16);

                printResult(result,2);
            }


            {
                matrix4x4_f b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f a = b;

                bool result =
                    VALUE_EQUAL<float>(a.v[0],1) && VALUE_EQUAL<float>(a.v[1],2) && VALUE_EQUAL<float>(a.v[2],3) && VALUE_EQUAL<float>(a.v[3],4) &&
                    VALUE_EQUAL<float>(a.v[4],5) && VALUE_EQUAL<float>(a.v[5],6) && VALUE_EQUAL<float>(a.v[6],7) && VALUE_EQUAL<float>(a.v[7],8) &&
                    VALUE_EQUAL<float>(a.v[8],9) && VALUE_EQUAL<float>(a.v[9],10) && VALUE_EQUAL<float>(a.v[10],11) && VALUE_EQUAL<float>(a.v[11],12) &&
                    VALUE_EQUAL<float>(a.v[12],13) && VALUE_EQUAL<float>(a.v[13],14) && VALUE_EQUAL<float>(a.v[14],15) && VALUE_EQUAL<float>(a.v[15],16);

                printResult(result,3);
            }


            {
                float b[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                matrix4x4_f a = b;

                bool result =
                    VALUE_EQUAL<float>(a.v[0],1) && VALUE_EQUAL<float>(a.v[1],2) && VALUE_EQUAL<float>(a.v[2],3) && VALUE_EQUAL<float>(a.v[3],4) &&
                    VALUE_EQUAL<float>(a.v[4],5) && VALUE_EQUAL<float>(a.v[5],6) && VALUE_EQUAL<float>(a.v[6],7) && VALUE_EQUAL<float>(a.v[7],8) &&
                    VALUE_EQUAL<float>(a.v[8],9) && VALUE_EQUAL<float>(a.v[9],10) && VALUE_EQUAL<float>(a.v[10],11) && VALUE_EQUAL<float>(a.v[11],12) &&
                    VALUE_EQUAL<float>(a.v[12],13) && VALUE_EQUAL<float>(a.v[13],14) && VALUE_EQUAL<float>(a.v[14],15) && VALUE_EQUAL<float>(a.v[15],16);

                printResult(result,4);
            }

            {
                matrix4x4_f a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                a[0] = 2.55f;
                a[1] = 2.56f;
                a[2] = 2.57f;
                a[3] = 2.58f;

                a[4] = 2.59f;
                a[5] = 2.510f;
                a[6] = 2.512f;
                a[7] = 2.513f;

                a[8] = 2.514f;
                a[9] = 2.515f;
                a[10] = 2.516f;
                a[11] = 2.517f;

                a[12] = 2.518f;
                a[13] = 2.519f;
                a[14] = 2.520f;
                a[15] = 2.521f;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],2.55f) && VALUE_EQUAL<float>(a.v[1],2.56f) && VALUE_EQUAL<float>(a.v[2],2.57f) && VALUE_EQUAL<float>(a.v[3],2.58f) &&
                    VALUE_EQUAL<float>(a.v[4],2.59f) && VALUE_EQUAL<float>(a.v[5],2.510f) && VALUE_EQUAL<float>(a.v[6],2.512f) && VALUE_EQUAL<float>(a.v[7],2.513f) &&
                    VALUE_EQUAL<float>(a.v[8],2.514f) && VALUE_EQUAL<float>(a.v[9],2.515f) && VALUE_EQUAL<float>(a.v[10],2.516f) && VALUE_EQUAL<float>(a.v[11],2.517f) &&
                    VALUE_EQUAL<float>(a.v[12],2.518f) && VALUE_EQUAL<float>(a.v[13],2.519f) && VALUE_EQUAL<float>(a.v[14],2.520f) && VALUE_EQUAL<float>(a.v[15],2.521f);

                printResult(result,5);
            }


            // ==
            {
                matrix4x4_f a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                bool result = ( a == b ) == true;                 
                printResult(result,6);
            }

            {
                matrix4x4_f a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                float b[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                bool result = ( a == b ) == true;                    
                printResult(result,7);
            }

            {
                matrix4x4_f a( 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1 );
                bool result = ( a == 1 ) == true;                 
                printResult(result,8);
            }

            // !=
            {
                matrix4x4_f a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,17,16 );
                bool result = ( a != b ) == true;                 
                printResult(result,9);
            }

            {
                matrix4x4_f a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                float b[] = { 1,2,3,4,5,6,7,8,91,10,11,12,13,14,15,16 };
                bool result = ( a != b ) == true;                    
                printResult(result,10);
            }

            {
                matrix4x4_f a( 1,1,1,1, 12,1,1,1, 1,1,1,1, 1,1,1,1 );
                bool result = ( a != 1 ) == true;                 
                printResult(result,11);
            }

            // +
            {
                matrix4x4_f c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f a = c+b;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],2) && VALUE_EQUAL<float>(a.v[1],4) && VALUE_EQUAL<float>(a.v[2],6) && VALUE_EQUAL<float>(a.v[3],8) &&
                    VALUE_EQUAL<float>(a.v[4],10) && VALUE_EQUAL<float>(a.v[5],12) && VALUE_EQUAL<float>(a.v[6],14) && VALUE_EQUAL<float>(a.v[7],16) &&
                    VALUE_EQUAL<float>(a.v[8],18) && VALUE_EQUAL<float>(a.v[9],20) && VALUE_EQUAL<float>(a.v[10],22) && VALUE_EQUAL<float>(a.v[11],24) &&
                    VALUE_EQUAL<float>(a.v[12],26) && VALUE_EQUAL<float>(a.v[13],28) && VALUE_EQUAL<float>(a.v[14],30) && VALUE_EQUAL<float>(a.v[15],32);
               
                printResult(result,12);
            }

            {
                matrix4x4_f c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                float b[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                matrix4x4_f a = c+b;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],2) && VALUE_EQUAL<float>(a.v[1],4) && VALUE_EQUAL<float>(a.v[2],6) && VALUE_EQUAL<float>(a.v[3],8) &&
                    VALUE_EQUAL<float>(a.v[4],10) && VALUE_EQUAL<float>(a.v[5],12) && VALUE_EQUAL<float>(a.v[6],14) && VALUE_EQUAL<float>(a.v[7],16) &&
                    VALUE_EQUAL<float>(a.v[8],18) && VALUE_EQUAL<float>(a.v[9],20) && VALUE_EQUAL<float>(a.v[10],22) && VALUE_EQUAL<float>(a.v[11],24) &&
                    VALUE_EQUAL<float>(a.v[12],26) && VALUE_EQUAL<float>(a.v[13],28) && VALUE_EQUAL<float>(a.v[14],30) && VALUE_EQUAL<float>(a.v[15],32);
                            
                printResult(result,13);
            }

            {
                matrix4x4_f a( 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32 );
                a = a+100;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],102) && VALUE_EQUAL<float>(a.v[1],104) && VALUE_EQUAL<float>(a.v[2],106) && VALUE_EQUAL<float>(a.v[3],108) &&
                    VALUE_EQUAL<float>(a.v[4],110) && VALUE_EQUAL<float>(a.v[5],112) && VALUE_EQUAL<float>(a.v[6],114) && VALUE_EQUAL<float>(a.v[7],116) &&
                    VALUE_EQUAL<float>(a.v[8],118) && VALUE_EQUAL<float>(a.v[9],120) && VALUE_EQUAL<float>(a.v[10],122) && VALUE_EQUAL<float>(a.v[11],124) &&
                    VALUE_EQUAL<float>(a.v[12],126) && VALUE_EQUAL<float>(a.v[13],128) && VALUE_EQUAL<float>(a.v[14],130) && VALUE_EQUAL<float>(a.v[15],132);
                             
                printResult(result,14);
            }


            // -
            {
                matrix4x4_f c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_f a = c-b;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],0) && VALUE_EQUAL<float>(a.v[1],0) && VALUE_EQUAL<float>(a.v[2],0) && VALUE_EQUAL<float>(a.v[3],0) &&
                    VALUE_EQUAL<float>(a.v[4],0) && VALUE_EQUAL<float>(a.v[5],0) && VALUE_EQUAL<float>(a.v[6],0) && VALUE_EQUAL<float>(a.v[7],0) &&
                    VALUE_EQUAL<float>(a.v[8],0) && VALUE_EQUAL<float>(a.v[9],0) && VALUE_EQUAL<float>(a.v[10],0) && VALUE_EQUAL<float>(a.v[11],0) &&
                    VALUE_EQUAL<float>(a.v[12],0) && VALUE_EQUAL<float>(a.v[13],0) && VALUE_EQUAL<float>(a.v[14],0) && VALUE_EQUAL<float>(a.v[15],0);
               
                printResult(result,15);
            }

            {
                matrix4x4_f c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                float b[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                matrix4x4_f a = c-b;
                bool result =
                    VALUE_EQUAL<float>(a.v[0],0) && VALUE_EQUAL<float>(a.v[1],0) && VALUE_EQUAL<float>(a.v[2],0) && VALUE_EQUAL<float>(a.v[3],0) &&
                    VALUE_EQUAL<float>(a.v[4],0) && VALUE_EQUAL<float>(a.v[5],0) && VALUE_EQUAL<float>(a.v[6],0) && VALUE_EQUAL<float>(a.v[7],0) &&
                    VALUE_EQUAL<float>(a.v[8],0) && VALUE_EQUAL<float>(a.v[9],0) && VALUE_EQUAL<float>(a.v[10],0) && VALUE_EQUAL<float>(a.v[11],0) &&
                    VALUE_EQUAL<float>(a.v[12],0) && VALUE_EQUAL<float>(a.v[13],0) && VALUE_EQUAL<float>(a.v[14],0) && VALUE_EQUAL<float>(a.v[15],0);
               
                printResult(result,16);
            }

            {
                matrix4x4_f a( 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32 );
                a = a-2;
                float exp[]={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
                bool result = a == exp;
                             
                printResult(result,17);
            }

            // *
            {
                matrix4x4_f c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                matrix4x4_f b( 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4 );
                matrix4x4_f a = c*b;
                float exp = 30;

                bool result = (a == exp);

                printResult(result,18);
            }

            {
                matrix4x4_f c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                float b[] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4 };
                matrix4x4_f a = c*b;
                float exp = 30;

                bool result = (a == exp);
                printResult(result,19);
            }

            {
                matrix4x4_f c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                float b = 2;
                matrix4x4_f a = c*b;
                float exp[] = {2,4,6,8,2,4,6,8,2,4,6,8,2,4,6,8};

                bool result = (a == exp);                  
                printResult(result,20);
            }

            // /
            {
                matrix4x4_f c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                matrix4x4_f b( 1,1,1,1,2,2,2,2,2,2,2,2,4,4,4,4 );
                matrix4x4_f a = c/b;
                float exp[] = {1,2,3,4,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.25f,0.5f,0.75f,1.0f};

                bool result = (a == exp);

                printResult(result,21);
            }

            {
                matrix4x4_f c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                float b[] = { 1,1,1,1,2,2,2,2,2,2,2,2,4,4,4,4 };
                matrix4x4_f a = c/b;
                float exp[] = {1,2,3,4,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.25f,0.5f,0.75f,1.0f};

                bool result = (a == exp);
                printResult(result,22);
            }

            {
                matrix4x4_f c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                float b = 2;
                matrix4x4_f a = c/b;
                float exp[] = {0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f};

                bool result = (a == exp);                  
                printResult(result,23);
            }

            // identityIt
            {
                matrix4x4_f a( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                a.identityIt();
                bool result = (a == matrix4x4_f::IDENTITY);

                printResult(result,24);
            }

            // transposeIt
            {
                matrix4x4_f a( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                a.transposeIt();
                float exp[] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4};
                bool result = (a == exp);

                printResult(result,25);
            }

            // inverseIt
            {
                matrix4x4_f a( 1,0,0,2,  0,2,0,2, 0,0,3,2,  3,3,3,4 );
                matrix4x4_f b = a;
                b.inverseIt();
                matrix4x4_f c = a*b;
                bool result = (c == matrix4x4_f::IDENTITY);

                printResult(result,26);
            }

            //
            {
                float4 v(1,2,3,4);
                matrix4x4_f m(1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4);
                float4 v2 = m*v;
                bool result = (v2 == float4(10,20,30,40));
                printResult(result,27);


            }

        }



        void MatrixTestor::testMatrix4x4_d()
        {
            std::cout<<std::endl<<"Testing 'matrix4x4_d'..."<<std::endl;

            {
                matrix4x4_d a;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],1) && VALUE_EQUAL<double>(a.v[1],0) && VALUE_EQUAL<double>(a.v[2],0) && VALUE_EQUAL<double>(a.v[3],0) &&
                    VALUE_EQUAL<double>(a.v[4],0) && VALUE_EQUAL<double>(a.v[5],1) && VALUE_EQUAL<double>(a.v[6],0) && VALUE_EQUAL<double>(a.v[7],0) &&
                    VALUE_EQUAL<double>(a.v[8],0) && VALUE_EQUAL<double>(a.v[9],0) && VALUE_EQUAL<double>(a.v[10],1) && VALUE_EQUAL<double>(a.v[11],0) &&
                    VALUE_EQUAL<double>(a.v[12],0) && VALUE_EQUAL<double>(a.v[13],0) && VALUE_EQUAL<double>(a.v[14],0) && VALUE_EQUAL<double>(a.v[15],1);

                printResult(result,0);
            }


            {
                matrix4x4_d a( 3.5f );

                bool result =
                    VALUE_EQUAL<double>(a.v[0],3.5f) && VALUE_EQUAL<double>(a.v[1],3.5f) && VALUE_EQUAL<double>(a.v[2],3.5f) && VALUE_EQUAL<double>(a.v[3],3.5f) &&
                    VALUE_EQUAL<double>(a.v[4],3.5f) && VALUE_EQUAL<double>(a.v[5],3.5f) && VALUE_EQUAL<double>(a.v[6],3.5f) && VALUE_EQUAL<double>(a.v[7],3.5f) &&
                    VALUE_EQUAL<double>(a.v[8],3.5f) && VALUE_EQUAL<double>(a.v[9],3.5f) && VALUE_EQUAL<double>(a.v[10],3.5f) && VALUE_EQUAL<double>(a.v[11],3.5f) &&
                    VALUE_EQUAL<double>(a.v[12],3.5f) && VALUE_EQUAL<double>(a.v[13],3.5f) && VALUE_EQUAL<double>(a.v[14],3.5f) && VALUE_EQUAL<double>(a.v[15],3.5f);

                printResult(result,1);
            }


            {
                matrix4x4_d a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );

                bool result =
                    VALUE_EQUAL<double>(a.v[0],1) && VALUE_EQUAL<double>(a.v[1],2) && VALUE_EQUAL<double>(a.v[2],3) && VALUE_EQUAL<double>(a.v[3],4) &&
                    VALUE_EQUAL<double>(a.v[4],5) && VALUE_EQUAL<double>(a.v[5],6) && VALUE_EQUAL<double>(a.v[6],7) && VALUE_EQUAL<double>(a.v[7],8) &&
                    VALUE_EQUAL<double>(a.v[8],9) && VALUE_EQUAL<double>(a.v[9],10) && VALUE_EQUAL<double>(a.v[10],11) && VALUE_EQUAL<double>(a.v[11],12) &&
                    VALUE_EQUAL<double>(a.v[12],13) && VALUE_EQUAL<double>(a.v[13],14) && VALUE_EQUAL<double>(a.v[14],15) && VALUE_EQUAL<double>(a.v[15],16);

                printResult(result,2);
            }


            {
                matrix4x4_d b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d a = b;

                bool result =
                    VALUE_EQUAL<double>(a.v[0],1) && VALUE_EQUAL<double>(a.v[1],2) && VALUE_EQUAL<double>(a.v[2],3) && VALUE_EQUAL<double>(a.v[3],4) &&
                    VALUE_EQUAL<double>(a.v[4],5) && VALUE_EQUAL<double>(a.v[5],6) && VALUE_EQUAL<double>(a.v[6],7) && VALUE_EQUAL<double>(a.v[7],8) &&
                    VALUE_EQUAL<double>(a.v[8],9) && VALUE_EQUAL<double>(a.v[9],10) && VALUE_EQUAL<double>(a.v[10],11) && VALUE_EQUAL<double>(a.v[11],12) &&
                    VALUE_EQUAL<double>(a.v[12],13) && VALUE_EQUAL<double>(a.v[13],14) && VALUE_EQUAL<double>(a.v[14],15) && VALUE_EQUAL<double>(a.v[15],16);

                printResult(result,3);
            }


            {
                double b[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                matrix4x4_d a = b;

                bool result =
                    VALUE_EQUAL<double>(a.v[0],1) && VALUE_EQUAL<double>(a.v[1],2) && VALUE_EQUAL<double>(a.v[2],3) && VALUE_EQUAL<double>(a.v[3],4) &&
                    VALUE_EQUAL<double>(a.v[4],5) && VALUE_EQUAL<double>(a.v[5],6) && VALUE_EQUAL<double>(a.v[6],7) && VALUE_EQUAL<double>(a.v[7],8) &&
                    VALUE_EQUAL<double>(a.v[8],9) && VALUE_EQUAL<double>(a.v[9],10) && VALUE_EQUAL<double>(a.v[10],11) && VALUE_EQUAL<double>(a.v[11],12) &&
                    VALUE_EQUAL<double>(a.v[12],13) && VALUE_EQUAL<double>(a.v[13],14) && VALUE_EQUAL<double>(a.v[14],15) && VALUE_EQUAL<double>(a.v[15],16);

                printResult(result,4);
            }

            {
                matrix4x4_d a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                a[0] = 2.55f;
                a[1] = 2.56f;
                a[2] = 2.57f;
                a[3] = 2.58f;

                a[4] = 2.59f;
                a[5] = 2.510f;
                a[6] = 2.512f;
                a[7] = 2.513f;

                a[8] = 2.514f;
                a[9] = 2.515f;
                a[10] = 2.516f;
                a[11] = 2.517f;

                a[12] = 2.518f;
                a[13] = 2.519f;
                a[14] = 2.520f;
                a[15] = 2.521f;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],2.55f) && VALUE_EQUAL<double>(a.v[1],2.56f) && VALUE_EQUAL<double>(a.v[2],2.57f) && VALUE_EQUAL<double>(a.v[3],2.58f) &&
                    VALUE_EQUAL<double>(a.v[4],2.59f) && VALUE_EQUAL<double>(a.v[5],2.510f) && VALUE_EQUAL<double>(a.v[6],2.512f) && VALUE_EQUAL<double>(a.v[7],2.513f) &&
                    VALUE_EQUAL<double>(a.v[8],2.514f) && VALUE_EQUAL<double>(a.v[9],2.515f) && VALUE_EQUAL<double>(a.v[10],2.516f) && VALUE_EQUAL<double>(a.v[11],2.517f) &&
                    VALUE_EQUAL<double>(a.v[12],2.518f) && VALUE_EQUAL<double>(a.v[13],2.519f) && VALUE_EQUAL<double>(a.v[14],2.520f) && VALUE_EQUAL<double>(a.v[15],2.521f);

                printResult(result,5);
            }


            // ==
            {
                matrix4x4_d a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                bool result = ( a == b ) == true;                 
                printResult(result,6);
            }

            {
                matrix4x4_d a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                double b[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                bool result = ( a == b ) == true;                    
                printResult(result,7);
            }

            {
                matrix4x4_d a( 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1 );
                bool result = ( a == 1 ) == true;                 
                printResult(result,8);
            }

            // !=
            {
                matrix4x4_d a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,17,16 );
                bool result = ( a != b ) == true;                 
                printResult(result,9);
            }

            {
                matrix4x4_d a( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                double b[] = { 1,2,3,4,5,6,7,8,91,10,11,12,13,14,15,16 };
                bool result = ( a != b ) == true;                    
                printResult(result,10);
            }

            {
                matrix4x4_d a( 1,1,1,1, 12,1,1,1, 1,1,1,1, 1,1,1,1 );
                bool result = ( a != 1 ) == true;                 
                printResult(result,11);
            }

            // +
            {
                matrix4x4_d c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d a = c+b;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],2) && VALUE_EQUAL<double>(a.v[1],4) && VALUE_EQUAL<double>(a.v[2],6) && VALUE_EQUAL<double>(a.v[3],8) &&
                    VALUE_EQUAL<double>(a.v[4],10) && VALUE_EQUAL<double>(a.v[5],12) && VALUE_EQUAL<double>(a.v[6],14) && VALUE_EQUAL<double>(a.v[7],16) &&
                    VALUE_EQUAL<double>(a.v[8],18) && VALUE_EQUAL<double>(a.v[9],20) && VALUE_EQUAL<double>(a.v[10],22) && VALUE_EQUAL<double>(a.v[11],24) &&
                    VALUE_EQUAL<double>(a.v[12],26) && VALUE_EQUAL<double>(a.v[13],28) && VALUE_EQUAL<double>(a.v[14],30) && VALUE_EQUAL<double>(a.v[15],32);
               
                printResult(result,12);
            }

            {
                matrix4x4_d c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                double b[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                matrix4x4_d a = c+b;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],2) && VALUE_EQUAL<double>(a.v[1],4) && VALUE_EQUAL<double>(a.v[2],6) && VALUE_EQUAL<double>(a.v[3],8) &&
                    VALUE_EQUAL<double>(a.v[4],10) && VALUE_EQUAL<double>(a.v[5],12) && VALUE_EQUAL<double>(a.v[6],14) && VALUE_EQUAL<double>(a.v[7],16) &&
                    VALUE_EQUAL<double>(a.v[8],18) && VALUE_EQUAL<double>(a.v[9],20) && VALUE_EQUAL<double>(a.v[10],22) && VALUE_EQUAL<double>(a.v[11],24) &&
                    VALUE_EQUAL<double>(a.v[12],26) && VALUE_EQUAL<double>(a.v[13],28) && VALUE_EQUAL<double>(a.v[14],30) && VALUE_EQUAL<double>(a.v[15],32);
                            
                printResult(result,13);
            }

            {
                matrix4x4_d a( 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32 );
                a = a+100;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],102) && VALUE_EQUAL<double>(a.v[1],104) && VALUE_EQUAL<double>(a.v[2],106) && VALUE_EQUAL<double>(a.v[3],108) &&
                    VALUE_EQUAL<double>(a.v[4],110) && VALUE_EQUAL<double>(a.v[5],112) && VALUE_EQUAL<double>(a.v[6],114) && VALUE_EQUAL<double>(a.v[7],116) &&
                    VALUE_EQUAL<double>(a.v[8],118) && VALUE_EQUAL<double>(a.v[9],120) && VALUE_EQUAL<double>(a.v[10],122) && VALUE_EQUAL<double>(a.v[11],124) &&
                    VALUE_EQUAL<double>(a.v[12],126) && VALUE_EQUAL<double>(a.v[13],128) && VALUE_EQUAL<double>(a.v[14],130) && VALUE_EQUAL<double>(a.v[15],132);
                             
                printResult(result,14);
            }


            // -
            {
                matrix4x4_d c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d b( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                matrix4x4_d a = c-b;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],0) && VALUE_EQUAL<double>(a.v[1],0) && VALUE_EQUAL<double>(a.v[2],0) && VALUE_EQUAL<double>(a.v[3],0) &&
                    VALUE_EQUAL<double>(a.v[4],0) && VALUE_EQUAL<double>(a.v[5],0) && VALUE_EQUAL<double>(a.v[6],0) && VALUE_EQUAL<double>(a.v[7],0) &&
                    VALUE_EQUAL<double>(a.v[8],0) && VALUE_EQUAL<double>(a.v[9],0) && VALUE_EQUAL<double>(a.v[10],0) && VALUE_EQUAL<double>(a.v[11],0) &&
                    VALUE_EQUAL<double>(a.v[12],0) && VALUE_EQUAL<double>(a.v[13],0) && VALUE_EQUAL<double>(a.v[14],0) && VALUE_EQUAL<double>(a.v[15],0);
               
                printResult(result,15);
            }

            {
                matrix4x4_d c( 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 );
                double b[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
                matrix4x4_d a = c-b;
                bool result =
                    VALUE_EQUAL<double>(a.v[0],0) && VALUE_EQUAL<double>(a.v[1],0) && VALUE_EQUAL<double>(a.v[2],0) && VALUE_EQUAL<double>(a.v[3],0) &&
                    VALUE_EQUAL<double>(a.v[4],0) && VALUE_EQUAL<double>(a.v[5],0) && VALUE_EQUAL<double>(a.v[6],0) && VALUE_EQUAL<double>(a.v[7],0) &&
                    VALUE_EQUAL<double>(a.v[8],0) && VALUE_EQUAL<double>(a.v[9],0) && VALUE_EQUAL<double>(a.v[10],0) && VALUE_EQUAL<double>(a.v[11],0) &&
                    VALUE_EQUAL<double>(a.v[12],0) && VALUE_EQUAL<double>(a.v[13],0) && VALUE_EQUAL<double>(a.v[14],0) && VALUE_EQUAL<double>(a.v[15],0);
               
                printResult(result,16);
            }

            {
                matrix4x4_d a( 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32 );
                a = a-2;
                double exp[]={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
                bool result = a == exp;
                             
                printResult(result,17);
            }

            // *
            {
                matrix4x4_d c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                matrix4x4_d b( 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4 );
                matrix4x4_d a = c*b;
                double exp = 30;

                bool result = (a == exp);

                printResult(result,18);
            }

            {
                matrix4x4_d c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                double b[] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4 };
                matrix4x4_d a = c*b;
                double exp = 30;

                bool result = (a == exp);
                printResult(result,19);
            }

            {
                matrix4x4_d c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                double b = 2;
                matrix4x4_d a = c*b;
                double exp[] = {2,4,6,8,2,4,6,8,2,4,6,8,2,4,6,8};

                bool result = (a == exp);                  
                printResult(result,20);
            }

            // /
            {
                matrix4x4_d c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                matrix4x4_d b( 1,1,1,1,2,2,2,2,2,2,2,2,4,4,4,4 );
                matrix4x4_d a = c/b;
                double exp[] = {1,2,3,4,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.25f,0.5f,0.75f,1.0f};

                bool result = (a == exp);

                printResult(result,21);
            }

            {
                matrix4x4_d c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                double b[] = { 1,1,1,1,2,2,2,2,2,2,2,2,4,4,4,4 };
                matrix4x4_d a = c/b;
                double exp[] = {1,2,3,4,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.25f,0.5f,0.75f,1.0f};

                bool result = (a == exp);
                printResult(result,22);
            }

            {
                matrix4x4_d c( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                double b = 2;
                matrix4x4_d a = c/b;
                double exp[] = {0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f,0.5f,1.0f,1.5f,2.0f};

                bool result = (a == exp);                  
                printResult(result,23);
            }

            // identityIt
            {
                matrix4x4_d a( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                a.identityIt();
                bool result = (a == matrix4x4_d::IDENTITY);

                printResult(result,24);
            }

            // transposeIt
            {
                matrix4x4_d a( 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 );
                a.transposeIt();
                double exp[] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4};
                bool result = (a == exp);

                printResult(result,25);
            }

            // inverseIt
            {
                matrix4x4_d a( 1,0,0,2,  0,2,0,2, 0,0,3,2,  3,3,3,4 );
                matrix4x4_d b = a;
                b.inverseIt();
                matrix4x4_d c = a*b;
                bool result = (c == matrix4x4_d::IDENTITY);

                printResult(result,26);
            }

            //
            {
                double4 v(1,2,3,4);
                matrix4x4_d m(1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4);
                double4 v2 = m*v;
                bool result = (v2 == double4(10,20,30,40));
                printResult(result,27);


            }

        }
    }
}
