#ifndef VECTOR3_H
#define VECTOR3_H

#include "Common/ZHSTD.h"
#include <assert.h>

#ifndef common_vector3_export_defines
#define common_vector3_export_defines( CLASS, CLASS_VEC3 ) \
        ZH_TEMPLATE_DLL template class ZH_DLL Vector3<CLASS> ; \
        typedef Vector3<CLASS> CLASS_VEC3; \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator == ( const CLASS* const, const Vector3<CLASS>& ); \
        ZH_TEMPLATE_DLL template ZH_DLL bool operator != ( const CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator + ( const CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator + ( const CLASS&, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator - ( const CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator - ( const CLASS&, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator * ( const CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator * ( const CLASS&, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator / ( const CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL Vector3<CLASS> operator / ( const CLASS&, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator += ( CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator -= ( CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator *= ( CLASS* const, const Vector3<CLASS>& );\
        ZH_TEMPLATE_DLL template ZH_DLL void operator /= ( CLASS* const, const Vector3<CLASS>& );
#endif

namespace ZH{
    namespace Math{

        template <class T>
        class Vector3{
        public:
            union {
                struct{T x;T y;T z;};
                struct{T r;T g;T b;};
                T val[3];
            };
        public:
            Vector3();
            Vector3( T, T, T);
            Vector3( const Vector3& );
            Vector3( const T* const );
            T operator [] ( unsigned int )const;
            T& operator [] ( unsigned int );
            Vector3& operator = ( const Vector3& );
            Vector3& operator = ( const T* const );

            bool operator == ( const Vector3& ) const;
            bool operator == ( const T* const ) const;
            bool operator != ( const Vector3& ) const;
            bool operator != ( const T* const ) const;
            Vector3 operator + ( const Vector3& ) const;
            Vector3 operator + ( const T* const ) const;
            Vector3 operator - ( const Vector3& ) const;
            Vector3 operator - ( const T* const ) const;
            Vector3 operator * ( const Vector3& ) const;
            Vector3 operator * ( const T* const ) const;
            Vector3 operator * ( const T& ) const;
            Vector3 operator / ( const Vector3& ) const;
            Vector3 operator / ( const T* const ) const;
            Vector3 operator / ( const T& ) const;
            void operator += ( const Vector3& );
            void operator += ( const T* const );
            void operator -= ( const Vector3& );
            void operator -= ( const T* const );
            void operator *= ( const Vector3& );
            void operator *= ( const T* const );
            void operator *= ( const T& );
            void operator /= ( const Vector3& );
            void operator /= ( const T* const );
            void operator /= ( const T& );


            void normalizeIt();
            Vector3 normalize() const;
            double length() const;
            double squaredLength() const;
            double dot( const Vector3& ) const;
            Vector3 cross( const Vector3& ) const;

        }; // class Vector3

        // ==
        template <class T>
        bool operator == ( const T* const, const Vector3<T>& );
        // !=
        template <class T>
        bool operator != ( const T* const, const Vector3<T>& );
        // +
        template <class T>
        Vector3<T> operator + ( const T* const, const Vector3<T>& );
        template <class T>
        Vector3<T> operator + ( const T&, const Vector3<T>& );
        // -
        template <class T>
        Vector3<T> operator - ( const T* const, const Vector3<T>& );
        template <class T>
        Vector3<T> operator - ( const T&, const Vector3<T>& );
        // *
        template <class T>
        Vector3<T> operator * ( const T* const, const Vector3<T>& );
        template <class T>
        Vector3<T> operator * ( const T&, const Vector3<T>& );
        // /
        template <class T>
        Vector3<T> operator / ( const T* const, const Vector3<T>& );
        template <class T>
        Vector3<T> operator / ( const T&, const Vector3<T>& );
        // +=
        template <class T>
        void operator += ( T* const, const Vector3<T>& );
        // -=
        template <class T>
        void operator -= ( T* const, const Vector3<T>& );
        // *=
        template <class T>
        void operator *= ( T* const, const Vector3<T>& );
        // /=
        template <class T>
        void operator /= ( T* const, const Vector3<T>& );

        // Exports
        common_vector3_export_defines( float, float3 )
        common_vector3_export_defines( double, double3 )
        common_vector3_export_defines( int, int3 )
        common_vector3_export_defines( unsigned int, uint3 )
        common_vector3_export_defines( short, short3 )
        common_vector3_export_defines( unsigned int short, ushort3 )

    } // namespace Math
} // namespace ZH

#endif


