#include "VectorTestor.h"
#include <iostream>
#include "Math/Int2.h"
#include "Math/Int3.h"
#include "Math/Int4.h"
#include "Math/float2.h"
#include "Math/float3.h"
#include "Math/float4.h"
#include "Math/double2.h"
#include "Math/double3.h"
#include "Math/double4.h"
#include "Math/MathCommon.h"

using namespace ZH::Math;

namespace ZH{
    namespace TEST{

        void VectorTestor::beginTest(const std::string& s)
        {
            BaseTestor::beginTest( s );

            // TODO
        }
        void VectorTestor::endTest(const std::string& s)
        {
            //TODO


            BaseTestor::endTest(s);
        }
        void VectorTestor::doTest(const std::string& s)
        {
            BaseTestor::doTest(s);

            testVector2();
            testVector3();
            testVector4();
        }

        void VectorTestor::testVector2()
        {
            std::cout<<"Testing 'Vector2'..."<<std::endl;
            
            // Vector2();
            float2 a;
            printResult(VALUE_EQUAL<float>(a.x,0.0f) && VALUE_EQUAL<float>(a.y,0.0f),0);
            
            // Member data
            a.x = 1; a.y = 2;
            printResult(VALUE_EQUAL<float>(a.x,1.0f) && VALUE_EQUAL<float>(a.y,2.0f),1);

            // Vector2( T a, T b);
            float2 b(6,3);
            printResult(VALUE_EQUAL<float>(b.x,6.0f) && VALUE_EQUAL<float>(b.y,3.0f),2);

            // Vector2( const Vector2& v );
            float2 c(a);
            printResult(VALUE_EQUAL<float>(c.x,1.0f) && VALUE_EQUAL<float>(c.y,2.0f),3);

            // T& operator [] ( unsigned int idx );
            c[0] = 19; c[1] = 82;
            printResult(VALUE_EQUAL<float>(c.x,19.0f) && VALUE_EQUAL<float>(c.y,82.0f),4);

            // Member data
            float singlef = c.y;
            printResult(VALUE_EQUAL<float>(singlef,82.0f),5);

            // T operator [] ( unsigned int idx )const;
            singlef = c[0];
            printResult(VALUE_EQUAL<float>(singlef,19.0f),6);

            // Vector2& operator = ( const Vector2& v );
            c = a;
            printResult(VALUE_EQUAL<float>(c.x,1.0f) && VALUE_EQUAL<float>(c[1],2.0f),7);

            // Vector2( T* v);
            float pp[] = { 1.2f, 3.6f}; float2 d(pp);
            printResult(VALUE_EQUAL<float>(d.x, 1.2f) && VALUE_EQUAL<float>(d[1],3.6f),8);

            // Vector2& operator = ( const T* v );
            c = pp;
            printResult(VALUE_EQUAL<float>(c.x, 1.2f) && VALUE_EQUAL<float>(c[1],3.6f),9);

            // ==, !=
            float2 xx(1.2f,3.5f);
            float2 xx2(1.2f,3.5f);
            float2 xx3(2.2f,4.5f);
            float  xxArr[] = {1.2f,3.5f};
            float  xxArr2[]= {1.3f,3.5f};
            printResult(xxArr == xx,10);
            printResult(xxArr2 != xx,11);
            printResult(xx3 != xx,12);
            printResult(xx2 == xx,13);

            // +
            float2 xx4 = xx + xx2;
            printResult( VALUE_EQUAL<float>(xx4.x,2.4f) && VALUE_EQUAL<float>(xx4.y, 7.0f),14);
            float2 xx5 = xx + xxArr;
            printResult( VALUE_EQUAL<float>(xx5.x,2.4f) && VALUE_EQUAL<float>(xx5.y,7.0f),15);
            float2 xx6 = xxArr + xx2;
            printResult( VALUE_EQUAL<float>(xx6.x,2.4f) && VALUE_EQUAL<float>(xx6.y,7.0f),16);
            float2 xx7 = 6.7f + xx2;
            printResult( VALUE_EQUAL<float>(xx7.x,7.9f) && VALUE_EQUAL<float>(xx7.y, 10.2f),17);

            // -
            {
                float2 mm1(10.0f,12.0f);
                float2 mm2(5.0f, 4.0f);
                float mmArr[] = {3.0f,2.5f};
                float2 mm12a = mm1 - mm2;
                float2 mm12b = mm1 - mmArr;
                float2 mm12c = mmArr - mm1;
                float2 mm12d = 3.0f - mm1;
                printResult(VALUE_EQUAL<float>(mm12a.x,5.0f) && VALUE_EQUAL<float>(mm12a.y,8.0f) ,18);
                printResult(VALUE_EQUAL<float>(mm12b.x,7.0f) && VALUE_EQUAL<float>(mm12b.y,9.5f) ,19);
                printResult(VALUE_EQUAL<float>(mm12c.x,-7.0f) && VALUE_EQUAL<float>(mm12c.y,-9.5f) ,20);
                printResult(VALUE_EQUAL<float>(mm12d.x,-7.0f) && VALUE_EQUAL<float>(mm12d.y,-9.0f) ,21);
            }

            // *
            {
                float2 mm1(10.0f,12.0f);
                float2 mm2(5.0f, 4.0f);
                float mmArr[] = {3.0f,2.5f};
                float k = 10.0f;
                float2 m1 = mm1 * mm2;
                float2 m2 = mm1 * mmArr;
                float2 m3 = mm1 * k;
                float2 m4 = mmArr * mm1;
                float2 m5 = k * mm1;
                printResult(VALUE_EQUAL<float>(m1.x,50.0f) && VALUE_EQUAL<float>(m1.y,48.0f) ,22);
                printResult(VALUE_EQUAL<float>(m2.x,30.0f) && VALUE_EQUAL<float>(m2.y,30.0f) ,23);
                printResult(VALUE_EQUAL<float>(m3.x,100.0f) && VALUE_EQUAL<float>(m3.y,120.0f) ,24);
                printResult(VALUE_EQUAL<float>(m4.x,30.0f) && VALUE_EQUAL<float>(m4.y,30.0f) ,25);
                printResult(VALUE_EQUAL<float>(m5.x,100.0f) && VALUE_EQUAL<float>(m5.y,120.0f) ,26);
            }

            // /
            {
                float2 mm1(10.0f,20.0f);
                float2 mm2(5.0f, 10.0f);
                float mmArr[] = {8.0f,5.0f};
                float k = 10.0f;
                float2 m1 = mm1 / mm2;
                float2 m2 = mm1 / mmArr;
                float2 m3 = mm1 / k;
                float2 m4 = mmArr / mm1;
                float2 m5 = k / mm1;
                printResult(VALUE_EQUAL<float>(m1.x,2.0f) && VALUE_EQUAL<float>(m1.y,2.0f) ,27);
                printResult(VALUE_EQUAL<float>(m2.x,1.25f) && VALUE_EQUAL<float>(m2.y,4.0f) ,28);
                printResult(VALUE_EQUAL<float>(m3.x,1.0f) && VALUE_EQUAL<float>(m3.y,2.0f) ,29);
                printResult(VALUE_EQUAL<float>(m4.x,0.8f) && VALUE_EQUAL<float>(m4.y,0.25f) ,30);
                printResult(VALUE_EQUAL<float>(m5.x,1.0f) && VALUE_EQUAL<float>(m5.y,0.5f) ,31);
            }

            // +=
            {
                float2 mm1(10.0f,20.0f);
                float2 mm2(5.0f, 10.0f);
                float mmArr[] = {8.0f,5.0f};
                mm1 += mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,15.0f) && VALUE_EQUAL<float>(mm1.y,30.0f) ,32);
                mm1 += mmArr;
                mmArr += mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,23.0f) && VALUE_EQUAL<float>(mm1.y,35.0f) ,33);
                printResult(VALUE_EQUAL<float>(mmArr[0],13.0f) && VALUE_EQUAL<float>(mmArr[1],15.0f) ,34);
            }

            // -=
            {
                float2 mm1(10.0f,20.0f);
                float2 mm2(5.0f, 10.0f);
                float mmArr[] = {8.0f,5.0f};
                mm1 -= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,5.0f) && VALUE_EQUAL<float>(mm1.y,10.0f) ,35);
                mm1 -= mmArr;
                mmArr -= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,-3.0f) && VALUE_EQUAL<float>(mm1.y,5.0f) ,36);
                printResult(VALUE_EQUAL<float>(mmArr[0],3.0f) && VALUE_EQUAL<float>(mmArr[1],-5.0f) ,37);
            }

            // *=
            {
                float2 mm1(10.0f,20.0f);
                float2 mm2(5.0f, 10.0f);
                float mmArr[] = {8.0f,5.0f};
                float k = 20.0f;
                mm1 *= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,50.0f) && VALUE_EQUAL<float>(mm1.y,200.0f) ,38);
                mm1 *= mmArr;
                printResult(VALUE_EQUAL<float>(mm1.x,400.0f) && VALUE_EQUAL<float>(mm1.y,1000.0f) ,39);
                mm2 *= k;
                printResult(VALUE_EQUAL<float>(mm2.x,100.0f) && VALUE_EQUAL<float>(mm2.y,200.0f) ,40);
                mmArr *= mm2;
                printResult(VALUE_EQUAL<float>(mmArr[0],800.0f) && VALUE_EQUAL<float>(mmArr[1],1000.0f) ,41);
            }

            // /=
            {
                float2 mm1(100.0f,200.0f);
                float2 mm2(5.0f, 10.0f);
                float mmArr[] = {2.0f,5.0f};
                float k = 20.0f;
                mm1 /= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,20.0f) && VALUE_EQUAL<float>(mm1.y,20.0f) ,42);
                mm1 /= mmArr;
                printResult(VALUE_EQUAL<float>(mm1.x,10.0f) && VALUE_EQUAL<float>(mm1.y,4.0f) ,43);
                mm2 /= k;
                printResult(VALUE_EQUAL<float>(mm2.x,0.25f) && VALUE_EQUAL<float>(mm2.y,0.5f) ,44);
                mmArr /= mm2;
                printResult(VALUE_EQUAL<float>(mmArr[0],8.0f) && VALUE_EQUAL<float>(mmArr[1],10.0f) ,45);
            }

            // length
            {
                float2 a(3.0f,4.0f);
                printResult(VALUE_EQUAL<double>(a.length(),5.0f) ,46);
            }

            // squared length
            {
                float2 a(3.0f,4.0f);
                printResult(VALUE_EQUAL<double>(a.squaredLength(),25.0f) ,47);
            }

            // dot product
            {
                float2 a(3.0f,4.0f);
                float2 b(10.0f, 5.0f);
                double c = a.dot(b);
                printResult(VALUE_EQUAL<double>(c,50.0f) ,48);
            }

        }

        void VectorTestor::testVector3()
        {
            std::cout<<"\n\n\nTesting 'Vector3'..."<<std::endl;

            // Vector3();
            float3 a;
            printResult(VALUE_EQUAL<float>(a.x,0.0f) && VALUE_EQUAL<float>(a.y,0.0f) && VALUE_EQUAL<float>(a.z,0.0f),49);

            // Member data
            a.x = 1; a.y = 2; a.z = 3;
            printResult(VALUE_EQUAL<float>(a.x,1.0f) && VALUE_EQUAL<float>(a.y,2.0f) && VALUE_EQUAL<float>(a.z,3.0f),50);

            // Vector3( T a, T b, T c);
            float3 b(6,3,1);
            printResult(VALUE_EQUAL<float>(b.x,6.0f) && VALUE_EQUAL<float>(b.y,3.0f) && VALUE_EQUAL<float>(b.z,1.0f),51);

            // Vector3( const Vector3& v );
            float3 c(a);
            printResult(VALUE_EQUAL<float>(c.x,1.0f) && VALUE_EQUAL<float>(c.y,2.0f) && VALUE_EQUAL<float>(c.z,3.0f),52);

            // T& operator [] ( unsigned int idx );
            c[0] = 19; c[1] = 82; c[2] = -2.5f;
            printResult(VALUE_EQUAL<float>(c.x,19.0f) && VALUE_EQUAL<float>(c.y,82.0f) && VALUE_EQUAL<float>(c.z, -2.5f),53);

            // Member data
            float singlef = c.y;
            printResult(VALUE_EQUAL<float>(singlef,82.0f),54);

            // T operator [] ( unsigned int idx )const;
            singlef = c[2];
            printResult(VALUE_EQUAL<float>(singlef,-2.5f),55);

            // Vector3& operator = ( const Vector3& v );
            c = a;
            printResult(VALUE_EQUAL<float>(c.x,1.0f) && VALUE_EQUAL<float>(c[1],2.0f) && VALUE_EQUAL<float>(c[2],3.0f),56);

            // Vector3( T* v);
            float pp[] = { 1.2f, 3.6f, 5.12f}; float3 d(pp);
            printResult(VALUE_EQUAL<float>(d.x, 1.2f) && VALUE_EQUAL<float>(d[1],3.6f) && VALUE_EQUAL<float>(d[2],5.12f),57);

            // Vector3& operator = ( const T* v );
            c = pp;
            printResult(VALUE_EQUAL<float>(c.x, 1.2f) && VALUE_EQUAL<float>(c[1],3.6f) && VALUE_EQUAL<float>(c[2],5.12f) ,58);

            // ==, !=
            float3 xx(1.2f,3.5f, 1.5f);
            float3 xx2(1.2f,3.5f,1.5f);
            float3 xx3(2.2f,4.5f, 3.0f);
            float  xxArr[] = {1.2f,3.5f, 1.5f};
            float  xxArr2[]= {1.3f,3.5f, 1.1f};
            printResult(xxArr == xx,59);
            printResult(xxArr2 != xx,60);
            printResult(xx3 != xx,61);
            printResult(xx2 == xx,62);

            // +
            float3 xx4 = xx + xx2;
            printResult( VALUE_EQUAL<float>(xx4.x,2.4f) && VALUE_EQUAL<float>(xx4.y, 7.0f) && VALUE_EQUAL<float>(xx4.z, 3.0f),63);
            float3 xx5 = xx + xxArr;
            printResult( VALUE_EQUAL<float>(xx5.x,2.4f) && VALUE_EQUAL<float>(xx5.y,7.0f) && VALUE_EQUAL<float>(xx5.z,3.0f),64);
            float3 xx6 = xxArr + xx2;
            printResult( VALUE_EQUAL<float>(xx6.x,2.4f) && VALUE_EQUAL<float>(xx6.y,7.0f) && VALUE_EQUAL<float>(xx6.z,3.0f),65);
            float3 xx7 = 6.7f + xx2;
            printResult( VALUE_EQUAL<float>(xx7.x,7.9f) && VALUE_EQUAL<float>(xx7.y, 10.2f) && VALUE_EQUAL<float>(xx7.z, 8.2f),66);

            // -
            {
                float3 mm1(10.0f,12.0f,6.0f);
                float3 mm2(5.0f, 4.0f, 2.0f);
                float mmArr[] = {3.0f,2.5f, 10.0f};
                float3 mm12a = mm1 - mm2;
                float3 mm12b = mm1 - mmArr;
                float3 mm12c = mmArr - mm1;
                float3 mm12d = 3.0f - mm1;
                printResult(VALUE_EQUAL<float>(mm12a.x,5.0f) && VALUE_EQUAL<float>(mm12a.y,8.0f) && VALUE_EQUAL<float>(mm12a.z,4.0f) ,67);
                printResult(VALUE_EQUAL<float>(mm12b.x,7.0f) && VALUE_EQUAL<float>(mm12b.y,9.5f) && VALUE_EQUAL<float>(mm12b.z,-4.0f),68);
                printResult(VALUE_EQUAL<float>(mm12c.x,-7.0f) && VALUE_EQUAL<float>(mm12c.y,-9.5f) && VALUE_EQUAL<float>(mm12c.z,4.0f) ,69);
                printResult(VALUE_EQUAL<float>(mm12d.x,-7.0f) && VALUE_EQUAL<float>(mm12d.y,-9.0f) && VALUE_EQUAL<float>(mm12d.z,-3.0f) ,70);
            }

            // *
            {
                float3 mm1(10.0f,12.0f,6.0f);
                float3 mm2(5.0f, 4.0f, 2.0f);
                float mmArr[] = {3.0f,2.5f, 10.0f};
                float k = 10.0f;
                float3 m1 = mm1 * mm2;
                float3 m2 = mm1 * mmArr;
                float3 m3 = mm1 * k;
                float3 m4 = mmArr * mm1;
                float3 m5 = k * mm1;
                printResult(VALUE_EQUAL<float>(m1.x,50.0f) && VALUE_EQUAL<float>(m1.y,48.0f) && VALUE_EQUAL<float>(m1.z,12.0f) ,71);
                printResult(VALUE_EQUAL<float>(m2.x,30.0f) && VALUE_EQUAL<float>(m2.y,30.0f) && VALUE_EQUAL<float>(m2.z,60.0f) ,72);
                printResult(VALUE_EQUAL<float>(m3.x,100.0f) && VALUE_EQUAL<float>(m3.y,120.0f) && VALUE_EQUAL<float>(m3.z,60.0f) ,73);
                printResult(VALUE_EQUAL<float>(m4.x,30.0f) && VALUE_EQUAL<float>(m4.y,30.0f) && VALUE_EQUAL<float>(m4.z,60.0f) ,74);
                printResult(VALUE_EQUAL<float>(m5.x,100.0f) && VALUE_EQUAL<float>(m5.y,120.0f) && VALUE_EQUAL<float>(m5.z,60.0f) ,75);
            }

            // /
            {
                float3 mm1(10.0f,100.0f,20.0f);
                float3 mm2(5.0f, 4.0f, 2.0f);
                float mmArr[] = {5.0f,2.0f, 10.0f};
                float k = 10.0f;
                float3 m1 = mm1 / mm2;
                float3 m2 = mm1 / mmArr;
                float3 m3 = mm1 / k;
                float3 m4 = mmArr / mm1;
                float3 m5 = k / mm1;
                printResult(VALUE_EQUAL<float>(m1.x,2.0f) && VALUE_EQUAL<float>(m1.y,25.0f) && VALUE_EQUAL<float>(m1.z,10.0f) ,76);
                printResult(VALUE_EQUAL<float>(m2.x,2.0f) && VALUE_EQUAL<float>(m2.y,50.0f) && VALUE_EQUAL<float>(m2.z,2.0f) ,77);
                printResult(VALUE_EQUAL<float>(m3.x,1.0f) && VALUE_EQUAL<float>(m3.y,10.0f) && VALUE_EQUAL<float>(m3.z,2.0f) ,78);
                printResult(VALUE_EQUAL<float>(m4.x,0.5f) && VALUE_EQUAL<float>(m4.y,0.02f) && VALUE_EQUAL<float>(m4.z,0.5f) ,79);
                printResult(VALUE_EQUAL<float>(m5.x,1.0f) && VALUE_EQUAL<float>(m5.y,0.1f) && VALUE_EQUAL<float>(m5.z,0.5f) ,80);
            }

            // +=
            {
                float3 mm1(10.0f,20.0f, 20.0f);
                float3 mm2(5.0f, 10.0f, 100.0f);
                float mmArr[] = {8.0f,5.0f,0.0f};
                mm1 += mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,15.0f) && VALUE_EQUAL<float>(mm1.y,30.0f) && VALUE_EQUAL<float>(mm1.z,120.0f) ,81);
                mm1 += mmArr;
                mmArr += mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,23.0f) && VALUE_EQUAL<float>(mm1.y,35.0f) && VALUE_EQUAL<float>(mm1.z,120.0f) ,82);
                printResult(VALUE_EQUAL<float>(mmArr[0],13.0f) && VALUE_EQUAL<float>(mmArr[1],15.0f) && VALUE_EQUAL<float>(mmArr[2],100.0f) ,83);
            }

            // -=
            {
                float3 mm1(10.0f,20.0f, 15.0f);
                float3 mm2(5.0f, 10.0f, 7.0f);
                float mmArr[] = {8.0f,5.0f, 13.0f};
                mm1 -= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,5.0f) && VALUE_EQUAL<float>(mm1.y,10.0f) && VALUE_EQUAL<float>(mm1.z,8.0f) ,84);
                mm1 -= mmArr;
                mmArr -= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,-3.0f) && VALUE_EQUAL<float>(mm1.y,5.0f) && VALUE_EQUAL<float>(mm1.z,-5.0f) ,85);
                printResult(VALUE_EQUAL<float>(mmArr[0],3.0f) && VALUE_EQUAL<float>(mmArr[1],-5.0f) && VALUE_EQUAL<float>(mmArr[2],6.0f) ,86);
            }

            // *=
            {
                float3 mm1(10.0f,20.0f, 5.0f);
                float3 mm2(5.0f, 10.0f, 10.0f);
                float mmArr[] = {8.0f,5.0f, -2.0f};
                float k = -20.0f;
                mm1 *= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,50.0f) && VALUE_EQUAL<float>(mm1.y,200.0f) && VALUE_EQUAL<float>(mm1.z,50.0f),87);
                mm1 *= mmArr;
                printResult(VALUE_EQUAL<float>(mm1.x,400.0f) && VALUE_EQUAL<float>(mm1.y,1000.0f) && VALUE_EQUAL<float>(mm1.z,-100.0f),88);
                mm2 *= k;
                printResult(VALUE_EQUAL<float>(mm2.x,-100.0f) && VALUE_EQUAL<float>(mm2.y,-200.0f) && VALUE_EQUAL<float>(mm2.z,-200.0f),89);
                mmArr *= mm2;
                printResult(VALUE_EQUAL<float>(mmArr[0],-800.0f) && VALUE_EQUAL<float>(mmArr[1],-1000.0f) && VALUE_EQUAL<float>(mmArr[2],400.0f),90);
            }

            // /=
            {
                float3 mm1(100.0f,200.0f,10.0f);
                float3 mm2(5.0f, 10.0f,-20.0f);
                float mmArr[] = {2.0f,5.0f,-100.0f};
                float k = -2.0f;
                mm1 /= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,20.0f) && VALUE_EQUAL<float>(mm1.y,20.0f) && VALUE_EQUAL<float>(mm1.z,-0.5f) ,91);
                mm1 /= mmArr;
                printResult(VALUE_EQUAL<float>(mm1.x,10.0f) && VALUE_EQUAL<float>(mm1.y,4.0f) && VALUE_EQUAL<float>(mm1.z,0.005f),92);
                mm2 /= k;
                printResult(VALUE_EQUAL<float>(mm2.x,-2.5f) && VALUE_EQUAL<float>(mm2.y,-5.0f) && VALUE_EQUAL<float>(mm2.z,10.0f),93);
                mmArr /= mm2;
                printResult(VALUE_EQUAL<float>(mmArr[0],-0.8f) && VALUE_EQUAL<float>(mmArr[1],-1.0f) && VALUE_EQUAL<float>(mmArr[2],-10.0f),94);
            }

            // length
            {
                float3 a(3.0f,4.0f,12.0f);
                printResult(VALUE_EQUAL<double>(a.length(),13.0f) ,95);
            }

            // squared length
            {
                float3 a(3.0f,4.0f,12.0f);
                printResult(VALUE_EQUAL<double>(a.squaredLength(),169.0f) ,96);
            }

            // dot product
            {
                float3 a(3.0f, 4.0f, 12.0f);
                float3 b(10.0f, 5.0f, 10.0f);
                double c = a.dot(b);
                printResult(VALUE_EQUAL<double>(c,170.0f) ,97);
            }

            // cross product
            {
                float3 a(1.0f, 2.0f, 3.0f);
                float3 b(4.0f, 5.0f, 6.0f);
                float3 c = a.cross(b);
                printResult(VALUE_EQUAL<double>(c.x,-3.0f) && VALUE_EQUAL<double>(c.y,6.0f) && VALUE_EQUAL<double>(c.z,-3.0f),98);
            }

            // normalize
            {
                float3 a(3.0f, 4.0f, 12.0f);
                float3 b = a.normalize();
                printResult(VALUE_EQUAL<double>(a.x,3.0f) && VALUE_EQUAL<double>(a.y,4.0f) && VALUE_EQUAL<double>(a.z,12.0f),99);
                printResult(VALUE_EQUAL<double>(b.x,3.0f/13.0f) && VALUE_EQUAL<double>(b.y,4.0f/13.0f) && VALUE_EQUAL<double>(b.z,12.0f/13.0f),100);
                a.normalizeIt();
                printResult(VALUE_EQUAL<double>(a.x,b.x) && VALUE_EQUAL<double>(a.y,b.y) && VALUE_EQUAL<double>(a.z,b.z),101);
            }
        }


        void VectorTestor::testVector4()
        {
            std::cout<<"\n\n\nTesting 'Vector4'..."<<std::endl;

            // Vector4();
            float4 a;
            printResult(VALUE_EQUAL<float>(a.x,0.0f) && VALUE_EQUAL<float>(a.y,0.0f) && VALUE_EQUAL<float>(a.z,0.0f) && VALUE_EQUAL<float>(a.w,0.0f),
                102);

            // Member data
            a.x = 1; a.y = 2; a.z = 3; a.w = 4.0f;
            printResult(VALUE_EQUAL<float>(a.x,1.0f) && VALUE_EQUAL<float>(a.y,2.0f) && VALUE_EQUAL<float>(a.z,3.0f) && VALUE_EQUAL<float>(a.w,4.0f),
                103);

            // Vector4( T a, T b, T c);
            float4 b(6,3,1,-2);
            printResult(VALUE_EQUAL<float>(b.x,6.0f) && VALUE_EQUAL<float>(b.y,3.0f) && VALUE_EQUAL<float>(b.z,1.0f) && VALUE_EQUAL<float>(b.w,-2.0f),
                104);

            // Vector4( const Vector4& v );
            float4 c(a);
            printResult(VALUE_EQUAL<float>(c.x,1.0f) && VALUE_EQUAL<float>(c.y,2.0f) && VALUE_EQUAL<float>(c.z,3.0f) && VALUE_EQUAL<float>(c.w,4.0f),
                105);

            // T& operator [] ( unsigned int idx );
            c[0] = 19; c[1] = 82; c[2] = -2.5f; c[3] = 3.0f;
            printResult(VALUE_EQUAL<float>(c.x,19.0f) && VALUE_EQUAL<float>(c.y,82.0f) && VALUE_EQUAL<float>(c.z, -2.5f) && VALUE_EQUAL<float>(c.w, 3.0f),
                106);

            // Member data
            float singlef = c.w;
            printResult(VALUE_EQUAL<float>(singlef,3.0f),
                107);

            // T operator [] ( unsigned int idx )const;
            singlef = c[2];
            printResult(VALUE_EQUAL<float>(singlef,-2.5f),
                108);

            // Vector4& operator = ( const Vector4& v );
            c = a;
            printResult(VALUE_EQUAL<float>(c.x,1.0f) && VALUE_EQUAL<float>(c[1],2.0f) && VALUE_EQUAL<float>(c[2],3.0f) && VALUE_EQUAL<float>(c[3],4.0f),
                109);

            // Vector4( T* v);
            float pp[] = { 1.2f, 3.6f, 5.12f, 2.2f}; float4 d(pp);
            printResult(VALUE_EQUAL<float>(d.x, 1.2f) && VALUE_EQUAL<float>(d[1],3.6f) && VALUE_EQUAL<float>(d[2],5.12f) && VALUE_EQUAL<float>(d[3],2.2f),
                110);

            // Vector4& operator = ( const T* v );
            c = pp;
            printResult(VALUE_EQUAL<float>(c.x, 1.2f) && VALUE_EQUAL<float>(c[1],3.6f) && VALUE_EQUAL<float>(c[2],5.12f) && VALUE_EQUAL<float>(c[3],2.2f),
                111);

            // ==, !=
            float4 xx(1.2f,3.5f, 1.5f, 2.0f);
            float4 xx2(1.2f,3.5f,1.5f, 2.0f);
            float4 xx3(2.2f,4.5f, 3.0f, -2.0f);
            float  xxArr[] = {1.2f,3.5f, 1.5f, 2.0f};
            float  xxArr2[]= {1.3f,3.5f, 1.1f, -20.0f};
            printResult(xxArr == xx,102);
            printResult(xxArr2 != xx,103);
            printResult(xx3 != xx,104);
            printResult(xx2 == xx,105);

            // +
            float4 xx4 = xx + xx2;
            printResult( VALUE_EQUAL<float>(xx4.x,2.4f) && VALUE_EQUAL<float>(xx4.y, 7.0f) && VALUE_EQUAL<float>(xx4.z, 3.0f) && VALUE_EQUAL<float>(xx4.w, 4.0f),
                112);
            float4 xx5 = xx + xxArr;
            printResult( VALUE_EQUAL<float>(xx5.x,2.4f) && VALUE_EQUAL<float>(xx5.y,7.0f) && VALUE_EQUAL<float>(xx5.z,3.0f) && VALUE_EQUAL<float>(xx5.w,4.0f),
                113);
            float4 xx6 = xxArr + xx2;
            printResult( VALUE_EQUAL<float>(xx6.x,2.4f) && VALUE_EQUAL<float>(xx6.y,7.0f) && VALUE_EQUAL<float>(xx6.z,3.0f) && VALUE_EQUAL<float>(xx6.w,4.0f),
                114);
            float4 xx7 = 6.7f + xx2;
            printResult( VALUE_EQUAL<float>(xx7.x,7.9f) && VALUE_EQUAL<float>(xx7.y, 10.2f) && VALUE_EQUAL<float>(xx7.z, 8.2f) && VALUE_EQUAL<float>(xx7.w, 8.7f),
                115);

            // -
            {
                float4 mm1(10.0f,12.0f,6.0f, 10.0f);
                float4 mm2(5.0f, 4.0f, 2.0f, 5.0f);
                float mmArr[] = {3.0f,2.5f, 10.0f, 2.0f};
                float4 mm12a = mm1 - mm2;
                float4 mm12b = mm1 - mmArr;
                float4 mm12c = mmArr - mm1;
                float4 mm12d = 3.0f - mm1;
                printResult(VALUE_EQUAL<float>(mm12a.x,5.0f) && VALUE_EQUAL<float>(mm12a.y,8.0f) && VALUE_EQUAL<float>(mm12a.z,4.0f) && VALUE_EQUAL<float>(mm12a.w,5.0f) ,
                    116);
                printResult(VALUE_EQUAL<float>(mm12b.x,7.0f) && VALUE_EQUAL<float>(mm12b.y,9.5f) && VALUE_EQUAL<float>(mm12b.z,-4.0f) && VALUE_EQUAL<float>(mm12b.w,8.0f),
                    116);
                printResult(VALUE_EQUAL<float>(mm12c.x,-7.0f) && VALUE_EQUAL<float>(mm12c.y,-9.5f) && VALUE_EQUAL<float>(mm12c.z,4.0f) && VALUE_EQUAL<float>(mm12c.w,-8.0f) ,
                    117);
                printResult(VALUE_EQUAL<float>(mm12d.x,-7.0f) && VALUE_EQUAL<float>(mm12d.y,-9.0f) && VALUE_EQUAL<float>(mm12d.z,-3.0f) && VALUE_EQUAL<float>(mm12d.w,-7.0f) ,
                    118);
            }

            // *
            {
                float4 mm1(10.0f,12.0f,6.0f, 2.0f);
                float4 mm2(5.0f, 4.0f, 2.0f,-5.0f);
                float mmArr[] = {3.0f,2.5f, 10.0f,-20.0f};
                float k = 10.0f;
                float4 m1 = mm1 * mm2;
                float4 m2 = mm1 * mmArr;
                float4 m3 = mm1 * k;
                float4 m4 = mmArr * mm1;
                float4 m5 = k * mm1;
                printResult(VALUE_EQUAL<float>(m1.x,50.0f) && VALUE_EQUAL<float>(m1.y,48.0f) && VALUE_EQUAL<float>(m1.z,12.0f) && VALUE_EQUAL<float>(m1.w,-10.0f) ,
                    119);
                printResult(VALUE_EQUAL<float>(m2.x,30.0f) && VALUE_EQUAL<float>(m2.y,30.0f) && VALUE_EQUAL<float>(m2.z,60.0f) && VALUE_EQUAL<float>(m2.w,-40.0f) ,
                    120);
                printResult(VALUE_EQUAL<float>(m3.x,100.0f) && VALUE_EQUAL<float>(m3.y,120.0f) && VALUE_EQUAL<float>(m3.z,60.0f) && VALUE_EQUAL<float>(m3.w,20.0f) ,
                    121);
                printResult(VALUE_EQUAL<float>(m4.x,30.0f) && VALUE_EQUAL<float>(m4.y,30.0f) && VALUE_EQUAL<float>(m4.z,60.0f) && VALUE_EQUAL<float>(m4.w,-40.0f) ,
                    122);
                printResult(VALUE_EQUAL<float>(m5.x,100.0f) && VALUE_EQUAL<float>(m5.y,120.0f) && VALUE_EQUAL<float>(m5.z,60.0f) && VALUE_EQUAL<float>(m5.w,20.0f) ,
                    123);
            }

            // /
            {
                float4 mm1(10.0f,100.0f,20.0f, 10.0f);
                float4 mm2(5.0f, 4.0f, 2.0f,20.0f);
                float mmArr[] = {5.0f,2.0f, 10.0f,100.0f};
                float k = 10.0f;
                float4 m1 = mm1 / mm2;
                float4 m2 = mm1 / mmArr;
                float4 m3 = mm1 / k;
                float4 m4 = mmArr / mm1;
                float4 m5 = k / mm1;
                printResult(VALUE_EQUAL<float>(m1.x,2.0f) && VALUE_EQUAL<float>(m1.y,25.0f) && VALUE_EQUAL<float>(m1.z,10.0f) && VALUE_EQUAL<float>(m1.w,0.5f) ,
                    124);
                printResult(VALUE_EQUAL<float>(m2.x,2.0f) && VALUE_EQUAL<float>(m2.y,50.0f) && VALUE_EQUAL<float>(m2.z,2.0f) && VALUE_EQUAL<float>(m2.w,0.1f) ,
                    125);
                printResult(VALUE_EQUAL<float>(m3.x,1.0f) && VALUE_EQUAL<float>(m3.y,10.0f) && VALUE_EQUAL<float>(m3.z,2.0f) && VALUE_EQUAL<float>(m3.w,1.0f) ,
                    126);
                printResult(VALUE_EQUAL<float>(m4.x,0.5f) && VALUE_EQUAL<float>(m4.y,0.02f) && VALUE_EQUAL<float>(m4.z,0.5f) && VALUE_EQUAL<float>(m4.w,10.0f) ,
                    127);
                printResult(VALUE_EQUAL<float>(m5.x,1.0f) && VALUE_EQUAL<float>(m5.y,0.1f) && VALUE_EQUAL<float>(m5.z,0.5f) && VALUE_EQUAL<float>(m5.w,1.0f) ,
                    128);
            }

            // +=
            {
                float4 mm1(10.0f,20.0f, 20.0f, -2.0f);
                float4 mm2(5.0f, 10.0f, 100.0f, -3.0f);
                float mmArr[] = {8.0f,5.0f,0.0f, 12.15f};
                mm1 += mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,15.0f) && VALUE_EQUAL<float>(mm1.y,30.0f) && VALUE_EQUAL<float>(mm1.z,120.0f) && VALUE_EQUAL<float>(mm1.w,-5.0f) ,
                    129);
                mm1 += mmArr;
                mmArr += mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,23.0f) && VALUE_EQUAL<float>(mm1.y,35.0f) && VALUE_EQUAL<float>(mm1.z,120.0f) && VALUE_EQUAL<float>(mm1.w,7.15f) ,
                    130);
                printResult(VALUE_EQUAL<float>(mmArr[0],13.0f) && VALUE_EQUAL<float>(mmArr[1],15.0f) && VALUE_EQUAL<float>(mmArr[2],100.0f) && VALUE_EQUAL<float>(mmArr[3],9.15f) ,
                    131);
            }

            // -=
            {
                float4 mm1(10.0f,20.0f, 15.0f, 2.0f);
                float4 mm2(5.0f, 10.0f, 7.0f, 3.0f);
                float mmArr[] = {8.0f,5.0f, 13.0f,-5.5f};
                mm1 -= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,5.0f) && VALUE_EQUAL<float>(mm1.y,10.0f) && VALUE_EQUAL<float>(mm1.z,8.0f) && VALUE_EQUAL<float>(mm1.w,-1.0f) ,
                    132);
                mm1 -= mmArr;
                mmArr -= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,-3.0f) && VALUE_EQUAL<float>(mm1.y,5.0f) && VALUE_EQUAL<float>(mm1.z,-5.0f) && VALUE_EQUAL<float>(mm1.w,4.5f) ,
                    133);
                printResult(VALUE_EQUAL<float>(mmArr[0],3.0f) && VALUE_EQUAL<float>(mmArr[1],-5.0f) && VALUE_EQUAL<float>(mmArr[2],6.0f) && VALUE_EQUAL<float>(mmArr[3],-8.5f) ,
                    134);
            }

            // *=
            {
                float4 mm1(10.0f,20.0f, 5.0f, 10.0f);
                float4 mm2(5.0f, 10.0f, 10.0f, -5.0f);
                float mmArr[] = {8.0f,5.0f, -2.0f, -10.0f};
                float k = -20.0f;
                mm1 *= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,50.0f) && VALUE_EQUAL<float>(mm1.y,200.0f) && VALUE_EQUAL<float>(mm1.z,50.0f) && VALUE_EQUAL<float>(mm1.w,-50.0f),
                    135);
                mm1 *= mmArr;
                printResult(VALUE_EQUAL<float>(mm1.x,400.0f) && VALUE_EQUAL<float>(mm1.y,1000.0f) && VALUE_EQUAL<float>(mm1.z,-100.0f) && VALUE_EQUAL<float>(mm1.w,500.0f),
                    136);
                mm2 *= k;
                printResult(VALUE_EQUAL<float>(mm2.x,-100.0f) && VALUE_EQUAL<float>(mm2.y,-200.0f) && VALUE_EQUAL<float>(mm2.z,-200.0f) && VALUE_EQUAL<float>(mm2.w,100.0f),
                    137);
                mmArr *= mm2;
                printResult(VALUE_EQUAL<float>(mmArr[0],-800.0f) && VALUE_EQUAL<float>(mmArr[1],-1000.0f) && VALUE_EQUAL<float>(mmArr[2],400.0f) && VALUE_EQUAL<float>(mmArr[3],-1000.0f),
                    138);
            }

            // /=
            {
                float4 mm1(100.0f,200.0f,10.0f,10.0f);
                float4 mm2(5.0f, 10.0f,-20.0f,20.0f);
                float mmArr[] = {2.0f,5.0f,-100.0f,100.0f};
                float k = -2.0f;
                mm1 /= mm2;
                printResult(VALUE_EQUAL<float>(mm1.x,20.0f) && VALUE_EQUAL<float>(mm1.y,20.0f) && VALUE_EQUAL<float>(mm1.z,-0.5f) && VALUE_EQUAL<float>(mm1.w,0.5f) ,
                    139);
                mm1 /= mmArr;
                printResult(VALUE_EQUAL<float>(mm1.x,10.0f) && VALUE_EQUAL<float>(mm1.y,4.0f) && VALUE_EQUAL<float>(mm1.z,0.005f) && VALUE_EQUAL<float>(mm1.w,0.005f),
                    140);
                mm2 /= k;
                printResult(VALUE_EQUAL<float>(mm2.x,-2.5f) && VALUE_EQUAL<float>(mm2.y,-5.0f) && VALUE_EQUAL<float>(mm2.z,10.0f) && VALUE_EQUAL<float>(mm2.w,-10.0f),
                    141);
                mmArr /= mm2;
                printResult(VALUE_EQUAL<float>(mmArr[0],-0.8f) && VALUE_EQUAL<float>(mmArr[1],-1.0f) && VALUE_EQUAL<float>(mmArr[2],-10.0f) && VALUE_EQUAL<float>(mmArr[3],-10.0f),
                    142);
            }

            // length
            {
                float4 a(3.0f,4.0f,12.0f,1.0f);
                printResult(VALUE_EQUAL<double>(a.length(),sqrt(170.0)) ,106);
            }

            // squared length
            {
                float4 a(3.0f,4.0f,12.0f, 1.0f);
                printResult(VALUE_EQUAL<double>(a.squaredLength(),170.0) ,107);
            }

            // dot product
            {
                float4 a(3.0f, 4.0f, 12.0f, 2.0f);
                float4 b(10.0f, 5.0f, 10.0f, 5.0f);
                double c = a.dot(b);
                printResult(VALUE_EQUAL<double>(c,180.0f) ,108);
            }

            // normalize
            {
                float4 a(3.0f, 4.0f, 12.0f, 1.0f);
                float4 b = a.normalize();
                printResult(VALUE_EQUAL<double>(a.x,3.0f) && VALUE_EQUAL<double>(a.y,4.0f) && VALUE_EQUAL<double>(a.z,12.0f) && VALUE_EQUAL<double>(a.w,1.0f),
                    143);
                printResult(VALUE_EQUAL<double>(b.x,3.0f/sqrt(170.0)) && VALUE_EQUAL<double>(b.y,4.0f/sqrt(170.0)) && VALUE_EQUAL<double>(b.z,12.0f/sqrt(170.0)) && VALUE_EQUAL<double>(b.w,1.0f/sqrt(170.0)),
                    144);
                a.normalizeIt();
                printResult(VALUE_EQUAL<double>(a.x,b.x) && VALUE_EQUAL<double>(a.y,b.y) && VALUE_EQUAL<double>(a.z,b.z) && VALUE_EQUAL<double>(a.w,b.w),
                    145);
            }
        }


    }
}

