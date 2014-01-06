#ifndef MATH_COMMON_H
#define MATH_COMMON_H

#include "Common/ZHSTD.h"
#include <math.h>

namespace ZH{
    namespace Math{
        template<class T>
        bool VALUE_EQUAL( T a, T b ) { return fabs(double(a-b)) < 1e-6; }

        template<class T> 
        void VALUE_SWAP( T& a, T& b){ T s=a; a=b; b=s; }

        // export it
        ZH_TEMPLATE_DLL template ZH_DLL bool VALUE_EQUAL( float a, float b );
        ZH_TEMPLATE_DLL template ZH_DLL bool VALUE_EQUAL( double a, double b );
        ZH_TEMPLATE_DLL template ZH_DLL bool VALUE_EQUAL( int a, int b );
        ZH_TEMPLATE_DLL template ZH_DLL bool VALUE_EQUAL( unsigned int a, unsigned int b );
        ZH_TEMPLATE_DLL template ZH_DLL void VALUE_SWAP( float& a, float& b );
        ZH_TEMPLATE_DLL template ZH_DLL void VALUE_SWAP( double& a, double& b );
        ZH_TEMPLATE_DLL template ZH_DLL void VALUE_SWAP( int& a, int& b );
        ZH_TEMPLATE_DLL template ZH_DLL void VALUE_SWAP( unsigned int& a, unsigned int& b );
    }
} // namespace ZH


#endif