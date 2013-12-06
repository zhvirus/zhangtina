#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Vector4.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        template <class T>
        bool operator == ( const T* const t, const Vector4<T>& v )
        {
            return VALUE_EQUAL<T>(v.x,t[0]) &&
                   VALUE_EQUAL<T>(v.y,t[1]) &&
                   VALUE_EQUAL<T>(v.z,t[2]) &&
                   VALUE_EQUAL<T>(v.w,t[3]);
        }

        // !=
        template <class T>
        bool operator != ( const T* const t, const Vector4<T>& v )
        {
             return !VALUE_EQUAL<T>(v.x,t[0]) ||
                    !VALUE_EQUAL<T>(v.y,t[1]) ||
                    !VALUE_EQUAL<T>(v.z,t[2]) ||
                    !VALUE_EQUAL<T>(v.w,t[3]);
        }

        // +
        template <class T>
        Vector4<T> operator + ( const T* const t, const Vector4<T>& v)
        {
            return Vector4<T>(t[0]+v.x, t[1]+v.y, t[2]+v.z, t[3]+v.w);
        }

        template <class T>
        Vector4<T> operator + ( const T& t, const Vector4<T>& v)
        {
            return Vector4<T>(t+v.x, t+v.y, t+v.z, t+v.w);
        }

        // -
        template <class T>
        Vector4<T> operator - ( const T* const t, const Vector4<T>& v)
        {
            return Vector4<T>(t[0]-v.x, t[1]-v.y, t[2]-v.z, t[3]-v.w);
        }

        template <class T>
        Vector4<T> operator - ( const T& t, const Vector4<T>& v)
        {
            return Vector4<T>(t-v.x, t-v.y, t-v.z, t-v.w);
        }
        // *
        template <class T>
        Vector4<T> operator * ( const T* const t, const Vector4<T>& v)
        {
            return Vector4<T>(t[0]*v.x, t[1]*v.y, t[2]*v.z, t[3]*v.w);
        }

        template <class T>
        Vector4<T> operator * ( const T& t, const Vector4<T>& v)
        {
            return Vector4<T>(t*v.x, t*v.y, t*v.z, t*v.w);
        }
        // /
        template <class T>
        Vector4<T> operator / ( const T* const t, const Vector4<T>& v)
        {
            return Vector4<T>(t[0]/v.x, t[1]/v.y, t[2]/v.z, t[3]/v.w);
        }

        template <class T>
        Vector4<T> operator / ( const T& t, const Vector4<T>& v)
        {
            return Vector4<T>(t/v.x, t/v.y, t/v.z, t/v.w);
        }
        // +=
        template <class T>
        void operator += ( T* const t, const Vector4<T>& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
            t[3] += v.w;
        }

        // -=
        template <class T>
        void operator -= ( T* const t, const Vector4<T>& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
            t[3] -= v.w;
        }
        // *=
        template <class T>
        void operator *= ( T* const t, const Vector4<T>& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
            t[3] *= v.w;
        }
        // /=
        template <class T>
        void operator /= ( T* const t, const Vector4<T>& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
            t[3] /= v.w;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        template<class T>
        Vector4<T>::Vector4():x((T)0),y((T)0),z((T)0),w((T)0)
        {
        }

        template<class T>
        Vector4<T>::Vector4( T a, T b, T c, T d)
        {
            x=a;
            y=b;
            z=c;
            w=d;
        }

        template<class T>
        Vector4<T>::Vector4( const Vector4<T>& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        template<class T>
        Vector4<T>::Vector4( const T* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
        }

        // []
        template<class T>
        T Vector4<T>::operator [] ( unsigned int idx ) const
        {
            assert( idx < 4 );
            return val[idx];
        }

        template<class T>
        T& Vector4<T>::operator [] ( unsigned int idx )
        {
            assert( idx < 4 );
            return val[idx];
        }

        // =
        template<class T>
        Vector4<T>& Vector4<T>::operator = ( const Vector4<T>& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }

        template<class T>
        Vector4<T>& Vector4<T>::operator = ( const T* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            w = v[3];
            return *this;
        }

        // ==
        template<class T>
        bool Vector4<T>::operator == ( const Vector4<T>& v) const
        {
            return VALUE_EQUAL<T>(v.x,x) &&
                   VALUE_EQUAL<T>(v.y,y) &&
                   VALUE_EQUAL<T>(v.z,z) &&
                   VALUE_EQUAL<T>(v.w,w);
        }

        template<class T>
        bool Vector4<T>::operator == ( const T* const v) const
        {
            return VALUE_EQUAL<T>(x,v[0]) &&
                   VALUE_EQUAL<T>(y,v[1]) &&
                   VALUE_EQUAL<T>(z,v[2]) &&
                   VALUE_EQUAL<T>(w,v[3]);
        }

        // !=
        template<class T>
        bool Vector4<T>::operator != ( const Vector4<T>& v) const
        {
            return !VALUE_EQUAL<T>(x,v.x) ||
                   !VALUE_EQUAL<T>(y,v.y) ||
                   !VALUE_EQUAL<T>(z,v.z) ||
                   !VALUE_EQUAL<T>(w,v.w);
        }

        template<class T>
        bool Vector4<T>::operator != ( const T* const v) const
        {
            return !VALUE_EQUAL<T>(x,v[0]) ||
                   !VALUE_EQUAL<T>(y,v[1]) ||
                   !VALUE_EQUAL<T>(z,v[2]) ||
                   !VALUE_EQUAL<T>(w,v[3]);
        }

        // +
        template<class T>
        Vector4<T> Vector4<T>::operator + ( const Vector4<T>& v) const
        {
            return Vector4<T>(x+v.x, y+v.y, z+v.z, w+v.w);
        }

        template<class T>
        Vector4<T> Vector4<T>::operator + ( const T* const v ) const
        {
            return Vector4<T>(x+v[0], y+v[1], z+v[2], w+v[3]);
        }

        // -
        template<class T>
        Vector4<T> Vector4<T>::operator - ( const Vector4<T>& v) const
        {
            return Vector4<T>(x-v.x, y-v.y, z-v.z, w-v.w);
        }

        template<class T>
        Vector4<T> Vector4<T>::operator - ( const T* const v ) const
        {
            return Vector4<T>(x-v[0], y-v[1], z-v[2], w-v[3]);
        }

        // *
        template<class T>
        Vector4<T> Vector4<T>::operator * ( const Vector4<T>& v) const
        {
            return Vector4<T>(x*v.x, y*v.y, z*v.z, w*v.w);
        }

        template<class T>
        Vector4<T> Vector4<T>::operator * ( const T* const v ) const
        {
            return Vector4<T>(x*v[0], y*v[1], z*v[2], w*v[3]);
        }

        template<class T>
        Vector4<T> Vector4<T>::operator * ( const T& v ) const
        {
            return Vector4<T>(x*v, y*v, z*v, w*v);
        }

        // /
        template<class T>
        Vector4<T> Vector4<T>::operator / ( const Vector4<T>& v) const
        {
            return Vector4<T>(x/v.x, y/v.y, z/v.z, w/v.w);
        }

        template<class T>
        Vector4<T> Vector4<T>::operator / ( const T* const v ) const
        {
            return Vector4<T>(x/v[0], y/v[1], z/v[2], w/v[3]);
        }

        template<class T>
        Vector4<T> Vector4<T>::operator / ( const T& v ) const
        {
            return Vector4<T>(x/v, y/v, z/v, w/v);
        }

        // +=
        template<class T>
        void Vector4<T>::operator += ( const Vector4<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
        }

        template<class T>
        void Vector4<T>::operator += ( const T* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
            w += v[3];
        }

        // -=
        template<class T>
        void Vector4<T>::operator -= ( const Vector4<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
        }

        template<class T>
        void Vector4<T>::operator -= ( const T* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
            w -= v[3];
        }

        // *=
        template<class T>
        void Vector4<T>::operator *= ( const Vector4<T>& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
        }

        template<class T>
        void Vector4<T>::operator *= ( const T* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
            w *= v[3];
        }

        template<class T>
        void Vector4<T>::operator *= ( const T& v )
        {
            x *= v;
            y *= v;
            z *= v;
            w *= v;
        }

        // /=
        template<class T>
        void Vector4<T>::operator /= ( const Vector4<T>& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
        }

        template<class T>
        void Vector4<T>::operator /= ( const T* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
            w /= v[3];
        }

        template<class T>
        void Vector4<T>::operator /= ( const T& v )
        {
            x /= v;
            y /= v;
            z /= v;
            w /= v;
        }

        // Non-operator functions
        template<class T>
        double Vector4<T>::length() const
        {
            return sqrt( (double)(x * x + y * y + z * z + w * w ) );
        }

        template<class T>
        double Vector4<T>::squaredLength() const
        {
            return x * x + y * y + z * z + w * w;
        }

        template<class T>
        double Vector4<T>::dot( const Vector4& v) const
        {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        template<class T>
        void Vector4<T>::normalizeIt()
        {
            const double length = sqrt( (double)(x * x + y * y + z*z + w*w) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (T)(x/length);
                y = (T)(y/length);
                z = (T)(z/length);
                w = (T)(w/length);
            }
        }

        template<class T>
        Vector4<T> Vector4<T>::normalize() const
        {
            const double length = sqrt( (double)(x * x + y * y + z*z + w*w) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return Vector4<T>((T)(x/length), (T)(y/length), (T)(z/length), (T)(w/length));
            }
            return *this;
        }

    }
}
