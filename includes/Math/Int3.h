#ifndef INT3_H
#define INT3_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_MATH_DLL int3{
        public:
#pragma warning( push )
#pragma warning( disable : 4201 )
            union {
                struct{int x;int y;int z;};
                struct{int r;int g;int b;};
                int val[3];
            };
#pragma warning( pop ) 
        public:
            int3();
            int3( int, int, int);
            int3( const int3& );
            int3( const int* const );
            int operator [] ( unsigned int )const;
            int& operator [] ( unsigned int );
            int3& operator = ( const int3& );
            int3& operator = ( const int* const );

            bool operator == ( const int3& ) const;
            bool operator == ( const int* const ) const;
            bool operator != ( const int3& ) const;
            bool operator != ( const int* const ) const;
            int3 operator + ( const int3& ) const;
            int3 operator + ( const int* const ) const;
            int3 operator - ( const int3& ) const;
            int3 operator - ( const int* const ) const;
            int3 operator * ( const int3& ) const;
            int3 operator * ( const int* const ) const;
            int3 operator * ( const int& ) const;
            int3 operator / ( const int3& ) const;
            int3 operator / ( const int* const ) const;
            int3 operator / ( const int& ) const;
            void operator += ( const int3& );
            void operator += ( const int* const );
            void operator -= ( const int3& );
            void operator -= ( const int* const );
            void operator *= ( const int3& );
            void operator *= ( const int* const );
            void operator *= ( const int& );
            void operator /= ( const int3& );
            void operator /= ( const int* const );
            void operator /= ( const int& );


            void normalizeIt();
            int3 normalize() const;
            float length() const;
            int squaredLength() const;
            int dot( const int3& ) const;
            int3 cross( const int3& ) const;

        }; // class int3

        // ==
        ZH_MATH_DLL bool operator == ( const int* const, const int3& );
        // !=
        ZH_MATH_DLL bool operator != ( const int* const, const int3& );
        // +
        ZH_MATH_DLL int3 operator + ( const int* const, const int3& );
        ZH_MATH_DLL int3 operator + ( const int&, const int3& );
        // -
        ZH_MATH_DLL int3 operator - ( const int* const, const int3& );
        ZH_MATH_DLL int3 operator - ( const int&, const int3& );
        // *
        ZH_MATH_DLL int3 operator * ( const int* const, const int3& );
        ZH_MATH_DLL int3 operator * ( const int&, const int3& );
        // /
        ZH_MATH_DLL int3 operator / ( const int* const, const int3& );
        ZH_MATH_DLL int3 operator / ( const int&, const int3& );
        // +=
        ZH_MATH_DLL void operator += ( int* const, const int3& );
        // -=
        ZH_MATH_DLL void operator -= ( int* const, const int3& );
        // *=
        ZH_MATH_DLL void operator *= ( int* const, const int3& );
        // /=
        ZH_MATH_DLL void operator /= ( int* const, const int3& );

    } // namespace Math
} // namespace ZH

#endif


