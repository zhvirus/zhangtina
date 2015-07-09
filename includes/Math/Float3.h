#ifndef FLOAT3_H
#define FLOAT3_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_MATH_DLL float3{
        public:
#pragma warning( push )
#pragma warning( disable : 4201 )
            union {
                struct{float x;float y;float z;};
                struct{float r;float g;float b;};
                float val[3];
            };
#pragma warning( pop ) 

        public:
            float3();
            float3( float, float, float);
            float3( const float3& );
            float3( const float* const );
            float operator [] ( unsigned int )const;
            float& operator [] ( unsigned int );
            float3& operator = ( const float3& );
            float3& operator = ( const float* const );

            bool operator == ( const float3& ) const;
            bool operator == ( const float* const ) const;
            bool operator != ( const float3& ) const;
            bool operator != ( const float* const ) const;
            float3 operator + ( const float3& ) const;
            float3 operator + ( const float* const ) const;
            float3 operator - ( const float3& ) const;
            float3 operator - ( const float* const ) const;
            float3 operator * ( const float3& ) const;
            float3 operator * ( const float* const ) const;
            float3 operator * ( const float& ) const;
            float3 operator / ( const float3& ) const;
            float3 operator / ( const float* const ) const;
            float3 operator / ( const float& ) const;
            void operator += ( const float3& );
            void operator += ( const float* const );
            void operator -= ( const float3& );
            void operator -= ( const float* const );
            void operator *= ( const float3& );
            void operator *= ( const float* const );
            void operator *= ( const float& );
            void operator /= ( const float3& );
            void operator /= ( const float* const );
            void operator /= ( const float& );


            void normalizeIt();
            float3 normalize() const;
            float length() const;
            float squaredLength() const;
            float dot( const float3& ) const;
            float3 cross( const float3& ) const;

        }; // class float3

        // ==
        ZH_MATH_DLL bool operator == ( const float* const, const float3& );
        // !=
        ZH_MATH_DLL bool operator != ( const float* const, const float3& );
        // +
        ZH_MATH_DLL float3 operator + ( const float* const, const float3& );
        ZH_MATH_DLL float3 operator + ( const float&, const float3& );
        // -
        ZH_MATH_DLL float3 operator - ( const float* const, const float3& );
        ZH_MATH_DLL float3 operator - ( const float&, const float3& );
        ZH_MATH_DLL float3 operator - ( const float3& );
        // *
        ZH_MATH_DLL float3 operator * ( const float* const, const float3& );
        ZH_MATH_DLL float3 operator * ( const float&, const float3& );
        // /
        ZH_MATH_DLL float3 operator / ( const float* const, const float3& );
        ZH_MATH_DLL float3 operator / ( const float&, const float3& );
        // +=
        ZH_MATH_DLL void operator += ( float* const, const float3& );
        // -=
        ZH_MATH_DLL void operator -= ( float* const, const float3& );
        // *=
        ZH_MATH_DLL void operator *= ( float* const, const float3& );
        // /=
        ZH_MATH_DLL void operator /= ( float* const, const float3& );

    } // namespace Math
} // namespace ZH

#endif


