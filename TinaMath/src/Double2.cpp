#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Double2.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        bool operator == ( const double* const t, const double2& v )
        {
            return VALUE_EQUAL<double>(v.x,t[0]) && VALUE_EQUAL<double>(v.y,t[1]);
        }

        // !=
        bool operator != ( const double* const t, const double2& v )
        {
             return !VALUE_EQUAL<double>(v.x,t[0]) || !VALUE_EQUAL<double>(v.y,t[1]);
        }

        // +
        double2 operator + ( const double* const t, const double2& v)
        {
            return double2(t[0]+v.x, t[1]+v.y);
        }

        double2 operator + ( const double& t, const double2& v)
        {
            return double2(t+v.x, t+v.y);
        }

        // -
        double2 operator - ( const double* const t, const double2& v)
        {
            return double2(t[0]-v.x, t[1]-v.y);
        }

        double2 operator - ( const double& t, const double2& v)
        {
            return double2(t-v.x, t-v.y);
        }
        // *
        double2 operator * ( const double* const t, const double2& v)
        {
            return double2(t[0]*v.x, t[1]*v.y);
        }

        double2 operator * ( const double& t, const double2& v)
        {
            return double2(t*v.x, t*v.y);
        }
        // /
        double2 operator / ( const double* const t, const double2& v)
        {
            return double2(t[0]/v.x, t[1]/v.y);
        }

        double2 operator / ( const double& t, const double2& v)
        {
            return double2(t/v.x, t/v.y);
        }
        // +=
        void operator += ( double* const t, const double2& v)
        {
            t[0] += v.x;
            t[1] += v.y;
        }

        // -=
        void operator -= ( double* const t, const double2& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
        }
        // *=
        void operator *= ( double* const t, const double2& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
        }
        // /=
        void operator /= ( double* const t, const double2& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
        }

        // ---------------------------------
        // class function
        // ---------------------------------

        double2::double2():x((double)0),y((double)0)
        {
        }


        double2::double2( double a, double b)
        {
            x=a;
            y=b;
        }


        double2::double2( const double2& v )
        {
            x = v.x;
            y = v.y;
        }


        double2::double2( const double* const v )
        {
            x = v[0];
            y = v[1];
        }

        // []

        double double2::operator [] ( unsigned int idx ) const
        {
            assert( idx < 2 );
            return val[idx];
        }


        double& double2::operator [] ( unsigned int idx )
        {
            assert( idx < 2 );
            return val[idx];
        }

        // =

        double2& double2::operator = ( const double2& v )
        {
            x = v.x;
            y = v.y;
            return *this;
        }


        double2& double2::operator = ( const double* const v )
        {
            x = v[0];
            y = v[1];
            return *this;
        }

        // ==

        bool double2::operator == ( const double2& v) const
        {
            return VALUE_EQUAL<double>(v.x,x) && VALUE_EQUAL<double>(v.y,y);
        }


        bool double2::operator == ( const double* const v) const
        {
            return VALUE_EQUAL<double>(x,v[0]) && VALUE_EQUAL<double>(y,v[1]);
        }

        // !=

        bool double2::operator != ( const double2& v) const
        {
            return !VALUE_EQUAL<double>(x,v.x) || !VALUE_EQUAL<double>(y,v.y);
        }


        bool double2::operator != ( const double* const v) const
        {
            return !VALUE_EQUAL<double>(x,v[0]) || !VALUE_EQUAL<double>(y,v[1]);
        }

        // +

        double2 double2::operator + ( const double2& v) const
        {
            return double2(x+v.x, y+v.y);
        }


        double2 double2::operator + ( const double* const v ) const
        {
            return double2(x+v[0], y+v[1]);
        }

        // -

        double2 double2::operator - ( const double2& v) const
        {
            return double2(x-v.x, y-v.y);
        }


        double2 double2::operator - ( const double* const v ) const
        {
            return double2(x-v[0], y-v[1]);
        }

        // *

        double2 double2::operator * ( const double2& v) const
        {
            return double2(x*v.x, y*v.y);
        }


        double2 double2::operator * ( const double* const v ) const
        {
            return double2(x*v[0], y*v[1]);
        }


        double2 double2::operator * ( const double& v ) const
        {
            return double2(x*v, y*v);
        }

        // /

        double2 double2::operator / ( const double2& v) const
        {
            return double2(x/v.x, y/v.y);
        }


        double2 double2::operator / ( const double* const v ) const
        {
            return double2(x/v[0], y/v[1]);
        }


        double2 double2::operator / ( const double& v ) const
        {
            return double2(x/v, y/v);
        }

        // +=

        void double2::operator += ( const double2& v)
        {
            x += v.x;
            y += v.y;
        }


        void double2::operator += ( const double* const v )
        {
            x += v[0];
            y += v[1];
        }

        // -=

        void double2::operator -= ( const double2& v)
        {
            x -= v.x;
            y -= v.y;
        }


        void double2::operator -= ( const double* const v )
        {
            x -= v[0];
            y -= v[1];
        }

        // *=

        void double2::operator *= ( const double2& v)
        {
            x *= v.x;
            y *= v.y;
        }


        void double2::operator *= ( const double* const v )
        {
            x *= v[0];
            y *= v[1];
        }


        void double2::operator *= ( const double& v )
        {
            x *= v;
            y *= v;
        }

        // /=

        void double2::operator /= ( const double2& v)
        {
            x /= v.x;
            y /= v.y;
        }


        void double2::operator /= ( const double* const v )
        {
            x /= v[0];
            y /= v[1];
        }


        void double2::operator /= ( const double& v )
        {
            x /= v;
            y /= v;
        }

        // Non-operator functions

        double double2::length() const
        {
            return sqrt( (double)(x * x + y * y) );
        }


        double double2::squaredLength() const
        {
            return x * x + y * y;
        }


        double double2::dot( const double2& v) const
        {
            return x * v.x + y * v.y;
        }
    }
}
