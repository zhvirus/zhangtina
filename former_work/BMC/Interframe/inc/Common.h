// Yanyun Chen
// Internet Graphics Group, Microsoft Research Asia
// Auguest 2003
///////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "Vector3f.h"
#include "Color.h"

#ifndef _The_common_definition_
#define _The_common_definition_

#define MY_EPS 1.0e-8f
#define CVec3 CVector3f
#define CCol4 CColor4f

#define max3(a,b,c)         ( max ( max (a, b), c) )
#define min3(a,b,c)         ( min ( min (a, b), c) )
#define signof(a)           ( (a >= 0)? 1 : 0 )
#define RANDOM_0_1          ((float)rand()/(float)RAND_MAX)
#define RANDOM_N1_P1        ((rand()-RAND_MAX*0.5f)/(RAND_MAX*0.5f))
#define RANDOM_VEC	        (CVec3(2.f*rand()-RAND_MAX, 2.f*rand()-RAND_MAX,\
                             2.f*rand()-RAND_MAX).Normalized())

#define MY_DELETE(p)        { if (p) { delete (p);    (p)=NULL; } }
#define MY_DELETE_ARRAY(p)  { if (p) { delete [] (p); (p)=NULL; } }
#define MY_CLAMP(a,b,c)     { if ((a) < (b)) (a) = (b);\
	                          if ((a) > (c)) (a) = (c); }
#define MY_ABS(x) (((x) > 0.f)? (x) : -(x))


#endif //_The_common_definietion_