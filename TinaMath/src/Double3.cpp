#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Double3.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        bool operator == ( const double* const t, const double3& v )
        {
            return VALUE_EQUAL<double>(v.x,t[0]) &&
                   VALUE_EQUAL<double>(v.y,t[1]) &&
                   VALUE_EQUAL<double>(v.z,t[2]);
        }

        // !=
        bool operator != ( const double* const t, const double3& v )
        {
             return !VALUE_EQUAL<double>(v.x,t[0]) ||
                    !VALUE_EQUAL<double>(v.y,t[1]) ||
                    !VALUE_EQUAL<double>(v.z,t[2]);
        }

        // +
        double3 operator + ( const double* const t, const double3& v)
        {
            return double3(t[0]+v.x, t[1]+v.y, t[2]+v.z);
        }

        double3 operator + ( const double& t, const double3& v)
        {
            return double3(t+v.x, t+v.y, t+v.z);
        }

        // -
        double3 operator - ( const double* const t, const double3& v)
        {
            return double3(t[0]-v.x, t[1]-v.y, t[2]-v.z);
        }

        double3 operator - ( const double& t, const double3& v)
        {
            return double3(t-v.x, t-v.y, t-v.z);
        }

        // *
        double3 operator * ( const double* const t, const double3& v)
        {
            return double3(t[0]*v.x, t[1]*v.y, t[2]*v.z);
        }

        double3 operator * ( const double& t, const double3& v)
        {
            return double3(t*v.x, t*v.y, t*v.z);
        }

        // /
        double3 operator / ( const double* const t, const double3& v)
        {
            return double3(t[0]/v.x, t[1]/v.y, t[2]/v.z);
        }

        double3 operator / ( const double& t, const double3& v)
        {
            return double3(t/v.x, t/v.y, t/v.z);
        }

        // +=
        void operator += ( double* const t, const double3& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
        }

        // -=
        void operator -= ( double* const t, const double3& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
        }

        // *=
        void operator *= ( double* const t, const double3& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
        }

        // /=
        void operator /= ( double* const t, const double3& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        double3::double3():x((double)0),y((double)0),z((double)0)
        {
        }

        double3::double3( double a, double b, double c)
        {
            x=a;
            y=b;
            z=c;
        }


        double3::double3( const double3& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }


        double3::double3( const double* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
        }

        // []
        double double3::operator [] ( unsigned int idx ) const
        {
            assert( idx < 3 );
            return val[idx];
        }


        double& double3::operator [] ( unsigned int idx )
        {
            assert( idx < 3 );
            return val[idx];
        }

        // =
        double3& double3::operator = ( const double3& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }


        double3& double3::operator = ( const double* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        // ==
        bool double3::operator == ( const double3& v) const
        {
            return VALUE_EQUAL<double>(v.x,x) &&
                   VALUE_EQUAL<double>(v.y,y) &&
                   VALUE_EQUAL<double>(v.z,z);
        }


        bool double3::operator == ( const double* const v) const
        {
            return VALUE_EQUAL<double>(x,v[0]) &&
                   VALUE_EQUAL<double>(y,v[1]) &&
                   VALUE_EQUAL<double>(z,v[2]);
        }

        // !=
        bool double3::operator != ( const double3& v) const
        {
            return !VALUE_EQUAL<double>(x,v.x) ||
                   !VALUE_EQUAL<double>(y,v.y) ||
                   !VALUE_EQUAL<double>(z,v.z);
        }


        bool double3::operator != ( const double* const v) const
        {
            return !VALUE_EQUAL<double>(x,v[0]) ||
                   !VALUE_EQUAL<double>(y,v[1]) ||
                   !VALUE_EQUAL<double>(z,v[2]);
        }

        // +
        double3 double3::operator + ( const double3& v) const
        {
            return double3(x+v.x, y+v.y, z+v.z);
        }


        double3 double3::operator + ( const double* const v ) const
        {
            return double3(x+v[0], y+v[1], z+v[2]);
        }

        // -
        double3 double3::operator - ( const double3& v) const
        {
            return double3(x-v.x, y-v.y, z-v.z);
        }


        double3 double3::operator - ( const double* const v ) const
        {
            return double3(x-v[0], y-v[1], z-v[2]);
        }

        // *
        double3 double3::operator * ( const double3& v) const
        {
            return double3(x*v.x, y*v.y, z*v.z);
        }


        double3 double3::operator * ( const double* const v ) const
        {
            return double3(x*v[0], y*v[1], z*v[2]);
        }


        double3 double3::operator * ( const double& v ) const
        {
            return double3(x*v, y*v, z*v);
        }

        // /
        double3 double3::operator / ( const double3& v) const
        {
            return double3(x/v.x, y/v.y, z/v.z);
        }


        double3 double3::operator / ( const double* const v ) const
        {
            return double3(x/v[0], y/v[1], z/v[2]);
        }


        double3 double3::operator / ( const double& v ) const
        {
            return double3(x/v, y/v, z/v);
        }

        // +=
        void double3::operator += ( const double3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }


        void double3::operator += ( const double* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
        }

        // -=
        void double3::operator -= ( const double3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }


        void double3::operator -= ( const double* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
        }

        // *=
        void double3::operator *= ( const double3& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }


        void double3::operator *= ( const double* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
        }


        void double3::operator *= ( const double& v )
        {
            x *= v;
            y *= v;
            z *= v;
        }

        // /=
        void double3::operator /= ( const double3& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
        }


        void double3::operator /= ( const double* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
        }


        void double3::operator /= ( const double& v )
        {
            x /= v;
            y /= v;
            z /= v;
        }

        // Non-operator functions
        double double3::length() const
        {
            return sqrt( (x * x + y * y + z*z ) );
        }


        double double3::squaredLength() const
        {
            return x * x + y * y + z * z ;
        }

        double double3::dot( const double3& v) const
        {
            return x * v.x + y * v.y + z * v.z ;
        }

        double3 double3::cross( const double3& v) const
        {
            return double3(y*v.z-z*v.y, z*v.x-x*v.z,x*v.y-y*v.x);
        }


        void double3::normalizeIt()
        {
            const double length = sqrt( (double)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (double)(x/length);
                y = (double)(y/length);
                z = (double)(z/length);
            }
        }

        double3 double3::normalize() const
        {
            const double length = sqrt( (double)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return double3((double)(x/length), (double)(y/length), (double)(z/length));
            }
            return *this;
        }

    }
}
