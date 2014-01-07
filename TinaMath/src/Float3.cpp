#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Float3.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        bool operator == ( const float* const t, const float3& v )
        {
            return VALUE_EQUAL<float>(v.x,t[0]) &&
                   VALUE_EQUAL<float>(v.y,t[1]) &&
                   VALUE_EQUAL<float>(v.z,t[2]);
        }

        // !=
        bool operator != ( const float* const t, const float3& v )
        {
             return !VALUE_EQUAL<float>(v.x,t[0]) ||
                    !VALUE_EQUAL<float>(v.y,t[1]) ||
                    !VALUE_EQUAL<float>(v.z,t[2]);
        }

        // +
        float3 operator + ( const float* const t, const float3& v)
        {
            return float3(t[0]+v.x, t[1]+v.y, t[2]+v.z);
        }

        float3 operator + ( const float& t, const float3& v)
        {
            return float3(t+v.x, t+v.y, t+v.z);
        }

        // -
        float3 operator - ( const float* const t, const float3& v)
        {
            return float3(t[0]-v.x, t[1]-v.y, t[2]-v.z);
        }

        float3 operator - ( const float& t, const float3& v)
        {
            return float3(t-v.x, t-v.y, t-v.z);
        }

        // *
        float3 operator * ( const float* const t, const float3& v)
        {
            return float3(t[0]*v.x, t[1]*v.y, t[2]*v.z);
        }

        float3 operator * ( const float& t, const float3& v)
        {
            return float3(t*v.x, t*v.y, t*v.z);
        }

        // /
        float3 operator / ( const float* const t, const float3& v)
        {
            return float3(t[0]/v.x, t[1]/v.y, t[2]/v.z);
        }

        float3 operator / ( const float& t, const float3& v)
        {
            return float3(t/v.x, t/v.y, t/v.z);
        }

        // +=
        void operator += ( float* const t, const float3& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
        }

        // -=
        void operator -= ( float* const t, const float3& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
        }

        // *=
        void operator *= ( float* const t, const float3& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
        }

        // /=
        void operator /= ( float* const t, const float3& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        float3::float3():x((float)0),y((float)0),z((float)0)
        {
        }

        float3::float3( float a, float b, float c)
        {
            x=a;
            y=b;
            z=c;
        }


        float3::float3( const float3& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }


        float3::float3( const float* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
        }

        // []
        float float3::operator [] ( unsigned int idx ) const
        {
            assert( idx < 3 );
            return val[idx];
        }


        float& float3::operator [] ( unsigned int idx )
        {
            assert( idx < 3 );
            return val[idx];
        }

        // =
        float3& float3::operator = ( const float3& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }


        float3& float3::operator = ( const float* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        // ==
        bool float3::operator == ( const float3& v) const
        {
            return VALUE_EQUAL<float>(v.x,x) &&
                   VALUE_EQUAL<float>(v.y,y) &&
                   VALUE_EQUAL<float>(v.z,z);
        }


        bool float3::operator == ( const float* const v) const
        {
            return VALUE_EQUAL<float>(x,v[0]) &&
                   VALUE_EQUAL<float>(y,v[1]) &&
                   VALUE_EQUAL<float>(z,v[2]);
        }

        // !=
        bool float3::operator != ( const float3& v) const
        {
            return !VALUE_EQUAL<float>(x,v.x) ||
                   !VALUE_EQUAL<float>(y,v.y) ||
                   !VALUE_EQUAL<float>(z,v.z);
        }


        bool float3::operator != ( const float* const v) const
        {
            return !VALUE_EQUAL<float>(x,v[0]) ||
                   !VALUE_EQUAL<float>(y,v[1]) ||
                   !VALUE_EQUAL<float>(z,v[2]);
        }

        // +
        float3 float3::operator + ( const float3& v) const
        {
            return float3(x+v.x, y+v.y, z+v.z);
        }


        float3 float3::operator + ( const float* const v ) const
        {
            return float3(x+v[0], y+v[1], z+v[2]);
        }

        // -
        float3 float3::operator - ( const float3& v) const
        {
            return float3(x-v.x, y-v.y, z-v.z);
        }


        float3 float3::operator - ( const float* const v ) const
        {
            return float3(x-v[0], y-v[1], z-v[2]);
        }

        // *
        float3 float3::operator * ( const float3& v) const
        {
            return float3(x*v.x, y*v.y, z*v.z);
        }


        float3 float3::operator * ( const float* const v ) const
        {
            return float3(x*v[0], y*v[1], z*v[2]);
        }


        float3 float3::operator * ( const float& v ) const
        {
            return float3(x*v, y*v, z*v);
        }

        // /
        float3 float3::operator / ( const float3& v) const
        {
            return float3(x/v.x, y/v.y, z/v.z);
        }


        float3 float3::operator / ( const float* const v ) const
        {
            return float3(x/v[0], y/v[1], z/v[2]);
        }


        float3 float3::operator / ( const float& v ) const
        {
            return float3(x/v, y/v, z/v);
        }

        // +=
        void float3::operator += ( const float3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }


        void float3::operator += ( const float* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
        }

        // -=
        void float3::operator -= ( const float3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }


        void float3::operator -= ( const float* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
        }

        // *=
        void float3::operator *= ( const float3& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }


        void float3::operator *= ( const float* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
        }


        void float3::operator *= ( const float& v )
        {
            x *= v;
            y *= v;
            z *= v;
        }

        // /=
        void float3::operator /= ( const float3& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
        }


        void float3::operator /= ( const float* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
        }


        void float3::operator /= ( const float& v )
        {
            x /= v;
            y /= v;
            z /= v;
        }

        // Non-operator functions
        float float3::length() const
        {
            return sqrt( (x * x + y * y + z*z ) );
        }


        float float3::squaredLength() const
        {
            return x * x + y * y + z * z ;
        }

        float float3::dot( const float3& v) const
        {
            return x * v.x + y * v.y + z * v.z ;
        }

        float3 float3::cross( const float3& v) const
        {
            return float3(y*v.z-z*v.y, z*v.x-x*v.z,x*v.y-y*v.x);
        }


        void float3::normalizeIt()
        {
            const float length = sqrt( (float)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (float)(x/length);
                y = (float)(y/length);
                z = (float)(z/length);
            }
        }

        float3 float3::normalize() const
        {
            const float length = sqrt( (float)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return float3((float)(x/length), (float)(y/length), (float)(z/length));
            }
            return *this;
        }

    }
}
