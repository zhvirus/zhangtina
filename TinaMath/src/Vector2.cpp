#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Vector2.h"
#include <math.h>
#include "Math/MathCommon.h"

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        template <class T>
        bool operator == ( const T* const t, const Vector2<T>& v )
        {
            return VALUE_EQUAL<T>(v.x,t[0]) && VALUE_EQUAL<T>(v.y,t[1]);
        }

        // !=
        template <class T>
        bool operator != ( const T* const t, const Vector2<T>& v )
        {
             return !VALUE_EQUAL<T>(v.x,t[0]) || !VALUE_EQUAL<T>(v.y,t[1]);
        }

        // +
        template <class T>
        Vector2<T> operator + ( const T* const t, const Vector2<T>& v)
        {
            return Vector2<T>(t[0]+v.x, t[1]+v.y);
        }

        template <class T>
        Vector2<T> operator + ( const T& t, const Vector2<T>& v)
        {
            return Vector2<T>(t+v.x, t+v.y);
        }

        // -
        template <class T>
        Vector2<T> operator - ( const T* const t, const Vector2<T>& v)
        {
            return Vector2<T>(t[0]-v.x, t[1]-v.y);
        }

        template <class T>
        Vector2<T> operator - ( const T& t, const Vector2<T>& v)
        {
            return Vector2<T>(t-v.x, t-v.y);
        }
        // *
        template <class T>
        Vector2<T> operator * ( const T* const t, const Vector2<T>& v)
        {
            return Vector2<T>(t[0]*v.x, t[1]*v.y);
        }

        template <class T>
        Vector2<T> operator * ( const T& t, const Vector2<T>& v)
        {
            return Vector2<T>(t*v.x, t*v.y);
        }
        // /
        template <class T>
        Vector2<T> operator / ( const T* const t, const Vector2<T>& v)
        {
            return Vector2<T>(t[0]/v.x, t[1]/v.y);
        }

        template <class T>
        Vector2<T> operator / ( const T& t, const Vector2<T>& v)
        {
            return Vector2<T>(t/v.x, t/v.y);
        }
        // +=
        template <class T>
        void operator += ( T* const t, const Vector2<T>& v)
        {
            t[0] += v.x;
            t[1] += v.y;
        }

        // -=
        template <class T>
        void operator -= ( T* const t, const Vector2<T>& v)
        {
            t[0] -= v.x;
            t[1] -= v.y;
        }
        // *=
        template <class T>
        void operator *= ( T* const t, const Vector2<T>& v)
        {
            t[0] *= v.x;
            t[1] *= v.y;
        }
        // /=
        template <class T>
        void operator /= ( T* const t, const Vector2<T>& v)
        {
            t[0] /= v.x;
            t[1] /= v.y;
        }

        // ---------------------------------
        // class function
        // ---------------------------------
        template<class T>
        Vector2<T>::Vector2():x((T)0),y((T)0)
        {
        }

        template<class T>
        Vector2<T>::Vector2( T a, T b)
        {
            x=a;
            y=b;
        }

        template<class T>
        Vector2<T>::Vector2( const Vector2<T>& v )
        {
            x = v.x;
            y = v.y;
        }

        template<class T>
        Vector2<T>::Vector2( const T* const v )
        {
            x = v[0];
            y = v[1];
        }

        // []
        template<class T>
        T Vector2<T>::operator [] ( unsigned int idx ) const
        {
            assert( idx < 2 );
            return val[idx];
        }

        template<class T>
        T& Vector2<T>::operator [] ( unsigned int idx )
        {
            assert( idx < 2 );
            return val[idx];
        }

        // =
        template<class T>
        Vector2<T>& Vector2<T>::operator = ( const Vector2<T>& v )
        {
            x = v.x;
            y = v.y;
            return *this;
        }

        template<class T>
        Vector2<T>& Vector2<T>::operator = ( const T* const v )
        {
            x = v[0];
            y = v[1];
            return *this;
        }

        // ==
        template<class T>
        bool Vector2<T>::operator == ( const Vector2<T>& v) const
        {
            return VALUE_EQUAL<T>(v.x,x) && VALUE_EQUAL<T>(v.y,y);
        }

        template<class T>
        bool Vector2<T>::operator == ( const T* const v) const
        {
            return VALUE_EQUAL<T>(x,v[0]) && VALUE_EQUAL<T>(y,v[1]);
        }

        // !=
        template<class T>
        bool Vector2<T>::operator != ( const Vector2<T>& v) const
        {
            return !VALUE_EQUAL<T>(x,v.x) || !VALUE_EQUAL<T>(y,v.y);
        }

        template<class T>
        bool Vector2<T>::operator != ( const T* const v) const
        {
            return !VALUE_EQUAL<T>(x,v[0]) || !VALUE_EQUAL<T>(y,v[1]);
        }

        // +
        template<class T>
        Vector2<T> Vector2<T>::operator + ( const Vector2<T>& v) const
        {
            return Vector2<T>(x+v.x, y+v.y);
        }

        template<class T>
        Vector2<T> Vector2<T>::operator + ( const T* const v ) const
        {
            return Vector2<T>(x+v[0], y+v[1]);
        }

        // -
        template<class T>
        Vector2<T> Vector2<T>::operator - ( const Vector2<T>& v) const
        {
            return Vector2<T>(x-v.x, y-v.y);
        }

        template<class T>
        Vector2<T> Vector2<T>::operator - ( const T* const v ) const
        {
            return Vector2<T>(x-v[0], y-v[1]);
        }

        // *
        template<class T>
        Vector2<T> Vector2<T>::operator * ( const Vector2<T>& v) const
        {
            return Vector2<T>(x*v.x, y*v.y);
        }

        template<class T>
        Vector2<T> Vector2<T>::operator * ( const T* const v ) const
        {
            return Vector2<T>(x*v[0], y*v[1]);
        }

        template<class T>
        Vector2<T> Vector2<T>::operator * ( const T& v ) const
        {
            return Vector2<T>(x*v, y*v);
        }

        // /
        template<class T>
        Vector2<T> Vector2<T>::operator / ( const Vector2<T>& v) const
        {
            return Vector2<T>(x/v.x, y/v.y);
        }

        template<class T>
        Vector2<T> Vector2<T>::operator / ( const T* const v ) const
        {
            return Vector2<T>(x/v[0], y/v[1]);
        }

        template<class T>
        Vector2<T> Vector2<T>::operator / ( const T& v ) const
        {
            return Vector2<T>(x/v, y/v);
        }

        // +=
        template<class T>
        void Vector2<T>::operator += ( const Vector2<T>& v)
        {
            x += v.x;
            y += v.y;
        }

        template<class T>
        void Vector2<T>::operator += ( const T* const v )
        {
            x += v[0];
            y += v[1];
        }

        // -=
        template<class T>
        void Vector2<T>::operator -= ( const Vector2<T>& v)
        {
            x -= v.x;
            y -= v.y;
        }

        template<class T>
        void Vector2<T>::operator -= ( const T* const v )
        {
            x -= v[0];
            y -= v[1];
        }

        // *=
        template<class T>
        void Vector2<T>::operator *= ( const Vector2<T>& v)
        {
            x *= v.x;
            y *= v.y;
        }

        template<class T>
        void Vector2<T>::operator *= ( const T* const v )
        {
            x *= v[0];
            y *= v[1];
        }

        template<class T>
        void Vector2<T>::operator *= ( const T& v )
        {
            x *= v;
            y *= v;
        }

        // /=
        template<class T>
        void Vector2<T>::operator /= ( const Vector2<T>& v)
        {
            x /= v.x;
            y /= v.y;
        }

        template<class T>
        void Vector2<T>::operator /= ( const T* const v )
        {
            x /= v[0];
            y /= v[1];
        }

        template<class T>
        void Vector2<T>::operator /= ( const T& v )
        {
            x /= v;
            y /= v;
        }

        // Non-operator functions
        template<class T>
        double Vector2<T>::length() const
        {
            return sqrt( (double)(x * x + y * y) );
        }

        template<class T>
        double Vector2<T>::squaredLength() const
        {
            return x * x + y * y;
        }

        template<class T>
        double Vector2<T>::dot( const Vector2& v) const
        {
            return x * v.x + y * v.y;
        }
    }
}
