#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Vector3.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        template <class T>
        bool operator == ( const T* const t, const Vector3<T>& v )
        {
            return VALUE_EQUAL<T>(v.x,t[0]) &&
                   VALUE_EQUAL<T>(v.y,t[1]) &&
                   VALUE_EQUAL<T>(v.z,t[2]);
        }

        // !=
        template <class T>
        bool operator != ( const T* const t, const Vector3<T>& v )
        {
             return !VALUE_EQUAL<T>(v.x,t[0]) ||
                    !VALUE_EQUAL<T>(v.y,t[1]) ||
                    !VALUE_EQUAL<T>(v.z,t[2]);
        }

        // +
        template <class T>
        Vector3<T> operator + ( const T* const t, const Vector3<T>& v)
        {
            return Vector3<T>(t[0]+v.x, t[1]+v.y, t[2]+v.z);
        }

        template <class T>
        Vector3<T> operator + ( const T& t, const Vector3<T>& v)
        {
            return Vector3<T>(t+v.x, t+v.y, t+v.z);
        }

        // -
        template <class T>
        Vector3<T> operator - ( const T* const t, const Vector3<T>& v)
        {
            return Vector3<T>(t[0]-v.x, t[1]-v.y, t[2]-v.z);
        }

        template <class T>
        Vector3<T> operator - ( const T& t, const Vector3<T>& v)
        {
            return Vector3<T>(t-v.x, t-v.y, t-v.z);
        }
        // *
        template <class T>
        Vector3<T> operator * ( const T* const t, const Vector3<T>& v)
        {
            return Vector3<T>(t[0]*v.x, t[1]*v.y, t[2]*v.z);
        }

        template <class T>
        Vector3<T> operator * ( const T& t, const Vector3<T>& v)
        {
            return Vector3<T>(t*v.x, t*v.y, t*v.z);
        }
        // /
        template <class T>
        Vector3<T> operator / ( const T* const t, const Vector3<T>& v)
        {
            return Vector3<T>(t[0]/v.x, t[1]/v.y, t[2]/v.z);
        }

        template <class T>
        Vector3<T> operator / ( const T& t, const Vector3<T>& v)
        {
            return Vector3<T>(t/v.x, t/v.y, t/v.z);
        }
        // +=
        template <class T>
        void operator += ( T* const t, const Vector3<T>& v)
        {
            t[0] += v.x;
            t[1] += v.y;
            t[2] += v.z;
        }

        // -=
        template <class T>
        void operator -= ( T* const t, const Vector3<T>& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
            t[2] -= v.z;
        }
        // *=
        template <class T>
        void operator *= ( T* const t, const Vector3<T>& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
            t[2] *= v.z;
        }
        // /=
        template <class T>
        void operator /= ( T* const t, const Vector3<T>& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
            t[2] /= v.z;
        }
        // ---------------------------------
        // class function
        // ---------------------------------
        template<class T>
        Vector3<T>::Vector3():x((T)0),y((T)0),z((T)0)
        {
        }

        template<class T>
        Vector3<T>::Vector3( T a, T b, T c)
        {
            x=a;
            y=b;
            z=c;
        }

        template<class T>
        Vector3<T>::Vector3( const Vector3<T>& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }

        template<class T>
        Vector3<T>::Vector3( const T* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
        }

        // []
        template<class T>
        T Vector3<T>::operator [] ( unsigned int idx ) const
        {
            assert( idx < 3 );
            return val[idx];
        }

        template<class T>
        T& Vector3<T>::operator [] ( unsigned int idx )
        {
            assert( idx < 3 );
            return val[idx];
        }

        // =
        template<class T>
        Vector3<T>& Vector3<T>::operator = ( const Vector3<T>& v )
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        template<class T>
        Vector3<T>& Vector3<T>::operator = ( const T* const v )
        {
            x = v[0];
            y = v[1];
            z = v[2];
            return *this;
        }

        // ==
        template<class T>
        bool Vector3<T>::operator == ( const Vector3<T>& v) const
        {
            return VALUE_EQUAL<T>(v.x,x) &&
                   VALUE_EQUAL<T>(v.y,y) &&
                   VALUE_EQUAL<T>(v.z,z);
        }

        template<class T>
        bool Vector3<T>::operator == ( const T* const v) const
        {
            return VALUE_EQUAL<T>(x,v[0]) &&
                   VALUE_EQUAL<T>(y,v[1]) &&
                   VALUE_EQUAL<T>(z,v[2]);
        }

        // !=
        template<class T>
        bool Vector3<T>::operator != ( const Vector3<T>& v) const
        {
            return !VALUE_EQUAL<T>(x,v.x) ||
                   !VALUE_EQUAL<T>(y,v.y) ||
                   !VALUE_EQUAL<T>(z,v.z);
        }

        template<class T>
        bool Vector3<T>::operator != ( const T* const v) const
        {
            return !VALUE_EQUAL<T>(x,v[0]) ||
                   !VALUE_EQUAL<T>(y,v[1]) ||
                   !VALUE_EQUAL<T>(z,v[2]);
        }

        // +
        template<class T>
        Vector3<T> Vector3<T>::operator + ( const Vector3<T>& v) const
        {
            return Vector3<T>(x+v.x, y+v.y, z+v.z);
        }

        template<class T>
        Vector3<T> Vector3<T>::operator + ( const T* const v ) const
        {
            return Vector3<T>(x+v[0], y+v[1], z+v[2]);
        }

        // -
        template<class T>
        Vector3<T> Vector3<T>::operator - ( const Vector3<T>& v) const
        {
            return Vector3<T>(x-v.x, y-v.y, z-v.z);
        }

        template<class T>
        Vector3<T> Vector3<T>::operator - ( const T* const v ) const
        {
            return Vector3<T>(x-v[0], y-v[1], z-v[2]);
        }

        // *
        template<class T>
        Vector3<T> Vector3<T>::operator * ( const Vector3<T>& v) const
        {
            return Vector3<T>(x*v.x, y*v.y, z*v.z);
        }

        template<class T>
        Vector3<T> Vector3<T>::operator * ( const T* const v ) const
        {
            return Vector3<T>(x*v[0], y*v[1], z*v[2]);
        }

        template<class T>
        Vector3<T> Vector3<T>::operator * ( const T& v ) const
        {
            return Vector3<T>(x*v, y*v, z*v);
        }

        // /
        template<class T>
        Vector3<T> Vector3<T>::operator / ( const Vector3<T>& v) const
        {
            return Vector3<T>(x/v.x, y/v.y, z/v.z);
        }

        template<class T>
        Vector3<T> Vector3<T>::operator / ( const T* const v ) const
        {
            return Vector3<T>(x/v[0], y/v[1], z/v[2]);
        }

        template<class T>
        Vector3<T> Vector3<T>::operator / ( const T& v ) const
        {
            return Vector3<T>(x/v, y/v, z/v);
        }

        // +=
        template<class T>
        void Vector3<T>::operator += ( const Vector3<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        template<class T>
        void Vector3<T>::operator += ( const T* const v )
        {
            x += v[0];
            y += v[1];
            z += v[2];
        }

        // -=
        template<class T>
        void Vector3<T>::operator -= ( const Vector3<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }

        template<class T>
        void Vector3<T>::operator -= ( const T* const v )
        {
            x -= v[0];
            y -= v[1];
            z -= v[2];
        }

        // *=
        template<class T>
        void Vector3<T>::operator *= ( const Vector3<T>& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }

        template<class T>
        void Vector3<T>::operator *= ( const T* const v )
        {
            x *= v[0];
            y *= v[1];
            z *= v[2];
        }

        template<class T>
        void Vector3<T>::operator *= ( const T& v )
        {
            x *= v;
            y *= v;
            z *= v;
        }

        // /=
        template<class T>
        void Vector3<T>::operator /= ( const Vector3<T>& v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
        }

        template<class T>
        void Vector3<T>::operator /= ( const T* const v )
        {
            x /= v[0];
            y /= v[1];
            z /= v[2];
        }

        template<class T>
        void Vector3<T>::operator /= ( const T& v )
        {
            x /= v;
            y /= v;
            z /= v;
        }

        // Non-operator functions
        template<class T>
        double Vector3<T>::length() const
        {
            return sqrt( (double)(x * x + y * y + z*z ) );
        }

        template<class T>
        double Vector3<T>::squaredLength() const
        {
            return x * x + y * y + z * z ;
        }

        template<class T>
        double Vector3<T>::dot( const Vector3& v) const
        {
            return x * v.x + y * v.y + z * v.z ;
        }

        template<class T>
        Vector3<T> Vector3<T>::cross( const Vector3<T>& v) const
        {
            return Vector3<T>(y*v.z-z*v.y, z*v.x-x*v.z,x*v.y-y*v.x);
        }

        template<class T>
        void Vector3<T>::normalizeIt()
        {
            const double length = sqrt( (double)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                x = (T)(x/length);
                y = (T)(y/length);
                z = (T)(z/length);
            }
        }

        template<class T>
        Vector3<T> Vector3<T>::normalize() const
        {
            const double length = sqrt( (double)(x * x + y * y + z*z ) );
            assert( length > 0.0f );
            if ( length > 0.0f ){
                return Vector3<T>((T)(x/length), (T)(y/length), (T)(z/length));
            }
            return *this;
        }

    }
}
