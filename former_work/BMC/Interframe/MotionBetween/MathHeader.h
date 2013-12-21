/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Header file of the math classes

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#ifndef	__LG_MATH_HEADER_H__
#define	__LG_MATH_HEADER_H__

#include <math.h>
#include <assert.h>

// the float zero value
#ifndef EPSILON
#define EPSILON 1.e-8
#endif

// print debugging infomations
#ifndef PRINT
//#define PRINT   NULL
#define PRINT   printf
#endif

#include "Vector3.h"
#include "Matrix3x3.h"
#include "Quaternion.h"

#include "Utilities.h"


#endif	// __LG_MATH_HEADER_H__
