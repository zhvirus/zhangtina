#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif
#include "Math/Int2.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        bool operator == ( const int* const t, const int2& v )
        {
            return VALUE_EQUAL<int>(v.x,t[0]) && VALUE_EQUAL<int>(v.y,t[1]);
        }

        // !=
        bool operator != ( const int* const t, const int2& v )
        {
             return !VALUE_EQUAL<int>(v.x,t[0]) || !VALUE_EQUAL<int>(v.y,t[1]);
        }

        // +
        int2 operator + ( const int* const t, const int2& v)
        {
            return int2(t[0]+v.x, t[1]+v.y);
        }

        int2 operator + ( const int& t, const int2& v)
        {
            return int2(t+v.x, t+v.y);
        }

        // -
        int2 operator - ( const int* const t, const int2& v)
        {
            return int2(t[0]-v.x, t[1]-v.y);
        }

        int2 operator - ( const int& t, const int2& v)
        {
            return int2(t-v.x, t-v.y);
        }
        // *
        int2 operator * ( const int* const t, const int2& v)
        {
            return int2(t[0]*v.x, t[1]*v.y);
        }

        int2 operator * ( const int& t, const int2& v)
        {
            return int2(t*v.x, t*v.y);
        }
        // /
        int2 operator / ( const int* const t, const int2& v)
        {
            return int2(t[0]/v.x, t[1]/v.y);
        }

        int2 operator / ( const int& t, const int2& v)
        {
            return int2(t/v.x, t/v.y);
        }
        // +=
        void operator += ( int* const t, const int2& v)
        {
            t[0] += v.x;
            t[1] += v.y;
        }

        // -=
        void operator -= ( int* const t, const int2& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
        }
        // *=
        void operator *= ( int* const t, const int2& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
        }
        // /=
        void operator /= ( int* const t, const int2& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
        }

        // ---------------------------------
        // class function
        // ---------------------------------

        int2::int2():x((int)0),y((int)0)
        {
        }


        int2::int2( int a, int b)
        {
            x=a;
            y=b;
        }


        int2::int2( const int2& v )
        {
            x = v.x;
            y = v.y;
        }


        int2::int2( const int* const v )
        {
            x = v[0];
            y = v[1];
        }

        // []

        int int2::operator [] ( unsigned int idx ) const
        {
            assert( idx < 2 );
            return val[idx];
        }


        int& int2::operator [] ( unsigned int idx )
        {
            assert( idx < 2 );
            return val[idx];
        }

        // =

        int2& int2::operator = ( const int2& v )
        {
            x = v.x;
            y = v.y;
            return *this;
        }


        int2& int2::operator = ( const int* const v )
        {
            x = v[0];
            y = v[1];
            return *this;
        }

        // ==

        bool int2::operator == ( const int2& v) const
        {
            return VALUE_EQUAL<int>(v.x,x) && VALUE_EQUAL<int>(v.y,y);
        }


        bool int2::operator == ( const int* const v) const
        {
            return VALUE_EQUAL<int>(x,v[0]) && VALUE_EQUAL<int>(y,v[1]);
        }

        // !=

        bool int2::operator != ( const int2& v) const
        {
            return !VALUE_EQUAL<int>(x,v.x) || !VALUE_EQUAL<int>(y,v.y);
        }


        bool int2::operator != ( const int* const v) const
        {
            return !VALUE_EQUAL<int>(x,v[0]) || !VALUE_EQUAL<int>(y,v[1]);
        }

        // +

        int2 int2::operator + ( const int2& v) const
        {
            return int2(x+v.x, y+v.y);
        }


        int2 int2::operator + ( const int* const v ) const
        {
            return int2(x+v[0], y+v[1]);
        }

        // -

        int2 int2::operator - ( const int2& v) const
        {
            return int2(x-v.x, y-v.y);
        }


        int2 int2::operator - ( const int* const v ) const
        {
            return int2(x-v[0], y-v[1]);
        }

        // *

        int2 int2::operator * ( const int2& v) const
        {
            return int2(x*v.x, y*v.y);
        }


        int2 int2::operator * ( const int* const v ) const
        {
            return int2(x*v[0], y*v[1]);
        }


        int2 int2::operator * ( const int& v ) const
        {
            return int2(x*v, y*v);
        }

        // /

        int2 int2::operator / ( const int2& v) const
        {
            return int2(x/v.x, y/v.y);
        }


        int2 int2::operator / ( const int* const v ) const
        {
            return int2(x/v[0], y/v[1]);
        }


        int2 int2::operator / ( const int& v ) const
        {
            return int2(x/v, y/v);
        }

        // +=

        void int2::operator += ( const int2& v)
        {
            x += v.x;
            y += v.y;
        }


        void int2::operator += ( const int* const v )
        {
            x += v[0];
            y += v[1];
        }

        // -=

        void int2::operator -= ( const int2& v)
        {
            x -= v.x;
            y -= v.y;
        }


        void int2::operator -= ( const int* const v )
        {
            x -= v[0];
            y -= v[1];
        }

        // *=

        void int2::operator *= ( const int2& v)
        {
            x *= v.x;
            y *= v.y;
        }


        void int2::operator *= ( const int* const v )
        {
            x *= v[0];
            y *= v[1];
        }


        void int2::operator *= ( const int& v )
        {
            x *= v;
            y *= v;
        }

        // /=

        void int2::operator /= ( const int2& v)
        {
            x /= v.x;
            y /= v.y;
        }


        void int2::operator /= ( const int* const v )
        {
            x /= v[0];
            y /= v[1];
        }


        void int2::operator /= ( const int& v )
        {
            x /= v;
            y /= v;
        }

        // Non-operator functions

        float int2::length() const
        {
            return sqrt( (float)(x * x + y * y) );
        }


        int int2::squaredLength() const
        {
            return x * x + y * y;
        }


        int int2::dot( const int2& v) const
        {
            return x * v.x + y * v.y;
        }
    }
}
