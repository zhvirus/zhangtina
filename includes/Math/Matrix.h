#ifndef MATRIX_H
#define MATRIX_H

#include "Common/ZHSTD.h"

#ifndef common_matrix_export_defines
#define common_matrix_export_defines( CLASS, CLASS_MATRIX ) \
        ZH_TEMPLATE_DLL template class ZH_DLL Matrix<CLASS> ; \
        typedef Matrix<CLASS> CLASS_MATRIX; \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator == ( const CLASS* const, const Matrix<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator != ( const CLASS* const, const Matrix<CLASS>& );
#endif

namespace ZH{
    namespace Math{

        template <class T>
        class Matrix{
        public:
            Matrix();
            Matrix( const Matrix& );
            Matrix( const T* const );
            ~Matrix(){}

            bool operator == ( const Matrix& ) const;
            bool operator != ( const Matrix& ) const;

            void identityIt();
        public:
            T v[16];
        };

        // ==
        template <class T>
        bool operator == ( const T* const, const Matrix<T>& );
        // !=
        template <class T>
        bool operator != ( const T* const, const Matrix<T>& );

        // Exports
        common_matrix_export_defines( float,  matrix_f );
        common_matrix_export_defines( double, matrix_d );
    }
} // namespace ZH


#endif