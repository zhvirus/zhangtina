/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Implementation of class CVector3LG

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#include "std.h"

#include "MathHeader.h"

CVector3LG::CVector3LG(const CVector3LG& a)
{
	for(int i=0;i<3;i++)
    {
		p[i] = a.p[i];
	}
}

CVector3LG& CVector3LG::operator=(const CVector3LG& a)
{
	for(int i=0;i<3;i++)
    {
		p[i] = a.p[i];
	}

	return (*this);
}

float CVector3LG::Length2() const
{
	return ( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] );
}

float  CVector3LG::Length() const
{
    return (float)sqrt( Length2() );
}
