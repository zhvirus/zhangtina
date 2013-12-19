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

            // inline
            inline T& operator [](unsigned int i){ return v[i];}
            inline T& operator [][](unsigned int i,unsigned int j){ return v[i*4+j];}

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
            bool inverseMatrix( Matrix& )const;
            
            bool isDiagonal()const;
            bool isUpperTriangular()const;
            bool isLowerTriangular()const;
            bool isIdentity()const;
            bool isInvertible()const;
            bool isOrthogonal()const;

            T traceValue()const;
        public:
            T v[16];
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

        // Exports
        common_matrix_export_defines( float,  matrix_f );
        common_matrix_export_defines( double, matrix_d );
    }
} // namespace ZH


#endif