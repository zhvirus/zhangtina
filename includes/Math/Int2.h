#ifndef INT2_H
#define INT2_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_MATH_DLL int2{
        public:
#pragma warning( push )
#pragma warning( disable : 4201 )
            union {
                struct{int x;int y;};
                struct{int r;int g;};
                int val[2];
            };
#pragma warning( pop ) 
        public:
            int2();
            int2( int, int);
            int2( const int2& );
            int2( const int* const );
            int operator [] ( unsigned int )const;
            int& operator [] ( unsigned int );
            int2& operator = ( const int2& );
            int2& operator = ( const int* const );

            bool operator == ( const int2& ) const;
            bool operator == ( const int* const ) const;
            bool operator != ( const int2& ) const;
            bool operator != ( const int* const ) const;
            int2 operator + ( const int2& ) const;
            int2 operator + ( const int* const ) const;
            int2 operator - ( const int2& ) const;
            int2 operator - ( const int* const ) const;
            int2 operator * ( const int2& ) const;
            int2 operator * ( const int* const ) const;
            int2 operator * ( const int& ) const;
            int2 operator / ( const int2& ) const;
            int2 operator / ( const int* const ) const;
            int2 operator / ( const int& ) const;
            void operator += ( const int2& );
            void operator += ( const int* const );
            void operator -= ( const int2& );
            void operator -= ( const int* const );
            void operator *= ( const int2& );
            void operator *= ( const int* const );
            void operator *= ( const int& );
            void operator /= ( const int2& );
            void operator /= ( const int* const );
            void operator /= ( const int& );

            float length() const;
            int squaredLength() const;
            int dot( const int2& ) const;
        };

        // ==
        ZH_MATH_DLL bool operator == ( const int* const, const int2& );
        // !=
        ZH_MATH_DLL bool operator != ( const int* const, const int2& );
        // +
        ZH_MATH_DLL int2 operator + ( const int* const, const int2& );
        ZH_MATH_DLL int2 operator + ( const int&, const int2& );
        // -
        ZH_MATH_DLL int2 operator - ( const int* const, const int2& );
        ZH_MATH_DLL int2 operator - ( const int&, const int2& );
        // *
        ZH_MATH_DLL int2 operator * ( const int* const, const int2& );
        ZH_MATH_DLL int2 operator * ( const int&, const int2& );
        // /
        ZH_MATH_DLL int2 operator / ( const int* const, const int2& );
        ZH_MATH_DLL int2 operator / ( const int&, const int2& );
        // +=
        ZH_MATH_DLL void operator += ( int* const, const int2& );
        // -=
        ZH_MATH_DLL void operator -= ( int* const, const int2& );
        // *=
        ZH_MATH_DLL void operator *= ( int* const, const int2& );
        // /=
        ZH_MATH_DLL void operator /= ( int* const, const int2& );

    }
} // namespace ZH


#endif
