#ifndef FLOAT2_H
#define FLOAT2_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_DLL float2{
        public:
#pragma warning( push )
#pragma warning( disable : 4201 )
            union {
                struct{float x;float y;};
                struct{float r;float g;};
                float val[2];
            };
#pragma warning( pop ) 
        public:
            float2();
            float2( float, float);
            float2( const float2& );
            float2( const float* const );
            float operator [] ( unsigned int )const;
            float& operator [] ( unsigned int );
            float2& operator = ( const float2& );
            float2& operator = ( const float* const );

            bool operator == ( const float2& ) const;
            bool operator == ( const float* const ) const;
            bool operator != ( const float2& ) const;
            bool operator != ( const float* const ) const;
            float2 operator + ( const float2& ) const;
            float2 operator + ( const float* const ) const;
            float2 operator - ( const float2& ) const;
            float2 operator - ( const float* const ) const;
            float2 operator * ( const float2& ) const;
            float2 operator * ( const float* const ) const;
            float2 operator * ( const float& ) const;
            float2 operator / ( const float2& ) const;
            float2 operator / ( const float* const ) const;
            float2 operator / ( const float& ) const;
            void operator += ( const float2& );
            void operator += ( const float* const );
            void operator -= ( const float2& );
            void operator -= ( const float* const );
            void operator *= ( const float2& );
            void operator *= ( const float* const );
            void operator *= ( const float& );
            void operator /= ( const float2& );
            void operator /= ( const float* const );
            void operator /= ( const float& );

            float length() const;
            float squaredLength() const;
            float dot( const float2& ) const;
        };

        // ==
        ZH_DLL bool operator == ( const float* const, const float2& );
        // !=
        ZH_DLL bool operator != ( const float* const, const float2& );
        // +
        ZH_DLL float2 operator + ( const float* const, const float2& );
        ZH_DLL float2 operator + ( const float&, const float2& );
        // -
        ZH_DLL float2 operator - ( const float* const, const float2& );
        ZH_DLL float2 operator - ( const float&, const float2& );
        // *
        ZH_DLL float2 operator * ( const float* const, const float2& );
        ZH_DLL float2 operator * ( const float&, const float2& );
        // /
        ZH_DLL float2 operator / ( const float* const, const float2& );
        ZH_DLL float2 operator / ( const float&, const float2& );
        // +=
        ZH_DLL void operator += ( float* const, const float2& );
        // -=
        ZH_DLL void operator -= ( float* const, const float2& );
        // *=
        ZH_DLL void operator *= ( float* const, const float2& );
        // /=
        ZH_DLL void operator /= ( float* const, const float2& );

    }
} // namespace ZH


#endif
