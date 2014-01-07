#ifndef INT4_H
#define INT4_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_DLL int4{
        public:
            union {
                struct{int x;int y;int z;int w;};
                struct{int r;int g;int b;int a;};
                int val[4];
            };
        public:
            int4();
            int4( int, int, int, int);
            int4( const int4& );
            int4( const int* const );
            int operator [] ( unsigned int )const;
            int& operator [] ( unsigned int );
            int4& operator = ( const int4& );
            int4& operator = ( const int* const );

            bool operator == ( const int4& ) const;
            bool operator == ( const int* const ) const;
            bool operator != ( const int4& ) const;
            bool operator != ( const int* const ) const;
            int4 operator + ( const int4& ) const;
            int4 operator + ( const int* const ) const;
            int4 operator - ( const int4& ) const;
            int4 operator - ( const int* const ) const;
            int4 operator * ( const int4& ) const;
            int4 operator * ( const int* const ) const;
            int4 operator * ( const int& ) const;
            int4 operator / ( const int4& ) const;
            int4 operator / ( const int* const ) const;
            int4 operator / ( const int& ) const;
            void operator += ( const int4& );
            void operator += ( const int* const );
            void operator -= ( const int4& );
            void operator -= ( const int* const );
            void operator *= ( const int4& );
            void operator *= ( const int* const );
            void operator *= ( const int& );
            void operator /= ( const int4& );
            void operator /= ( const int* const );
            void operator /= ( const int& );


            void normalizeIt();
            int4 normalize() const;
            float length() const;
            int squaredLength() const;
            int dot( const int4& ) const;

        }; // class int4

        // ==
        ZH_DLL bool operator == ( const int* const, const int4& );

        // !=
        ZH_DLL bool operator != ( const int* const, const int4& );

        // +
        ZH_DLL int4 operator + ( const int* const, const int4& );

        ZH_DLL int4 operator + ( const int&, const int4& );

        // -
        ZH_DLL int4 operator - ( const int* const, const int4& );

        ZH_DLL int4 operator - ( const int&, const int4& );

        // *
        ZH_DLL int4 operator * ( const int* const, const int4& );

        ZH_DLL int4 operator * ( const int&, const int4& );

        // /
        ZH_DLL int4 operator / ( const int* const, const int4& );

        ZH_DLL int4 operator / ( const int&, const int4& );

        // +=
        ZH_DLL void operator += ( int* const, const int4& );

        // -=
        ZH_DLL void operator -= ( int* const, const int4& );

        // *=
        ZH_DLL void operator *= ( int* const, const int4& );

        // /=
        ZH_DLL void operator /= ( int* const, const int4& );


    } // namespace Math
} // namespace ZH

#endif



