#ifndef VECTOR2_H
#define VECTOR2_H

#include "Common/ZHSTD.h"
#include <assert.h>

#ifndef common_vector2_export_defines
#define common_vector2_export_defines( CLASS, CLASS_VEC2 ) \
        ZH_TEMPLATE_DLL template class ZH_DLL Vector2<CLASS> ; \
        typedef Vector2<CLASS> CLASS_VEC2; \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator == ( const CLASS* const, const Vector2<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator != ( const CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator + ( const CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator + ( const CLASS&, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator - ( const CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator - ( const CLASS&, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator * ( const CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator * ( const CLASS&, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator / ( const CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector2<CLASS> operator / ( const CLASS&, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator += ( CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator -= ( CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator *= ( CLASS* const, const Vector2<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator /= ( CLASS* const, const Vector2<CLASS>& );
#endif

namespace ZH{
    namespace Math{

        template <class T>
        class Vector2{
        public:
            union {
                struct{T x;T y;};
                struct{T r;T g;};
                T val[2];
            };
        public:
            Vector2();
            Vector2( T, T);
            Vector2( const Vector2& );
            Vector2( const T* const );
            T operator [] ( unsigned int )const;
            T& operator [] ( unsigned int );
            Vector2& operator = ( const Vector2& );
            Vector2& operator = ( const T* const );

            bool operator == ( const Vector2& ) const;
            bool operator == ( const T* const ) const;
            bool operator != ( const Vector2& ) const;
            bool operator != ( const T* const ) const;
            Vector2 operator + ( const Vector2& ) const;
            Vector2 operator + ( const T* const ) const;
            Vector2 operator - ( const Vector2& ) const;
            Vector2 operator - ( const T* const ) const;
            Vector2 operator * ( const Vector2& ) const;
            Vector2 operator * ( const T* const ) const;
            Vector2 operator * ( const T& ) const;
            Vector2 operator / ( const Vector2& ) const;
            Vector2 operator / ( const T* const ) const;
            Vector2 operator / ( const T& ) const;
            void operator += ( const Vector2& );
            void operator += ( const T* const );
            void operator -= ( const Vector2& );
            void operator -= ( const T* const );
            void operator *= ( const Vector2& );
            void operator *= ( const T* const );
            void operator *= ( const T& );
            void operator /= ( const Vector2& );
            void operator /= ( const T* const );
            void operator /= ( const T& );

            double length() const;
            double squaredLength() const;
            double dot( const Vector2& ) const;
        };

        // ==
        template <class T>
        bool operator == ( const T* const, const Vector2<T>& );
        // !=
        template <class T>
        bool operator != ( const T* const, const Vector2<T>& );
        // +
        template <class T>
        Vector2<T> operator + ( const T* const, const Vector2<T>& );
        template <class T>
        Vector2<T> operator + ( const T&, const Vector2<T>& );
        // -
        template <class T>
        Vector2<T> operator - ( const T* const, const Vector2<T>& );
        template <class T>
        Vector2<T> operator - ( const T&, const Vector2<T>& );
        // *
        template <class T>
        Vector2<T> operator * ( const T* const, const Vector2<T>& );
        template <class T>
        Vector2<T> operator * ( const T&, const Vector2<T>& );
        // /
        template <class T>
        Vector2<T> operator / ( const T* const, const Vector2<T>& );
        template <class T>
        Vector2<T> operator / ( const T&, const Vector2<T>& );
        // +=
        template <class T>
        void operator += ( T* const, const Vector2<T>& );
        // -=
        template <class T>
        void operator -= ( T* const, const Vector2<T>& );
        // *=
        template <class T>
        void operator *= ( T* const, const Vector2<T>& );
        // /=
        template <class T>
        void operator /= ( T* const, const Vector2<T>& );


        // Exports
        common_vector2_export_defines( float, float2 )
        common_vector2_export_defines( double, double2 )
        common_vector2_export_defines( int, int2 )
        common_vector2_export_defines( unsigned int, uint2 )
        common_vector2_export_defines( short, short2 )
        common_vector2_export_defines( unsigned int short, ushort2 )

    }
} // namespace ZH


#endif
