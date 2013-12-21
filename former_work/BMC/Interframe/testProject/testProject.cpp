// testProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include "windows.h"
#include <MMSystem.h>
#include <stdlib.h>
#include "xmmintrin.h"
#include <vector>
#include <iostream>

#pragma comment(lib,"Winmm.lib")

class __declspec(align(16)) vector4 {
public:
    union {
        struct { float x, y, z, w; };
        __m128 mm;
    };
};
class CVector3f
{
public:
    union	{
        struct	{	float	data[3];	};
        struct	{	float	x, y, z;	};
        struct	{	float	n, m, l;	};
        struct	{	float	r, g, b;	};
        struct	{	float	u, v;		};
        __m128 mm;
    };
};
inline void dot3( CVector3f & r, const CVector3f & x, const CVector3f & y )
{
    /**//*register vector4    s;
        s.mm = _mm_mul_ps( x.mm , y.mm );
        r.mm = _mm_add_ss( s.mm , _mm_movehl_ps( s.mm , s.mm ) );
        r.mm = _mm_add_ss( r.mm , _mm_shuffle_ps( r.mm , r.mm , 1 ) );*/
    __asm {
            mov        ecx, y
            mov        edx, x
            mov        eax, r
            movaps    xmm0, xmmword ptr [ecx]
            movaps    xmm1, xmmword ptr [edx]
            mulps    xmm1, xmm0        //    xmm1 = s0, s1, s2, s3
            movhlps    xmm0, xmm1        //    xmm0 = s2, s3, *, *
            addss    xmm0, xmm1        //    xmm0 = s0+s2, s3, *, *
            shufps    xmm1, xmm1, 1    //    xmm1 = s1, s0, s0, s0
            addss    xmm0, xmm1        //    xmm0 = s0+s2+s1, s3, *, *
            movaps    xmmword ptr [eax], xmm0
    }
}

inline void dot4( vector4 *r, const vector4 *x, const vector4 *y )
{
    /**//*register vector4    s;
        s.mm = _mm_mul_ps( x->mm , y->mm );
        r->mm = _mm_add_ss( s.mm , _mm_movehl_ps( s.mm , s.mm ) );
        r->mm = _mm_add_ss( r->mm , _mm_shuffle_ps( r->mm , r->mm , 1 ) );
        r->mm = _mm_add_ss( r->mm , _mm_shuffle_ps( r->mm , r->mm , 3 ) );*/
    __asm {
        mov        ecx, y
            mov        edx, x
            mov        eax, r
            movaps    xmm0, xmmword ptr [ecx]
        movaps    xmm1, xmmword ptr [edx]
        mulps    xmm1, xmm0        //    xmm1 = s0, s1, s2, s3
            movhlps    xmm0, xmm1        //    xmm0 = s2, s3, *, *
            addss    xmm0, xmm1        //    xmm0 = s0+s2, s3, *, *
            shufps    xmm1, xmm1, 253    //    xmm1 = s1, s3, s3, s3
            addss    xmm0, xmm1        //    xmm0 = s0+s2+s1, s3, *, *
            movhlps    xmm1, xmm1        //    xmm1 = s3, s3, s3, s3
            addss    xmm0, xmm1        //    xmm0 = s0+s2+s1+s3, s3, *, *
            movaps    xmmword ptr [eax], xmm0
    }
}
int _tmain(int argc, _TCHAR* argv[])
{
   // float x[]={1,2,3,4};
   // float y[]={1,2,3,4};
    //float r[4];




    //CVector3f a,b,c;
    //a.x    =1;
    //a.y    = 2;
    //a.z    =3;

    //b.x    = 1;
    //b.y    =2;
    //b.z    = 3;



    //dot3(c,a,b);

    //float x,y,z;

    //CVector3f dddd;

   // printf("%x\n%x\n%x\n%d\n",&(x),&(y),&(z),sizeof(dddd));


//    int k=0;
    
    class RR
    {
    public:
        int a;
        float b;
        double k;
    };

    std::vector<RR> ss;
    RR a,b;
    ss.push_back(a);
    ss.push_back(b);
    ss.push_back(a);
    ss.push_back(b);
    ss.push_back(a);
    ss.push_back(b);
    ss.push_back(a);
    ss.push_back(b);
    ss.push_back(a);
    ss.push_back(b);
    ss.push_back(a);
    ss.push_back(b);

    printf("%d",sizeof(ss));
   // int k=ss[2].a;
    int k;


    system("PAUSE");


	return 0;
}

