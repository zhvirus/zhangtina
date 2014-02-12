#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Common/ZHSTD.h"

namespace ZH{
    namespace Math{

        // Forwards
        class float4;

        class ZH_MATH_DLL matrix4x4_f{
        public:
            ~matrix4x4_f(){}
            matrix4x4_f();
            matrix4x4_f( const matrix4x4_f& );
            matrix4x4_f( const float* const );
            matrix4x4_f( float );
            matrix4x4_f(
                    float,float,float,float,
                    float,float,float,float,
                    float,float,float,float,
                    float,float,float,float);

            // inline
            inline float& operator [](unsigned int i){ return v[i];}
            inline float  operator [](unsigned int i)const{ return v[i];}

            // =
            matrix4x4_f& operator = ( const matrix4x4_f& );
            matrix4x4_f& operator = ( const float* const );
            matrix4x4_f& operator = ( float );

            // ==
            bool operator == ( const matrix4x4_f& ) const;
            bool operator == ( const float* const ) const;
            bool operator == ( float ) const;

            // !=
            bool operator != ( const matrix4x4_f& ) const;
            bool operator != ( const float* const ) const;
            bool operator != ( float ) const;

            // +
            matrix4x4_f operator+(const matrix4x4_f&) const;
            matrix4x4_f operator+(const float* const) const;
            matrix4x4_f operator+(float)const;

            // -
            matrix4x4_f operator-(const matrix4x4_f&) const;
            matrix4x4_f operator-(const float* const) const;
            matrix4x4_f operator-(float) const;

            // *
            matrix4x4_f operator*(const matrix4x4_f&) const;
            matrix4x4_f operator*(const float* const) const;
            matrix4x4_f operator*(float) const;

            // /
            matrix4x4_f operator/(const matrix4x4_f&) const;
            matrix4x4_f operator/(const float* const) const;
            matrix4x4_f operator/(float) const;

            // http://en.wikipedia.org/wiki/Matrix_(mathematics)
            //
            void identityIt();
            void transposeIt();
            bool inverseIt();
            matrix4x4_f transposeMatrix()const;
            matrix4x4_f inverseMatrix()const;    
            bool isIdentity()const;
            matrix4x4_f matrix4x4_f::adjoint() const;
            float determinant() const;

        public:
            static const matrix4x4_f ZERO;
            static const matrix4x4_f IDENTITY;
            union{
                float v[16];
                float _v[4][4];
            };

        static float MINOR(const matrix4x4_f&, int,int,int,int,int,int);
            
        };

        // ==
        ZH_MATH_DLL bool operator == ( const float* const, const matrix4x4_f& );
        ZH_MATH_DLL bool operator == ( float, const matrix4x4_f& );
        
        // !=
        ZH_MATH_DLL bool operator != ( const float* const, const matrix4x4_f& );
        ZH_MATH_DLL bool operator != ( float, const matrix4x4_f& );

        // matrix4x4_f * float4
        ZH_MATH_DLL float4 operator * (const matrix4x4_f&, const float4&);
 
    }
} // namespace ZH


#endif