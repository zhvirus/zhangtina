#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif
#include "Math/Double4.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        
        bool operator == ( const double* const t, const double4& v )
        {
            return VALUE_EQUAL<double>(v.x,t[0]) &&
                   VALUE_EQUAL<double>(v.y,t[1]) &&
                   VALUE_EQUAL<double>(v.z,t[2]) &&
                   VALUE_EQUAL<double>(v.w,t[3]);
        }

        // !=
        
        bool operator != ( const double* const t, const double4& v )
        {
             return !VALUE_EQUAL<double>(v.x,t[0]) ||
                    !VALUE_EQUAL<double>(v.y,t[1]) ||
                    !VALUE_EQUAL<double>(v.z,t[2]) ||
                    !VALUE_EQUAL<double>(v.w,t[3]);
        }

        // +
        
        double4 operator + ( const double* const t, const double4& v)
        {
            return double4(t[0]+v.x, t[1]+v.y, t[2]+v.z, t[3]+v.w);
        }

        
        double4 operator + ( const double& t, const double4& v)
        {
            return double4(t+v.x, t+v.y, t+v.z, t+v.w);
        }

        // -
        
        double4 operator - ( const double* const t, const double4& v)
        {
            return double4(t[0]-v.x, t[1]-v.y, t[2]-v.z, t[3]-v.w);
        }

        
        double4 operator - ( const double& t, const double4& v)
        {
            return double4(t-v.x, t-v.y, t-v.z, t-v.w);
        }
        // *
        
        double4 operator * ( const double* const t, const double4& v)
        {
            return double4(t[0]*v.x, t[1]*v.y, t[2]*v.z, t[3]*v.w);
        }

        
        double4 operator * ( const double& t, const double4& v)
        {
            return double4(t*v.x, t*v.y, t*v.z, t*v.w);
        }
        // /
        
        double4 operator / ( const double* const t, const double4& v)
        {
            return double4(t[0]/v.x, t[1]/v.y, t[2]/v.z, t[3]/v.w);
        }

        
        double4 operator / ( const double& t, const double4& v)
        {
            return double4(t/v.x, t/v.y, t/v.z, t/v.w);
        }
        // +=
        
        void operator += ( double* const t, const double4& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
            t[3] += v.w;
        }

        // -=
        
        void operator -= ( double* const t, const double4& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
            t[3] -= v.w;
        }
        // *=
        
        void operator *= ( double* const t, const double4& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
            t[3] *= v.w;
        }
        // /=
        
        void operator /= ( double* const t, const double4& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
            t[3] /= v.w;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        
        double4::double4():x((double)0),y((double)0),z((double)0),w((double)0)
        {
        }

        
        double4::double4( double a, double b, double c, double d)
        {
            x=a;
            y=b;
            z=c;
            w=d;
        }

        
        double4::double4( const double4& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        
        double4::double4( const double* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
        }

        // []
        
        double double4::operator [] ( unsigned int idx ) const
        {
            assert( idx < 4 );
            return val[idx];
        }

        
        double& double4::operator [] ( unsigned int idx )
        {
            assert( idx < 4 );
            return val[idx];
        }

        // =
        
        double4& double4::operator = ( const double4& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }

        
        double4& double4::operator = ( const double* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
            return *this;
        }

        // ==
        
        bool double4::operator == ( const double4& v) const
        {
            return VALUE_EQUAL<double>(v.x,x) &&
                   VALUE_EQUAL<double>(v.y,y) &&
                   VALUE_EQUAL<double>(v.z,z) &&
                   VALUE_EQUAL<double>(v.w,w);
        }

        
        bool double4::operator == ( const double* const v) const
        {
            return VALUE_EQUAL<double>(x,v[0]) &&
                   VALUE_EQUAL<double>(y,v[1]) &&
                   VALUE_EQUAL<double>(z,v[2]) &&
                   VALUE_EQUAL<double>(w,v[3]);
        }

        // !=
        
        bool double4::operator != ( const double4& v) const
        {
            return !VALUE_EQUAL<double>(x,v.x) ||
                   !VALUE_EQUAL<double>(y,v.y) ||
                   !VALUE_EQUAL<double>(z,v.z) ||
                   !VALUE_EQUAL<double>(w,v.w);
        }

        
        bool double4::operator != ( const double* const v) const
        {
            return !VALUE_EQUAL<double>(x,v[0]) ||
                   !VALUE_EQUAL<double>(y,v[1]) ||
                   !VALUE_EQUAL<double>(z,v[2]) ||
                   !VALUE_EQUAL<double>(w,v[3]);
        }

        // +
        
        double4 double4::operator + ( const double4& v) const
        {
            return double4(x+v.x, y+v.y, z+v.z, w+v.w);
        }

        
        double4 double4::operator + ( const double* const v ) const
        {
            return double4(x+v[0], y+v[1], z+v[2], w+v[3]);
        }

        // -
        
        double4 double4::operator - ( const double4& v) const
        {
            return double4(x-v.x, y-v.y, z-v.z, w-v.w);
        }

        
        double4 double4::operator - ( const double* const v ) const
        {
            return double4(x-v[0], y-v[1], z-v[2], w-v[3]);
        }

        // *
        
        double4 double4::operator * ( const double4& v) const
        {
            return double4(x*v.x, y*v.y, z*v.z, w*v.w);
        }

        
        double4 double4::operator * ( const double* const v ) const
        {
            return double4(x*v[0], y*v[1], z*v[2], w*v[3]);
        }

        
        double4 double4::operator * ( const double& v ) const
        {
            return double4(x*v, y*v, z*v, w*v);
        }

        // /
        
        double4 double4::operator / ( const double4& v) const
        {
            return double4(x/v.x, y/v.y, z/v.z, w/v.w);
        }

        
        double4 double4::operator / ( const double* const v ) const
        {
            return double4(x/v[0], y/v[1], z/v[2], w/v[3]);
        }

        
        double4 double4::operator / ( const double& v ) const
        {
            return double4(x/v, y/v, z/v, w/v);
        }

        // +=
        
        void double4::operator += ( const double4& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
        }

        
        void double4::operator += ( const double* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
            w += v[3];
        }

        // -=
        
        void double4::operator -= ( const double4& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
        }

        
        void double4::operator -= ( const double* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
            w -= v[3];
        }

        // *=
        
        void double4::operator *= ( const double4& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
        }

        
        void double4::operator *= ( const double* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
            w *= v[3];
        }

        
        void double4::operator *= ( const double& v )
        {
            x *= v;
            y *= v;
            z *= v;
            w *= v;
        }

        // /=
        
        void double4::operator /= ( const double4& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
        }

        
        void double4::operator /= ( const double* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
            w /= v[3];
        }

        
        void double4::operator /= ( const double& v )
        {
            x /= v;
            y /= v;
            z /= v;
            w /= v;
        }

        // Non-operator functions
        
        double double4::length() const
        {
            return sqrt( (double)(x * x + y * y + z * z + w * w ) );
        }

        
        double double4::squaredLength() const
        {
            return x * x + y * y + z * z + w * w;
        }

        
        double double4::dot( const double4& v) const
        {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        
        void double4::normalizeIt()
        {
            const double length = sqrt( (double)(x * x + y * y + z*z + w*w) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (double)(x/length);
                y = (double)(y/length);
                z = (double)(z/length);
                w = (double)(w/length);
            }
        }

        
        double4 double4::normalize() const
        {
            const double length = sqrt( (double)(x * x + y * y + z*z + w*w) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return double4((double)(x/length), (double)(y/length), (double)(z/length), (double)(w/length));
            }
            return *this;
        }

    }
}
