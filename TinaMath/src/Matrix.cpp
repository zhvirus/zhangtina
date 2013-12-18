#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Math/Matrix.h"
#include "Math/MathCommon.h"
#include <string.h>

namespace ZH{
    namespace Math{

        // ---------------------------------
        // Non-class function
        // ---------------------------------

        // ==
        template <class T>
        bool operator == ( const T* const t, const Matrix<T>& m )
        {
            return VALUE_EQUAL<T>(t[0], m.v[0]) && VALUE_EQUAL<T>(t[1], m.v[1]) &&
                   VALUE_EQUAL<T>(t[2], m.v[2]) && VALUE_EQUAL<T>(t[3], m.v[3]) &&
                   VALUE_EQUAL<T>(t[4], m.v[4]) && VALUE_EQUAL<T>(t[5], m.v[5]) &&
                   VALUE_EQUAL<T>(t[6], m.v[6]) && VALUE_EQUAL<T>(t[7], m.v[7]) &&
                   VALUE_EQUAL<T>(t[8], m.v[8]) && VALUE_EQUAL<T>(t[9], m.v[9]) &&
                   VALUE_EQUAL<T>(t[10], m.v[10]) && VALUE_EQUAL<T>(t[11], m.v[11]) &&
                   VALUE_EQUAL<T>(t[12], m.v[12]) && VALUE_EQUAL<T>(t[13], m.v[13]) &&
                   VALUE_EQUAL<T>(t[14], m.v[14]) && VALUE_EQUAL<T>(t[15], m.v[15])
                   ;
        }

        // !=
        template <class T>
        bool operator != ( const T* const t, const Matrix<T>& m )
        {
            return !(t==m);
        }


        // ---------------------------------
        // class function
        // ---------------------------------
        template<class T>
        Matrix<T>::Matrix()
        {
            identityIt();
        }

        template<class T>
        Matrix<T>::Matrix( const Matrix<T>& m)
        {
            memcpy((void*)v, (void*)m.v, sizeof(T)*16);
        }

        template<class T>
        Matrix<T>::Matrix( const T* const t)
        {
            memcpy((void*)v, (void*)t, sizeof(T)*16);
        }

        template<class T>
        bool Matrix<T>::operator == ( const Matrix<T>& m) const
        {
            return v == m;
        }

        template<class T>
        bool Matrix<T>::operator != ( const Matrix<T>& m) const
        {
            return v != m;
        }

        template<class T>
        void Matrix<T>::identityIt()
        {
            memset((void*)(v), 0, sizeof(T)*16);
            v[0]  = T(1);
            v[5]  = T(1);
            v[10] = T(1);
            v[15] = T(1);
        }
    }
}
