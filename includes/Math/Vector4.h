#ifndef VECTOR4_H
#define VECTOR4_H

#include "Common/ZHSTD.h"
#include <assert.h>

#ifndef common_vector4_export_defines
#define common_vector4_export_defines( CLASS, CLASS_VEC4 ) \
        ZH_TEMPLATE_DLL template class ZH_DLL Vector4<CLASS> ; \
        typedef Vector4<CLASS> CLASS_VEC4; \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator == ( const CLASS* const, const Vector4<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator != ( const CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator + ( const CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator + ( const CLASS&, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator - ( const CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator - ( const CLASS&, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator * ( const CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator * ( const CLASS&, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator / ( const CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector4<CLASS> operator / ( const CLASS&, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator += ( CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator -= ( CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator *= ( CLASS* const, const Vector4<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator /= ( CLASS* const, const Vector4<CLASS>& );
#endif

namespace ZH{
    namespace Math{

        template <class T>
        class Vector4{
        public:
            union {
                struct{T x;T y;T z;T w;};
                struct{T r;T g;T b;T a;};
                T val[4];
            };
        public:
            Vector4();
            Vector4( T, T, T, T);
            Vector4( const Vector4& );
            Vector4( const T* const );
            T operator [] ( unsigned int )const;
            T& operator [] ( unsigned int );
            Vector4& operator = ( const Vector4& );
            Vector4& operator = ( const T* const );

            bool operator == ( const Vector4& ) const;
            bool operator == ( const T* const ) const;
            bool operator != ( const Vector4& ) const;
            bool operator != ( const T* const ) const;
            Vector4 operator + ( const Vector4& ) const;
            Vector4 operator + ( const T* const ) const;
            Vector4 operator - ( const Vector4& ) const;
            Vector4 operator - ( const T* const ) const;
            Vector4 operator * ( const Vector4& ) const;
            Vector4 operator * ( const T* const ) const;
            Vector4 operator * ( const T& ) const;
            Vector4 operator / ( const Vector4& ) const;
            Vector4 operator / ( const T* const ) const;
            Vector4 operator / ( const T& ) const;
            void operator += ( const Vector4& );
            void operator += ( const T* const );
            void operator -= ( const Vector4& );
            void operator -= ( const T* const );
            void operator *= ( const Vector4& );
            void operator *= ( const T* const );
            void operator *= ( const T& );
            void operator /= ( const Vector4& );
            void operator /= ( const T* const );
            void operator /= ( const T& );


            void normalizeIt();
            Vector4 normalize() const;
            double length() const;
            double squaredLength() const;
            double dot( const Vector4& ) const;

        }; // class Vector4

        // ==
        template <class T>
        bool operator == ( const T* const, const Vector4<T>& );
        // !=
        template <class T>
        bool operator != ( const T* const, const Vector4<T>& );
        // +
        template <class T>
        Vector4<T> operator + ( const T* const, const Vector4<T>& );
        template <class T>
        Vector4<T> operator + ( const T&, const Vector4<T>& );
        // -
        template <class T>
        Vector4<T> operator - ( const T* const, const Vector4<T>& );
        template <class T>
        Vector4<T> operator - ( const T&, const Vector4<T>& );
        // *
        template <class T>
        Vector4<T> operator * ( const T* const, const Vector4<T>& );
        template <class T>
        Vector4<T> operator * ( const T&, const Vector4<T>& );
        // /
        template <class T>
        Vector4<T> operator / ( const T* const, const Vector4<T>& );
        template <class T>
        Vector4<T> operator / ( const T&, const Vector4<T>& );
        // +=
        template <class T>
        void operator += ( T* const, const Vector4<T>& );
        // -=
        template <class T>
        void operator -= ( T* const, const Vector4<T>& );
        // *=
        template <class T>
        void operator *= ( T* const, const Vector4<T>& );
        // /=
        template <class T>
        void operator /= ( T* const, const Vector4<T>& );

        // Exports
        common_vector4_export_defines( float, float4 )
        common_vector4_export_defines( double, double4 )
        common_vector4_export_defines( int, int4 )
        common_vector4_export_defines( unsigned int, uint4 )
        common_vector4_export_defines( short, short4 )
        common_vector4_export_defines( unsigned int short, ushort4 )

    } // namespace Math
} // namespace ZH

#endif



