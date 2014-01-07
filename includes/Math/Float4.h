#ifndef FLOAT4_H
#define FLOAT4_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_DLL float4{
        public:
            union {
                struct{float x;float y;float z;float w;};
                struct{float r;float g;float b;float a;};
                float val[4];
            };
        public:
            float4();
            float4( float, float, float, float);
            float4( const float4& );
            float4( const float* const );
            float operator [] ( unsigned int )const;
            float& operator [] ( unsigned int );
            float4& operator = ( const float4& );
            float4& operator = ( const float* const );

            bool operator == ( const float4& ) const;
            bool operator == ( const float* const ) const;
            bool operator != ( const float4& ) const;
            bool operator != ( const float* const ) const;
            float4 operator + ( const float4& ) const;
            float4 operator + ( const float* const ) const;
            float4 operator - ( const float4& ) const;
            float4 operator - ( const float* const ) const;
            float4 operator * ( const float4& ) const;
            float4 operator * ( const float* const ) const;
            float4 operator * ( const float& ) const;
            float4 operator / ( const float4& ) const;
            float4 operator / ( const float* const ) const;
            float4 operator / ( const float& ) const;
            void operator += ( const float4& );
            void operator += ( const float* const );
            void operator -= ( const float4& );
            void operator -= ( const float* const );
            void operator *= ( const float4& );
            void operator *= ( const float* const );
            void operator *= ( const float& );
            void operator /= ( const float4& );
            void operator /= ( const float* const );
            void operator /= ( const float& );


            void normalizeIt();
            float4 normalize() const;
            float length() const;
            float squaredLength() const;
            float dot( const float4& ) const;

        }; // class float4

        // ==
        ZH_DLL bool operator == ( const float* const, const float4& );

        // !=
        ZH_DLL bool operator != ( const float* const, const float4& );

        // +
        ZH_DLL float4 operator + ( const float* const, const float4& );

        ZH_DLL float4 operator + ( const float&, const float4& );

        // -
        ZH_DLL float4 operator - ( const float* const, const float4& );

        ZH_DLL float4 operator - ( const float&, const float4& );

        // *
        ZH_DLL float4 operator * ( const float* const, const float4& );

        ZH_DLL float4 operator * ( const float&, const float4& );

        // /
        ZH_DLL float4 operator / ( const float* const, const float4& );

        ZH_DLL float4 operator / ( const float&, const float4& );

        // +=
        ZH_DLL void operator += ( float* const, const float4& );

        // -=
        ZH_DLL void operator -= ( float* const, const float4& );

        // *=
        ZH_DLL void operator *= ( float* const, const float4& );

        // /=
        ZH_DLL void operator /= ( float* const, const float4& );


    } // namespace Math
} // namespace ZH

#endif



