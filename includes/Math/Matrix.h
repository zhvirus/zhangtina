#ifndef MATRIX_H
#define MATRIX_H

#include "Common/ZHSTD.h"

#ifndef common_matrix_export_defines
#define common_matrix_export_defines( CLASS, CLASS_MATRIX ) \
        ZH_TEMPLATE_DLL template class ZH_DLL Matrix<CLASS> ; \
        typedef Matrix<CLASS> CLASS_MATRIX; \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator == ( const CLASS* const, const Matrix<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator == ( CLASS, const Matrix<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator != ( const CLASS* const, const Matrix<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator != ( CLASS, const Matrix<CLASS>& );

#endif

namespace ZH{
    namespace Math{

        template <class T>
        class Matrix{
        public:
            ~Matrix(){}
            Matrix();
            Matrix( const Matrix& );
            Matrix( const T* const );
            Matrix( T );
            Matrix( T,T,T,T,
                    T,T,T,T,
                    T,T,T,T,
                    T,T,T,T);

            // inline
            inline T& operator [](unsigned int i){ return v[i];}

            // =
            Matrix& operator = ( const Matrix& );
            Matrix& operator = ( const T* const );
            Matrix& operator = ( T );
            // ==
            bool operator == ( const Matrix& ) const;
            bool operator == ( const T* const ) const;
            bool operator == ( T ) const;
            // !=
            bool operator != ( const Matrix& ) const;
            bool operator != ( const T* const ) const;
            bool operator != ( T ) const;
            // +
            Matrix operator+(const Matrix&) const;
            Matrix operator+(const T* const) const;
            Matrix operator+(T)const;
            // -
            Matrix operator-(const Matrix&) const;
            Matrix operator-(const T* const) const;
            Matrix operator-(T) const;
            // *
            Matrix operator*(const Matrix&) const;
            Matrix operator*(const T* const) const;
            Matrix operator*(T) const;
            // /
            Matrix operator/(const Matrix&) const;
            Matrix operator/(const T* const) const;
            Matrix operator/(T) const;

            // http://en.wikipedia.org/wiki/Matrix_(mathematics)
            //
            void identityIt();
            void transposeIt();
            bool inverseIt();
            Matrix transposeMatrix()const;
            Matrix inverseMatrix()const;    
            bool isIdentity()const;
            Matrix<T> Matrix<T>::adjoint() const;
            T determinant() const;

        public:
            static const Matrix<T> ZERO;
            static const Matrix<T> IDENTITY;
            union{
                T v[16];
                T _v[4][4];
            };

        static T MINOR(const Matrix<T>&, int,int,int,int,int,int);
            
        };

        // ==
        template <class T>
        bool operator == ( const T* const, const Matrix<T>& );
        template <class T>
        bool operator == ( T, const Matrix<T>& );
        // !=
        template <class T>
        bool operator != ( const T* const, const Matrix<T>& );
        template <class T>
        bool operator != ( T, const Matrix<T>& );

 
    }
} // namespace ZH


#endif