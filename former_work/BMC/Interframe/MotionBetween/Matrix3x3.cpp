/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Implementation of class CMatrix3x3LG

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/



#include "MathHeader.h"


CMatrix3x3LG& CMatrix3x3LG::operator=(const CMatrix3x3LG& a)
{
	for( int i=0; i<3; i++ )
    {
	    for( int j=0; j<3; j++ )
		    p[i][j] = a.getValue(i,j) ;
    }
    return (*this);
}
