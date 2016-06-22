#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif
#include "Math/Int4.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        
        bool operator == ( const int* const t, const int4& v )
        {
            return VALUE_EQUAL<int>(v.x,t[0]) &&
                   VALUE_EQUAL<int>(v.y,t[1]) &&
                   VALUE_EQUAL<int>(v.z,t[2]) &&
                   VALUE_EQUAL<int>(v.w,t[3]);
        }

        // !=
        
        bool operator != ( const int* const t, const int4& v )
        {
             return !VALUE_EQUAL<int>(v.x,t[0]) ||
                    !VALUE_EQUAL<int>(v.y,t[1]) ||
                    !VALUE_EQUAL<int>(v.z,t[2]) ||
                    !VALUE_EQUAL<int>(v.w,t[3]);
        }

        // +
        
        int4 operator + ( const int* const t, const int4& v)
        {
            return int4(t[0]+v.x, t[1]+v.y, t[2]+v.z, t[3]+v.w);
        }

        
        int4 operator + ( const int& t, const int4& v)
        {
            return int4(t+v.x, t+v.y, t+v.z, t+v.w);
        }

        // -
        
        int4 operator - ( const int* const t, const int4& v)
        {
            return int4(t[0]-v.x, t[1]-v.y, t[2]-v.z, t[3]-v.w);
        }

        
        int4 operator - ( const int& t, const int4& v)
        {
            return int4(t-v.x, t-v.y, t-v.z, t-v.w);
        }
        // *
        
        int4 operator * ( const int* const t, const int4& v)
        {
            return int4(t[0]*v.x, t[1]*v.y, t[2]*v.z, t[3]*v.w);
        }

        
        int4 operator * ( const int& t, const int4& v)
        {
            return int4(t*v.x, t*v.y, t*v.z, t*v.w);
        }
        // /
        
        int4 operator / ( const int* const t, const int4& v)
        {
            return int4(t[0]/v.x, t[1]/v.y, t[2]/v.z, t[3]/v.w);
        }

        
        int4 operator / ( const int& t, const int4& v)
        {
            return int4(t/v.x, t/v.y, t/v.z, t/v.w);
        }
        // +=
        
        void operator += ( int* const t, const int4& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
            t[3] += v.w;
        }

        // -=
        
        void operator -= ( int* const t, const int4& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
            t[3] -= v.w;
        }
        // *=
        
        void operator *= ( int* const t, const int4& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
            t[3] *= v.w;
        }
        // /=
        
        void operator /= ( int* const t, const int4& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
            t[3] /= v.w;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        
        int4::int4():x((int)0),y((int)0),z((int)0),w((int)0)
        {
        }

        
        int4::int4( int a, int b, int c, int d)
        {
            x=a;
            y=b;
            z=c;
            w=d;
        }

        
        int4::int4( const int4& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        
        int4::int4( const int* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
        }

        // []
        
        int int4::operator [] ( unsigned int idx ) const
        {
            assert( idx < 4 );
            return val[idx];
        }

        
        int& int4::operator [] ( unsigned int idx )
        {
            assert( idx < 4 );
            return val[idx];
        }

        // =
        
        int4& int4::operator = ( const int4& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }

        
        int4& int4::operator = ( const int* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
            return *this;
        }

        // ==
        
        bool int4::operator == ( const int4& v) const
        {
            return VALUE_EQUAL<int>(v.x,x) &&
                   VALUE_EQUAL<int>(v.y,y) &&
                   VALUE_EQUAL<int>(v.z,z) &&
                   VALUE_EQUAL<int>(v.w,w);
        }

        
        bool int4::operator == ( const int* const v) const
        {
            return VALUE_EQUAL<int>(x,v[0]) &&
                   VALUE_EQUAL<int>(y,v[1]) &&
                   VALUE_EQUAL<int>(z,v[2]) &&
                   VALUE_EQUAL<int>(w,v[3]);
        }

        // !=
        
        bool int4::operator != ( const int4& v) const
        {
            return !VALUE_EQUAL<int>(x,v.x) ||
                   !VALUE_EQUAL<int>(y,v.y) ||
                   !VALUE_EQUAL<int>(z,v.z) ||
                   !VALUE_EQUAL<int>(w,v.w);
        }

        
        bool int4::operator != ( const int* const v) const
        {
            return !VALUE_EQUAL<int>(x,v[0]) ||
                   !VALUE_EQUAL<int>(y,v[1]) ||
                   !VALUE_EQUAL<int>(z,v[2]) ||
                   !VALUE_EQUAL<int>(w,v[3]);
        }

        // +
        
        int4 int4::operator + ( const int4& v) const
        {
            return int4(x+v.x, y+v.y, z+v.z, w+v.w);
        }

        
        int4 int4::operator + ( const int* const v ) const
        {
            return int4(x+v[0], y+v[1], z+v[2], w+v[3]);
        }

        // -
        
        int4 int4::operator - ( const int4& v) const
        {
            return int4(x-v.x, y-v.y, z-v.z, w-v.w);
        }

        
        int4 int4::operator - ( const int* const v ) const
        {
            return int4(x-v[0], y-v[1], z-v[2], w-v[3]);
        }

        // *
        
        int4 int4::operator * ( const int4& v) const
        {
            return int4(x*v.x, y*v.y, z*v.z, w*v.w);
        }

        
        int4 int4::operator * ( const int* const v ) const
        {
            return int4(x*v[0], y*v[1], z*v[2], w*v[3]);
        }

        
        int4 int4::operator * ( const int& v ) const
        {
            return int4(x*v, y*v, z*v, w*v);
        }

        // /
        
        int4 int4::operator / ( const int4& v) const
        {
            return int4(x/v.x, y/v.y, z/v.z, w/v.w);
        }

        
        int4 int4::operator / ( const int* const v ) const
        {
            return int4(x/v[0], y/v[1], z/v[2], w/v[3]);
        }

        
        int4 int4::operator / ( const int& v ) const
        {
            return int4(x/v, y/v, z/v, w/v);
        }

        // +=
        
        void int4::operator += ( const int4& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
        }

        
        void int4::operator += ( const int* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
            w += v[3];
        }

        // -=
        
        void int4::operator -= ( const int4& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
        }

        
        void int4::operator -= ( const int* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
            w -= v[3];
        }

        // *=
        
        void int4::operator *= ( const int4& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
        }

        
        void int4::operator *= ( const int* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
            w *= v[3];
        }

        
        void int4::operator *= ( const int& v )
        {
            x *= v;
            y *= v;
            z *= v;
            w *= v;
        }

        // /=
        
        void int4::operator /= ( const int4& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
        }

        
        void int4::operator /= ( const int* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
            w /= v[3];
        }

        
        void int4::operator /= ( const int& v )
        {
            x /= v;
            y /= v;
            z /= v;
            w /= v;
        }

        // Non-operator functions
        
        float int4::length() const
        {
            return (float)sqrt(x * x + y * y + z * z + w * w);
        }

        
        int int4::squaredLength() const
        {
            return x * x + y * y + z * z + w * w;
        }

        
        int int4::dot( const int4& v) const
        {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        
        void int4::normalizeIt()
        {
            const float length = (float)sqrt(x * x + y * y + z*z + w*w);
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (int)(x/length);
                y = (int)(y/length);
                z = (int)(z/length);
                w = (int)(w/length);
            }
        }

        
        int4 int4::normalize() const
        {
            const float length = (float)sqrt(x * x + y * y + z*z + w*w);
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return int4((int)(x/length), (int)(y/length), (int)(z/length), (int)(w/length));
            }
            return *this;
        }

    }
}
