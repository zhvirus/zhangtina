#include "Math/Vector4.h"
#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Matrix.h"
#include "Math/MathCommon.h"
#include <string.h>


namespace ZH{
    namespace Math{

        template<class T>
        T Matrix<T>::MINOR(const Matrix<T>& m, int r0, int r1, int r2, int c0, int c1, int c2)
        {
            return m._v[r0][c0] * (m._v[r1][c1] * m._v[r2][c2] - m._v[r2][c1] * m._v[r1][c2]) -
                m._v[r0][c1] * (m._v[r1][c0] * m._v[r2][c2] - m._v[r2][c0] * m._v[r1][c2]) +
                m._v[r0][c2] * (m._v[r1][c0] * m._v[r2][c1] - m._v[r2][c0] * m._v[r1][c1]);
        }

        // ---------------------------------
        // Non-class function
        // ---------------------------------
        template <class T>
        float4 operator* (const float4& v, const matrix_f& m)
        {
            return float4();
        }

        // ==
        template <class T>
        bool operator == ( const T* const t, const Matrix<T>& m )
        {
            return VALUE_EQUAL<T>(t[0], m.v[0]) && VALUE_EQUAL<T>(t[1], m.v[1]) &&
                VALUE_EQUAL<T>(t[2], m.v[2]) && VALUE_EQUAL<T>(t[3], m.v[3]) &&
                VALUE_EQUAL<T>(t[4], m.v[4]) && VALUE_EQUAL<T>(t[5], m.v[5]) &&
                VALUE_EQUAL<T>(t[6], m.v[6]) && VALUE_EQUAL<T>(t[7], m.v[7]) &&
                VALUE_EQUAL<T>(t[8], m.v[8]) && VALUE_EQUAL<T>(t[9], m.v[9]) &&
                VALUE_EQUAL<T>(t[10], m.v[10]) && VALUE_EQUAL<T>(t[11], m.v[11]) &&
                VALUE_EQUAL<T>(t[12], m.v[12]) && VALUE_EQUAL<T>(t[13], m.v[13]) &&
                VALUE_EQUAL<T>(t[14], m.v[14]) && VALUE_EQUAL<T>(t[15], m.v[15])
                ;
        }
        template <class T>
        bool operator == ( T t, const Matrix<T>& m )
        {
            return VALUE_EQUAL<T>(t, m.v[0]) && VALUE_EQUAL<T>(t, m.v[1]) &&
                VALUE_EQUAL<T>(t, m.v[2]) && VALUE_EQUAL<T>(t, m.v[3]) &&
                VALUE_EQUAL<T>(t, m.v[4]) && VALUE_EQUAL<T>(t, m.v[5]) &&
                VALUE_EQUAL<T>(t, m.v[6]) && VALUE_EQUAL<T>(t, m.v[7]) &&
                VALUE_EQUAL<T>(t, m.v[8]) && VALUE_EQUAL<T>(t, m.v[9]) &&
                VALUE_EQUAL<T>(t, m.v[10]) && VALUE_EQUAL<T>(t, m.v[11]) &&
                VALUE_EQUAL<T>(t, m.v[12]) && VALUE_EQUAL<T>(t, m.v[13]) &&
                VALUE_EQUAL<T>(t, m.v[14]) && VALUE_EQUAL<T>(t, m.v[15])
                ;
        }

        // !=
        template <class T>
        bool operator != ( const T* const t, const Matrix<T>& m )
        {
            return !(t==m);
        }
        template <class T>
        bool operator != ( T t, const Matrix<T>& m )
        {
            return !(t==m);
        }


        // ---------------------------------
        // class function
        // ---------------------------------

        template<class T>
        const Matrix<T> Matrix<T>::IDENTITY(
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1);

        template<class T>
        const Matrix<T> Matrix<T>::ZERO(
            0,0,0,0,
            0,0,0,0,
            0,0,0,0,
            0,0,0,0);


        // constructor
        template<class T>
        Matrix<T>::Matrix()
        {
            identityIt();
        }

        template<class T>
        Matrix<T>::Matrix( const Matrix<T>& m)
        {
            *this = m;
        }

        template<class T>
        Matrix<T>::Matrix( const T* const t)
        {
            *this = t;
        }

        template<class T>
        Matrix<T>::Matrix( T t)
        {
            *this = t;
        }

        template<class T>
        Matrix<T>::Matrix(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33)
        {
            v[0] = m00; v[1]=m01; v[2]=m02; v[3]=m03;
            v[4] = m10; v[5]=m11; v[6]=m12; v[7]=m13;
            v[8] = m20; v[9]=m21; v[10]=m22; v[11]=m23;
            v[12] = m30; v[13]=m31; v[14]=m32; v[15]=m33;
        }

        //---------------------------
        // =
        //---------------------------
        template<class T>
        Matrix<T>& Matrix<T>::operator = ( const Matrix<T>& m )
        {
            memcpy((void*)v, (void*)m.v, sizeof(T)*16);
            return *this;
        }
        template<class T>
        Matrix<T>& Matrix<T>::operator = ( const T* const t )
        {
            memcpy((void*)v, (void*)t, sizeof(T)*16);
            return *this;
        }
        template<class T>
        Matrix<T>& Matrix<T>::operator = ( T t )
        {
            for(unsigned int i=0;i<16;i++){
                v[i] = t;
            }
            return *this;
        }

        //---------------------------
        // ==
        //---------------------------
        template<class T>
        bool Matrix<T>::operator == ( const Matrix<T>& m ) const
        {
            return v == m;
        }
        template<class T>
        bool Matrix<T>::operator == ( const T* const t ) const
        {
            return t == *this;
        }
        template<class T>
        bool Matrix<T>::operator == ( T t ) const
        {
            return t == *this;
        }

        //---------------------------
        // !=
        //---------------------------
        template<class T>
        bool Matrix<T>::operator != ( const Matrix<T>& m ) const
        {
            return v != m;
        }
        template<class T>
        bool Matrix<T>::operator != ( const T* const t ) const
        {
            return t != *this;
        }
        template<class T>
        bool Matrix<T>::operator != ( T t ) const
        {
            return t != *this;
        }

        //---------------------------
        // +
        //---------------------------
        template<class T>
        Matrix<T> Matrix<T>::operator + ( const Matrix<T>& m ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] + m.v[i];
            }
            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator + ( const T* const t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] + t[i];
            }
            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator + ( T t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] + t;
            }
            return Matrix<T>(data);
        }

        //---------------------------
        // -
        //---------------------------
        template<class T>
        Matrix<T> Matrix<T>::operator - ( const Matrix<T>& m ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] - m.v[i];
            }
            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator - ( const T* const t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] - t[i];
            }
            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator - ( T t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] - t;
            }
            return Matrix<T>(data);
        }

        //---------------------------
        // *
        //---------------------------
        template<class T>
        Matrix<T> Matrix<T>::operator * ( const Matrix<T>& m ) const
        {
            T data[16];
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

            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator * ( const T* const t ) const
        {
            T data[16];
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

            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator * ( T t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] * t;
            }
            return Matrix<T>(data);
        }
        //---------------------------
        // /
        //---------------------------
        template<class T>
        Matrix<T> Matrix<T>::operator / ( const Matrix<T>& m ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] / m.v[i];
            }
            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator / ( const T* const t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] / t[i];
            }
            return Matrix<T>(data);
        }
        template<class T>
        Matrix<T> Matrix<T>::operator / ( T t ) const
        {
            T data[16];
            for(unsigned int i=0;i<16;i++){
                data[i] = v[i] / t;
            }
            return Matrix<T>(data);
        }

        //************************************************
        //     1  0  0  0
        //     0  1  0  0
        //     0  0  1  0
        //     0  0  0  1
        //************************************************
        template<class T>
        void Matrix<T>::identityIt()
        {
            memset((void*)(v), 0, sizeof(T)*16);
            v[0]  = T(1);
            v[5]  = T(1);
            v[10] = T(1);
            v[15] = T(1);
        }

        //************************************************
        //     m00  m01  m02  m03      m00  m10  m20  m30
        //     m10  m11  m12  m13  =>  m01  m11  m21  m31
        //     m20  m21  m22  m23      m02  m12  m22  m32
        //     m30  m31  m32  m33      m03  m13  m23  m33
        //************************************************
        template<class T>
        void Matrix<T>::transposeIt()
        {
            VALUE_SWAP<T>(v[1],v[4]);
            VALUE_SWAP<T>(v[2],v[8]);
            VALUE_SWAP<T>(v[3],v[12]);
            VALUE_SWAP<T>(v[6],v[9]);
            VALUE_SWAP<T>(v[7],v[13]);
            VALUE_SWAP<T>(v[11],v[14]);
        }
        template<class T>
        Matrix<T> Matrix<T>::transposeMatrix()const
        {
            Matrix<T> mat(*this);
            mat.transposeIt();
            return mat;
        }

        template<class T>
        Matrix<T> Matrix<T>::adjoint() const
        {
            return Matrix<T>( MINOR(*this, 1, 2, 3, 1, 2, 3),
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

        template<class T>
        T Matrix<T>::determinant() const
        {
            return v[0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
                v[1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
                v[2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
                v[3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
        }

        template<class T>
        bool Matrix<T>::inverseIt()
        {
            *this = inverseMatrix();
            return true;
        }

        template<class T>
        Matrix<T> Matrix<T>::inverseMatrix()const
        {
            return adjoint() * ((T)1.0 / determinant());
        }

        template<class T>
        bool Matrix<T>::isIdentity()const
        {
            return *this == Matrix<T>::IDENTITY;
        }

    }
}