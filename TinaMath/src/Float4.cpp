#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif
#include "Math/Float4.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        
        bool operator == ( const float* const t, const float4& v )
        {
            return VALUE_EQUAL<float>(v.x,t[0]) &&
                   VALUE_EQUAL<float>(v.y,t[1]) &&
                   VALUE_EQUAL<float>(v.z,t[2]) &&
                   VALUE_EQUAL<float>(v.w,t[3]);
        }

        // !=
        
        bool operator != ( const float* const t, const float4& v )
        {
             return !VALUE_EQUAL<float>(v.x,t[0]) ||
                    !VALUE_EQUAL<float>(v.y,t[1]) ||
                    !VALUE_EQUAL<float>(v.z,t[2]) ||
                    !VALUE_EQUAL<float>(v.w,t[3]);
        }

        // +
        
        float4 operator + ( const float* const t, const float4& v)
        {
            return float4(t[0]+v.x, t[1]+v.y, t[2]+v.z, t[3]+v.w);
        }

        
        float4 operator + ( const float& t, const float4& v)
        {
            return float4(t+v.x, t+v.y, t+v.z, t+v.w);
        }

        // -
        
        float4 operator - ( const float* const t, const float4& v)
        {
            return float4(t[0]-v.x, t[1]-v.y, t[2]-v.z, t[3]-v.w);
        }

        
        float4 operator - ( const float& t, const float4& v)
        {
            return float4(t-v.x, t-v.y, t-v.z, t-v.w);
        }
        // *
        
        float4 operator * ( const float* const t, const float4& v)
        {
            return float4(t[0]*v.x, t[1]*v.y, t[2]*v.z, t[3]*v.w);
        }

        
        float4 operator * ( const float& t, const float4& v)
        {
            return float4(t*v.x, t*v.y, t*v.z, t*v.w);
        }
        // /
        
        float4 operator / ( const float* const t, const float4& v)
        {
            return float4(t[0]/v.x, t[1]/v.y, t[2]/v.z, t[3]/v.w);
        }

        
        float4 operator / ( const float& t, const float4& v)
        {
            return float4(t/v.x, t/v.y, t/v.z, t/v.w);
        }
        // +=
        
        void operator += ( float* const t, const float4& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
            t[3] += v.w;
        }

        // -=
        
        void operator -= ( float* const t, const float4& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
            t[3] -= v.w;
        }
        // *=
        
        void operator *= ( float* const t, const float4& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
            t[3] *= v.w;
        }
        // /=
        
        void operator /= ( float* const t, const float4& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
            t[3] /= v.w;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        
        float4::float4():x((float)0),y((float)0),z((float)0),w((float)0)
        {
        }

        
        float4::float4( float a, float b, float c, float d)
        {
            x=a;
            y=b;
            z=c;
            w=d;
        }

        
        float4::float4( const float4& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        
        float4::float4( const float* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
        }

        // []
        
        float float4::operator [] ( unsigned int idx ) const
        {
            assert( idx < 4 );
            return val[idx];
        }

        
        float& float4::operator [] ( unsigned int idx )
        {
            assert( idx < 4 );
            return val[idx];
        }

        // =
        
        float4& float4::operator = ( const float4& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }

        
        float4& float4::operator = ( const float* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
            return *this;
        }

        // ==
        
        bool float4::operator == ( const float4& v) const
        {
            return VALUE_EQUAL<float>(v.x,x) &&
                   VALUE_EQUAL<float>(v.y,y) &&
                   VALUE_EQUAL<float>(v.z,z) &&
                   VALUE_EQUAL<float>(v.w,w);
        }

        
        bool float4::operator == ( const float* const v) const
        {
            return VALUE_EQUAL<float>(x,v[0]) &&
                   VALUE_EQUAL<float>(y,v[1]) &&
                   VALUE_EQUAL<float>(z,v[2]) &&
                   VALUE_EQUAL<float>(w,v[3]);
        }

        // !=
        
        bool float4::operator != ( const float4& v) const
        {
            return !VALUE_EQUAL<float>(x,v.x) ||
                   !VALUE_EQUAL<float>(y,v.y) ||
                   !VALUE_EQUAL<float>(z,v.z) ||
                   !VALUE_EQUAL<float>(w,v.w);
        }

        
        bool float4::operator != ( const float* const v) const
        {
            return !VALUE_EQUAL<float>(x,v[0]) ||
                   !VALUE_EQUAL<float>(y,v[1]) ||
                   !VALUE_EQUAL<float>(z,v[2]) ||
                   !VALUE_EQUAL<float>(w,v[3]);
        }

        // +
        
        float4 float4::operator + ( const float4& v) const
        {
            return float4(x+v.x, y+v.y, z+v.z, w+v.w);
        }

        
        float4 float4::operator + ( const float* const v ) const
        {
            return float4(x+v[0], y+v[1], z+v[2], w+v[3]);
        }

        // -
        
        float4 float4::operator - ( const float4& v) const
        {
            return float4(x-v.x, y-v.y, z-v.z, w-v.w);
        }

        
        float4 float4::operator - ( const float* const v ) const
        {
            return float4(x-v[0], y-v[1], z-v[2], w-v[3]);
        }

        // *
        
        float4 float4::operator * ( const float4& v) const
        {
            return float4(x*v.x, y*v.y, z*v.z, w*v.w);
        }

        
        float4 float4::operator * ( const float* const v ) const
        {
            return float4(x*v[0], y*v[1], z*v[2], w*v[3]);
        }

        
        float4 float4::operator * ( const float& v ) const
        {
            return float4(x*v, y*v, z*v, w*v);
        }

        // /
        
        float4 float4::operator / ( const float4& v) const
        {
            return float4(x/v.x, y/v.y, z/v.z, w/v.w);
        }

        
        float4 float4::operator / ( const float* const v ) const
        {
            return float4(x/v[0], y/v[1], z/v[2], w/v[3]);
        }

        
        float4 float4::operator / ( const float& v ) const
        {
            return float4(x/v, y/v, z/v, w/v);
        }

        // +=
        
        void float4::operator += ( const float4& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
        }

        
        void float4::operator += ( const float* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
            w += v[3];
        }

        // -=
        
        void float4::operator -= ( const float4& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
        }

        
        void float4::operator -= ( const float* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
            w -= v[3];
        }

        // *=
        
        void float4::operator *= ( const float4& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
        }

        
        void float4::operator *= ( const float* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
            w *= v[3];
        }

        
        void float4::operator *= ( const float& v )
        {
            x *= v;
            y *= v;
            z *= v;
            w *= v;
        }

        // /=
        
        void float4::operator /= ( const float4& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
        }

        
        void float4::operator /= ( const float* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
            w /= v[3];
        }

        
        void float4::operator /= ( const float& v )
        {
            x /= v;
            y /= v;
            z /= v;
            w /= v;
        }

        // Non-operator functions
        
        float float4::length() const
        {
            return sqrt( (float)(x * x + y * y + z * z + w * w ) );
        }

        
        float float4::squaredLength() const
        {
            return x * x + y * y + z * z + w * w;
        }

        
        float float4::dot( const float4& v) const
        {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        
        void float4::normalizeIt()
        {
            const float length = sqrt( (float)(x * x + y * y + z*z + w*w) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (float)(x/length);
                y = (float)(y/length);
                z = (float)(z/length);
                w = (float)(w/length);
            }
        }

        
        float4 float4::normalize() const
        {
            const float length = sqrt( (float)(x * x + y * y + z*z + w*w) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return float4((float)(x/length), (float)(y/length), (float)(z/length), (float)(w/length));
            }
            return *this;
        }

    }
}
