#ifndef __VTL_PLATFORM_HPP__
#define __VTL_PLATFORM_HPP__

/*************************************************************************\
Microsoft Research Asia
Copyright (c) 2001 Microsoft Corporation

Module Name:
    Platform dependent defination

Abstract:
    Defines commands and macros dependent on platform.
    1. PrintError for debug use
    2. use system assert
    3. define vtl_inline for possible faster debug version
    4. use system tchar
    5. define sizetype as system size_t
    6. define functions for zero comparison
        a) AbsLessThan
        b) EqualsTo
        c) IsZero

Notes:
    Support win32 only

Usage:
    EqualsTo and IsZero has possible threshold parameter.
    Default threshold = std::numeric_limits<T>::epsilon()

History:
    Created  on 2003 Feb 13 by liyin@ust.hk
    Modified on 2003 Mar 21 by liyin@ust.hk

\*************************************************************************/

// --------------------------------------------------------------------
// Platform independent dependecy
// --------------------------------------------------------------------
#include <stdio.h>

#ifndef VTL_NO_VERBOSE_ERROR
#define PrintError  \
            printf("Line %d, file %s\nError: ", __LINE__, __FILE__);\
            printf
#else
    #ifndef VTL_ABSOLUTE_QUIET
        #define PrintError printf
    #else
        #define PrintError __noop
    #endif
#endif

#ifndef _WIN32
#error VTL does not support non-win32 platform yet!
#endif

// --------------------------------------------------------------------
// _WIN32 dependency
// --------------------------------------------------------------------
#include <windows.h>

// --------------------------------------------------------------------
// Assertion support
// --------------------------------------------------------------------
#ifndef assert
#include <assert.h>
#endif

// --------------------------------------------------------------------
// TODO: exception support
// --------------------------------------------------------------------


// --------------------------------------------------------------------
// inline alias, can be invalide by macro
// --------------------------------------------------------------------
#ifndef VTL_DISABLE_FORCEINLINE
#define vtl_inline __forceinline
#else
#define vtl_inline __inline
#endif

// Unicode support
// --------------------------------------------------------------------
#include "tchar.h"

// --------------------------------------------------------------------
// VTL data type define
// --------------------------------------------------------------------
// vtl is using the following types
// BYTE, char	// for 8 bit data
// WORD, short	// for 16 bit data
// DWORD, long	// for 32 bit data
// UINT, int	// for integer flag or count, unkown length
// float		// 32 bit floating point
// double		// 64 bit floating point
// long double	// 64 bit floating point

typedef size_t sizetype;


// --------------------------------------------------------------------
// std::numeric_limits<class T>
// It's defined for most data, char, short, long, int, long double long double
// signed and unsigned version
// useful function include
//      1. denorm_min:  // return minimum denormalized value
//      2. infinity     // return positive infinity
//      3. epsilon      // return smallest effective increment from 1.0
// --------------------------------------------------------------------
#include <limits>

namespace vtl
{
    // These limits helper functions are used for long double, long double, long double

    // Absolute value of a is less than b
    template<class T>
        bool AbsLessThan(T a, T b)
    {
        return a < b && a > -b;;
    }

    // if a and be are close enough as same value
    template<class T>
        bool EqualsTo(T a, T b, T threshold = std::numeric_limits<T>::epsilon())
    {
        return AbsLessThan (a - b, threshold);
    }

    // if a is close enough as zero
    template<class T>
        bool IsZero(T a, T threshold = std::numeric_limits<T>::epsilon())
    {
        return AbsLessThan(a, threshold);
    }

}// namespace


#endif//__VTL_PLATFORM_HPP__
