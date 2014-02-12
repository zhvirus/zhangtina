#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif

#include "Math/Matrix4x4_d.h"
#include "Math/Double4.h"
#include "Math/MathCommon.h"
#include <string.h>


namespace ZH{
    namespace Math{

        double matrix4x4_d::MINOR(const matrix4x4_d& m, int r0, int r1, int r2, int c0, int c1, int c2)
        {
            return m._v[r0][c0] * (m._v[r1][c1] * m._v[r2][c2] - m._v[r2][c1] * m._v[r1][c2]) -
                m._v[r0][c1] * (m._v[r1][c0] * m._v[r2][c2] - m._v[r2][c0] * m._v[r1][c2]) +
                m._v[r0][c2] * (m._v[r1][c0] * m._v[r2][c1] - m._v[r2][c0] * m._v[r1][c1]);
        }

        // ---------------------------------
        // Non-class function
        // ---------------------------------


        // ==      
        bool operator == ( const double* const t, const matrix4x4_d& m )
        {
            return VALUE_EQUAL<double>(t[0], m.v[0]) && VALUE_EQUAL<double>(t[1], m.v[1]) &&
                VALUE_EQUAL<double>(t[2], m.v[2]) && VALUE_EQUAL<double>(t[3], m.v[3]) &&
                VALUE_EQUAL<double>(t[4], m.v[4]) && VALUE_EQUAL<double>(t[5], m.v[5]) &&
                VALUE_EQUAL<double>(t[6], m.v[6]) && VALUE_EQUAL<double>(t[7], m.v[7]) &&
                VALUE_EQUAL<double>(t[8], m.v[8]) && VALUE_EQUAL<double>(t[9], m.v[9]) &&
                VALUE_EQUAL<double>(t[10], m.v[10]) && VALUE_EQUAL<double>(t[11], m.v[11]) &&
                VALUE_EQUAL<double>(t[12], m.v[12]) && VALUE_EQUAL<double>(t[13], m.v[13]) &&
                VALUE_EQUAL<double>(t[14], m.v[14]) && VALUE_EQUAL<double>(t[15], m.v[15])
                ;
        }
        
        bool operator == ( double t, const matrix4x4_d& m )
        {
            return VALUE_EQUAL<double>(t, m.v[0]) && VALUE_EQUAL<double>(t, m.v[1]) &&
                VALUE_EQUAL<double>(t, m.v[2]) && VALUE_EQUAL<double>(t, m.v[3]) &&
                VALUE_EQUAL<double>(t, m.v[4]) && VALUE_EQUAL<double>(t, m.v[5]) &&
                VALUE_EQUAL<double>(t, m.v[6]) && VALUE_EQUAL<double>(t, m.v[7]) &&
                VALUE_EQUAL<double>(t, m.v[8]) && VALUE_EQUAL<double>(t, m.v[9]) &&
                VALUE_EQUAL<double>(t, m.v[10]) && VALUE_EQUAL<double>(t, m.v[11]) &&
                VALUE_EQUAL<double>(t, m.v[12]) && VALUE_EQUAL<double>(t, m.v[13]) &&
                VALUE_EQUAL<double>(t, m.v[14]) && VALUE_EQUAL<double>(t, m.v[15])
                ;
        }

        // !=    
        bool operator != ( const double* const t, const matrix4x4_d& m )
        {
            return !(t==m);
        }
        
        bool operator != ( double t, const matrix4x4_d& m )
        {
            return !(t==m);
        }

        // matrix4x4_d * double4
        double4 operator * (const matrix4x4_d& m, const double4& v)
        {
            return double4(
                m[0]*v[0]+m[1]*v[1]+m[2]*v[2]+m[3]*v[3],
                m[4]*v[0]+m[5]*v[1]+m[6]*v[2]+m[7]*v[3],
                m[8]*v[0]+m[9]*v[1]+m[10]*v[2]+m[11]*v[3],
                m[12]*v[0]+m[13]*v[1]+m[14]*v[2]+m[15]*v[3]
                );

        }

        // ---------------------------------
        // class function
        // ---------------------------------


        const matrix4x4_d matrix4x4_d::IDENTITY(
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1);


        const matrix4x4_d matrix4x4_d::ZERO(
            0,0,0,0,
            0,0,0,0,
            0,0,0,0,
            0,0,0,0);


        // constructor

        matrix4x4_d::matrix4x4_d()
        {
            identityIt();
        }


        matrix4x4_d::matrix4x4_d( const matrix4x4_d& m)
        {
            *this = m;
        }


        matrix4x4_d::matrix4x4_d( const double* const t)
        {
            *this = t;
        }


        matrix4x4_d::matrix4x4_d( double t)
        {
            *this = t;
        }


        matrix4x4_d::matrix4x4_d(
            double m00, double m01, double m02, double m03,
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33)
        {
            v[0] = m00; v[1]=m01; v[2]=m02; v[3]=m03;
            v[4] = m10; v[5]=m11; v[6]=m12; v[7]=m13;
            v[8] = m20; v[9]=m21; v[10]=m22; v[11]=m23;
            v[12] = m30; v[13]=m31; v[14]=m32; v[15]=m33;
        }

        //---------------------------
        // =
        //---------------------------

        matrix4x4_d& matrix4x4_d::operator = ( const matrix4x4_d& m )
        {
            memcpy((void*)v, (void*)m.v, sizeof(double)*16);
            return *this;
        }

        matrix4x4_d& matrix4x4_d::operator = ( const double* const t )
        {
            memcpy((void*)v, (void*)t, sizeof(double)*16);
            return *this;
        }

        matrix4x4_d& matrix4x4_d::operator = ( double t )
        {
            for(unsigned int i=0;i<16;i++){
                v[i] = t;
            }
            return *this;
        }

        //---------------------------
        // ==
        //---------------------------

        bool matrix4x4_d::operator == ( const matrix4x4_d& m ) const
        {
            return v == m;
        }

        bool matrix4x4_d::operator == ( const double* const t ) const
        {
            return t == *this;
        }

        bool matrix4x4_d::operator == ( double t ) const
        {
            return t == *this;
        }

        //---------------------------
        // !=
        //---------------------------

        bool matrix4x4_d::operator != ( const matrix4x4_d& m ) const
        {
            return v != m;
        }

        bool matrix4x4_d::operator != ( const double* const t ) const
        {
            return t != *this;
        }

        bool matrix4x4_d::operator != ( double t ) const
        {
            return t != *this;
        }

        //---------------------------
        // +
        //---------------------------

        matrix4x4_d matrix4x4_d::operator + ( const matrix4x4_d& m ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] + m.v[i];
            }
            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator + ( const double* const t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] + t[i];
            }
            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator + ( double t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] + t;
            }
            return matrix4x4_d(data);
        }

        //---------------------------
        // -
        //---------------------------

        matrix4x4_d matrix4x4_d::operator - ( const matrix4x4_d& m ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] - m.v[i];
            }
            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator - ( const double* const t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] - t[i];
            }
            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator - ( double t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] - t;
            }
            return matrix4x4_d(data);
        }

        //---------------------------
        // *
        //---------------------------

        matrix4x4_d matrix4x4_d::operator * ( const matrix4x4_d& m ) const
        {
            double data[16];
            data[0]=v[0]*m.v[0]+v[1]*m.v[4]+v[2]*m.v[8]+v[3]*m.v[12];
            data[1]=v[0]*m.v[1]+v[1]*m.v[5]+v[2]*m.v[9]+v[3]*m.v[13];
            data[2]=v[0]*m.v[2]+v[1]*m.v[6]+v[2]*m.v[10]+v[3]*m.v[14];
            data[3]=v[0]*m.v[3]+v[1]*m.v[7]+v[2]*m.v[11]+v[3]*m.v[15];

            data[4]=v[4]*m.v[0]+v[5]*m.v[4]+v[6]*m.v[8]+v[7]*m.v[12];
            data[5]=v[4]*m.v[1]+v[5]*m.v[5]+v[6]*m.v[9]+v[7]*m.v[13];
            data[6]=v[4]*m.v[2]+v[5]*m.v[6]+v[6]*m.v[10]+v[7]*m.v[14];
            data[7]=v[4]*m.v[3]+v[5]*m.v[7]+v[6]*m.v[11]+v[7]*m.v[15];

            data[8]=v[8]*m.v[0]+v[9]*m.v[4]+v[10]*m.v[8]+v[11]*m.v[12];
            data[9]=v[8]*m.v[1]+v[9]*m.v[5]+v[10]*m.v[9]+v[11]*m.v[13];
            data[10]=v[8]*m.v[2]+v[9]*m.v[6]+v[10]*m.v[10]+v[11]*m.v[14];
            data[11]=v[8]*m.v[3]+v[9]*m.v[7]+v[10]*m.v[11]+v[11]*m.v[15];

            data[12]=v[12]*m.v[0]+v[13]*m.v[4]+v[14]*m.v[8]+v[15]*m.v[12];
            data[13]=v[12]*m.v[1]+v[13]*m.v[5]+v[14]*m.v[9]+v[15]*m.v[13];
            data[14]=v[12]*m.v[2]+v[13]*m.v[6]+v[14]*m.v[10]+v[15]*m.v[14];
            data[15]=v[12]*m.v[3]+v[13]*m.v[7]+v[14]*m.v[11]+v[15]*m.v[15];

            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator * ( const double* const t ) const
        {
            double data[16];
            data[0]=v[0]*t[0]+v[1]*t[4]+v[2]*t[8]+v[3]*t[12];
            data[1]=v[0]*t[1]+v[1]*t[5]+v[2]*t[9]+v[3]*t[13];
            data[2]=v[0]*t[2]+v[1]*t[6]+v[2]*t[10]+v[3]*t[14];
            data[3]=v[0]*t[3]+v[1]*t[7]+v[2]*t[11]+v[3]*t[15];

            data[4]=v[4]*t[0]+v[5]*t[4]+v[6]*t[8]+v[7]*t[12];
            data[5]=v[4]*t[1]+v[5]*t[5]+v[6]*t[9]+v[7]*t[13];
            data[6]=v[4]*t[2]+v[5]*t[6]+v[6]*t[10]+v[7]*t[14];
            data[7]=v[4]*t[3]+v[5]*t[7]+v[6]*t[11]+v[7]*t[15];

            data[8]=v[8]*t[0]+v[9]*t[4]+v[10]*t[8]+v[11]*t[12];
            data[9]=v[8]*t[1]+v[9]*t[5]+v[10]*t[9]+v[11]*t[13];
            data[10]=v[8]*t[2]+v[9]*t[6]+v[10]*t[10]+v[11]*t[14];
            data[11]=v[8]*t[3]+v[9]*t[7]+v[10]*t[11]+v[11]*t[15];

            data[12]=v[12]*t[0]+v[13]*t[4]+v[14]*t[8]+v[15]*t[12];
            data[13]=v[12]*t[1]+v[13]*t[5]+v[14]*t[9]+v[15]*t[13];
            data[14]=v[12]*t[2]+v[13]*t[6]+v[14]*t[10]+v[15]*t[14];
            data[15]=v[12]*t[3]+v[13]*t[7]+v[14]*t[11]+v[15]*t[15];

            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator * ( double t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] * t;
            }
            return matrix4x4_d(data);
        }
        //---------------------------
        // /
        //---------------------------

        matrix4x4_d matrix4x4_d::operator / ( const matrix4x4_d& m ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] / m.v[i];
            }
            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator / ( const double* const t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] / t[i];
            }
            return matrix4x4_d(data);
        }

        matrix4x4_d matrix4x4_d::operator / ( double t ) const
        {
            double data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] / t;
            }
            return matrix4x4_d(data);
        }

        //************************************************
        //     1  0  0  0
        //     0  1  0  0
        //     0  0  1  0
        //     0  0  0  1
        //************************************************

        void matrix4x4_d::identityIt()
        {
            memset((void*)(v), 0, sizeof(double)*16);
            v[0]  = double(1);
            v[5]  = double(1);
            v[10] = double(1);
            v[15] = double(1);
        }

        //************************************************
        //     m00  m01  m02  m03      m00  m10  m20  m30
        //     m10  m11  m12  m13  =>  m01  m11  m21  m31
        //     m20  m21  m22  m23      m02  m12  m22  m32
        //     m30  m31  m32  m33      m03  m13  m23  m33
        //************************************************

        void matrix4x4_d::transposeIt()
        {
            VALUE_SWAP<double>(v[1],v[4]);
            VALUE_SWAP<double>(v[2],v[8]);
            VALUE_SWAP<double>(v[3],v[12]);
            VALUE_SWAP<double>(v[6],v[9]);
            VALUE_SWAP<double>(v[7],v[13]);
            VALUE_SWAP<double>(v[11],v[14]);
        }

        matrix4x4_d matrix4x4_d::transposeMatrix()const
        {
            matrix4x4_d mat(*this);
            mat.transposeIt();
            return mat;
        }


        matrix4x4_d matrix4x4_d::adjoint() const
        {
            return matrix4x4_d( MINOR(*this, 1, 2, 3, 1, 2, 3),
                -MINOR(*this, 0, 2, 3, 1, 2, 3),
                MINOR(*this, 0, 1, 3, 1, 2, 3),
                -MINOR(*this, 0, 1, 2, 1, 2, 3),

                -MINOR(*this, 1, 2, 3, 0, 2, 3),
                MINOR(*this, 0, 2, 3, 0, 2, 3),
                -MINOR(*this, 0, 1, 3, 0, 2, 3),
                MINOR(*this, 0, 1, 2, 0, 2, 3),

                MINOR(*this, 1, 2, 3, 0, 1, 3),
                -MINOR(*this, 0, 2, 3, 0, 1, 3),
                MINOR(*this, 0, 1, 3, 0, 1, 3),
                -MINOR(*this, 0, 1, 2, 0, 1, 3),

                -MINOR(*this, 1, 2, 3, 0, 1, 2),
                MINOR(*this, 0, 2, 3, 0, 1, 2),
                -MINOR(*this, 0, 1, 3, 0, 1, 2),
                MINOR(*this, 0, 1, 2, 0, 1, 2));
        }


        double matrix4x4_d::determinant() const
        {
            return v[0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
                v[1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
                v[2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
                v[3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
        }


        bool matrix4x4_d::inverseIt()
        {
            *this = inverseMatrix();
            return true;
        }


        matrix4x4_d matrix4x4_d::inverseMatrix()const
        {
            return adjoint() * ((double)1.0 / determinant());
        }


        bool matrix4x4_d::isIdentity()const
        {
            return *this == matrix4x4_d::IDENTITY;
        }

    }
}