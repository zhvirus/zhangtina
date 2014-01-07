#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Int3.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        bool operator == ( const int* const t, const int3& v )
        {
            return VALUE_EQUAL<int>(v.x,t[0]) &&
                   VALUE_EQUAL<int>(v.y,t[1]) &&
                   VALUE_EQUAL<int>(v.z,t[2]);
        }

        // !=
        bool operator != ( const int* const t, const int3& v )
        {
             return !VALUE_EQUAL<int>(v.x,t[0]) ||
                    !VALUE_EQUAL<int>(v.y,t[1]) ||
                    !VALUE_EQUAL<int>(v.z,t[2]);
        }

        // +
        int3 operator + ( const int* const t, const int3& v)
        {
            return int3(t[0]+v.x, t[1]+v.y, t[2]+v.z);
        }

        int3 operator + ( const int& t, const int3& v)
        {
            return int3(t+v.x, t+v.y, t+v.z);
        }

        // -
        int3 operator - ( const int* const t, const int3& v)
        {
            return int3(t[0]-v.x, t[1]-v.y, t[2]-v.z);
        }

        int3 operator - ( const int& t, const int3& v)
        {
            return int3(t-v.x, t-v.y, t-v.z);
        }

        // *
        int3 operator * ( const int* const t, const int3& v)
        {
            return int3(t[0]*v.x, t[1]*v.y, t[2]*v.z);
        }

        int3 operator * ( const int& t, const int3& v)
        {
            return int3(t*v.x, t*v.y, t*v.z);
        }

        // /
        int3 operator / ( const int* const t, const int3& v)
        {
            return int3(t[0]/v.x, t[1]/v.y, t[2]/v.z);
        }

        int3 operator / ( const int& t, const int3& v)
        {
            return int3(t/v.x, t/v.y, t/v.z);
        }

        // +=
        void operator += ( int* const t, const int3& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
        }

        // -=
        void operator -= ( int* const t, const int3& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
        }

        // *=
        void operator *= ( int* const t, const int3& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
        }

        // /=
        void operator /= ( int* const t, const int3& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        int3::int3():x((int)0),y((int)0),z((int)0)
        {
        }

        int3::int3( int a, int b, int c)
        {
            x=a;
            y=b;
            z=c;
        }


        int3::int3( const int3& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }


        int3::int3( const int* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
        }

        // []
        int int3::operator [] ( unsigned int idx ) const
        {
            assert( idx < 3 );
            return val[idx];
        }


        int& int3::operator [] ( unsigned int idx )
        {
            assert( idx < 3 );
            return val[idx];
        }

        // =
        int3& int3::operator = ( const int3& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }


        int3& int3::operator = ( const int* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        // ==
        bool int3::operator == ( const int3& v) const
        {
            return VALUE_EQUAL<int>(v.x,x) &&
                   VALUE_EQUAL<int>(v.y,y) &&
                   VALUE_EQUAL<int>(v.z,z);
        }


        bool int3::operator == ( const int* const v) const
        {
            return VALUE_EQUAL<int>(x,v[0]) &&
                   VALUE_EQUAL<int>(y,v[1]) &&
                   VALUE_EQUAL<int>(z,v[2]);
        }

        // !=
        bool int3::operator != ( const int3& v) const
        {
            return !VALUE_EQUAL<int>(x,v.x) ||
                   !VALUE_EQUAL<int>(y,v.y) ||
                   !VALUE_EQUAL<int>(z,v.z);
        }


        bool int3::operator != ( const int* const v) const
        {
            return !VALUE_EQUAL<int>(x,v[0]) ||
                   !VALUE_EQUAL<int>(y,v[1]) ||
                   !VALUE_EQUAL<int>(z,v[2]);
        }

        // +
        int3 int3::operator + ( const int3& v) const
        {
            return int3(x+v.x, y+v.y, z+v.z);
        }


        int3 int3::operator + ( const int* const v ) const
        {
            return int3(x+v[0], y+v[1], z+v[2]);
        }

        // -
        int3 int3::operator - ( const int3& v) const
        {
            return int3(x-v.x, y-v.y, z-v.z);
        }


        int3 int3::operator - ( const int* const v ) const
        {
            return int3(x-v[0], y-v[1], z-v[2]);
        }

        // *
        int3 int3::operator * ( const int3& v) const
        {
            return int3(x*v.x, y*v.y, z*v.z);
        }


        int3 int3::operator * ( const int* const v ) const
        {
            return int3(x*v[0], y*v[1], z*v[2]);
        }


        int3 int3::operator * ( const int& v ) const
        {
            return int3(x*v, y*v, z*v);
        }

        // /
        int3 int3::operator / ( const int3& v) const
        {
            return int3(x/v.x, y/v.y, z/v.z);
        }


        int3 int3::operator / ( const int* const v ) const
        {
            return int3(x/v[0], y/v[1], z/v[2]);
        }


        int3 int3::operator / ( const int& v ) const
        {
            return int3(x/v, y/v, z/v);
        }

        // +=
        void int3::operator += ( const int3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }


        void int3::operator += ( const int* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
        }

        // -=
        void int3::operator -= ( const int3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }


        void int3::operator -= ( const int* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
        }

        // *=
        void int3::operator *= ( const int3& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }


        void int3::operator *= ( const int* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
        }


        void int3::operator *= ( const int& v )
        {
            x *= v;
            y *= v;
            z *= v;
        }

        // /=
        void int3::operator /= ( const int3& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
        }


        void int3::operator /= ( const int* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
        }


        void int3::operator /= ( const int& v )
        {
            x /= v;
            y /= v;
            z /= v;
        }

        // Non-operator functions
        float int3::length() const
        {
            return sqrt( (float)(x * x + y * y + z*z ) );
        }


        int int3::squaredLength() const
        {
            return x * x + y * y + z * z ;
        }

        int int3::dot( const int3& v) const
        {
            return x * v.x + y * v.y + z * v.z ;
        }

        int3 int3::cross( const int3& v) const
        {
            return int3(y*v.z-z*v.y, z*v.x-x*v.z,x*v.y-y*v.x);
        }


        void int3::normalizeIt()
        {
            const float length = sqrt( (float)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (int)(x/length);
                y = (int)(y/length);
                z = (int)(z/length);
            }
        }

        int3 int3::normalize() const
        {
            const float length = sqrt( (float)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return int3((int)(x/length), (int)(y/length), (int)(z/length));
            }
            return *this;
        }

    }
}
