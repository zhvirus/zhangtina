/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Implementation of class CQuaternion

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/


#include "MathHeader.h"


CQuaternion::CQuaternion(const CQuaternion& qua)
{
	for(int i=0;i<4;i++)
    {
		p[i] = qua.p[i];
	}
}


float  CQuaternion::Length() const
{
    return (float)sqrt( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] + p[3]*p[3] );
}

CQuaternion  CQuaternion::Normalize() const
{
	return (*this)/Length();
}


CQuaternion&     CQuaternion::operator=(const CQuaternion& qua)
{
	for(int i=0;i<4;i++)
    {
		p[i] = qua.p[i];
	}
	return (*this);
}

