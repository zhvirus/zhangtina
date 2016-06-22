#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif
#include "Math/Float2.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        bool operator == ( const float* const t, const float2& v )
        {
            return VALUE_EQUAL<float>(v.x,t[0]) && VALUE_EQUAL<float>(v.y,t[1]);
        }

        // !=
        bool operator != ( const float* const t, const float2& v )
        {
             return !VALUE_EQUAL<float>(v.x,t[0]) || !VALUE_EQUAL<float>(v.y,t[1]);
        }

        // +
        float2 operator + ( const float* const t, const float2& v)
        {
            return float2(t[0]+v.x, t[1]+v.y);
        }

        float2 operator + ( const float& t, const float2& v)
        {
            return float2(t+v.x, t+v.y);
        }

        // -
        float2 operator - ( const float* const t, const float2& v)
        {
            return float2(t[0]-v.x, t[1]-v.y);
        }

        float2 operator - ( const float& t, const float2& v)
        {
            return float2(t-v.x, t-v.y);
        }
        // *
        float2 operator * ( const float* const t, const float2& v)
        {
            return float2(t[0]*v.x, t[1]*v.y);
        }

        float2 operator * ( const float& t, const float2& v)
        {
            return float2(t*v.x, t*v.y);
        }
        // /
        float2 operator / ( const float* const t, const float2& v)
        {
            return float2(t[0]/v.x, t[1]/v.y);
        }

        float2 operator / ( const float& t, const float2& v)
        {
            return float2(t/v.x, t/v.y);
        }
        // +=
        void operator += ( float* const t, const float2& v)
        {
            t[0] += v.x;
            t[1] += v.y;
        }

        // -=
        void operator -= ( float* const t, const float2& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
        }
        // *=
        void operator *= ( float* const t, const float2& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
        }
        // /=
        void operator /= ( float* const t, const float2& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
        }

        // ---------------------------------
        // class function
        // ---------------------------------

        float2::float2():x((float)0),y((float)0)
        {
        }


        float2::float2( float a, float b)
        {
            x=a;
            y=b;
        }


        float2::float2( const float2& v )
        {
            x = v.x;
            y = v.y;
        }


        float2::float2( const float* const v )
        {
            x = v[0];
            y = v[1];
        }

        // []

        float float2::operator [] ( unsigned int idx ) const
        {
            assert( idx < 2 );
            return val[idx];
        }


        float& float2::operator [] ( unsigned int idx )
        {
            assert( idx < 2 );
            return val[idx];
        }

        // =

        float2& float2::operator = ( const float2& v )
        {
            x = v.x;
            y = v.y;
            return *this;
        }


        float2& float2::operator = ( const float* const v )
        {
            x = v[0];
            y = v[1];
            return *this;
        }

        // ==

        bool float2::operator == ( const float2& v) const
        {
            return VALUE_EQUAL<float>(v.x,x) && VALUE_EQUAL<float>(v.y,y);
        }


        bool float2::operator == ( const float* const v) const
        {
            return VALUE_EQUAL<float>(x,v[0]) && VALUE_EQUAL<float>(y,v[1]);
        }

        // !=

        bool float2::operator != ( const float2& v) const
        {
            return !VALUE_EQUAL<float>(x,v.x) || !VALUE_EQUAL<float>(y,v.y);
        }


        bool float2::operator != ( const float* const v) const
        {
            return !VALUE_EQUAL<float>(x,v[0]) || !VALUE_EQUAL<float>(y,v[1]);
        }

        // +

        float2 float2::operator + ( const float2& v) const
        {
            return float2(x+v.x, y+v.y);
        }


        float2 float2::operator + ( const float* const v ) const
        {
            return float2(x+v[0], y+v[1]);
        }

        // -

        float2 float2::operator - ( const float2& v) const
        {
            return float2(x-v.x, y-v.y);
        }


        float2 float2::operator - ( const float* const v ) const
        {
            return float2(x-v[0], y-v[1]);
        }

        // *

        float2 float2::operator * ( const float2& v) const
        {
            return float2(x*v.x, y*v.y);
        }


        float2 float2::operator * ( const float* const v ) const
        {
            return float2(x*v[0], y*v[1]);
        }


        float2 float2::operator * ( const float& v ) const
        {
            return float2(x*v, y*v);
        }

        // /

        float2 float2::operator / ( const float2& v) const
        {
            return float2(x/v.x, y/v.y);
        }


        float2 float2::operator / ( const float* const v ) const
        {
            return float2(x/v[0], y/v[1]);
        }


        float2 float2::operator / ( const float& v ) const
        {
            return float2(x/v, y/v);
        }

        // +=

        void float2::operator += ( const float2& v)
        {
            x += v.x;
            y += v.y;
        }


        void float2::operator += ( const float* const v )
        {
            x += v[0];
            y += v[1];
        }

        // -=

        void float2::operator -= ( const float2& v)
        {
            x -= v.x;
            y -= v.y;
        }


        void float2::operator -= ( const float* const v )
        {
            x -= v[0];
            y -= v[1];
        }

        // *=

        void float2::operator *= ( const float2& v)
        {
            x *= v.x;
            y *= v.y;
        }


        void float2::operator *= ( const float* const v )
        {
            x *= v[0];
            y *= v[1];
        }


        void float2::operator *= ( const float& v )
        {
            x *= v;
            y *= v;
        }

        // /=

        void float2::operator /= ( const float2& v)
        {
            x /= v.x;
            y /= v.y;
        }


        void float2::operator /= ( const float* const v )
        {
            x /= v[0];
            y /= v[1];
        }


        void float2::operator /= ( const float& v )
        {
            x /= v;
            y /= v;
        }

        // Non-operator functions

        float float2::length() const
        {
            return (float)sqrt(x * x + y * y);
        }


        float float2::squaredLength() const
        {
            return x * x + y * y;
        }


        float float2::dot( const float2& v) const
        {
            return x * v.x + y * v.y;
        }
    }
}
