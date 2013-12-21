/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Interfaces of class CMatrix3x3LG

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/


#ifndef __LG_MATRIX_3x3__
#define __LG_MATRIX_3x3__

class CQuaternion;
class CVector3LG;

// class of 3x3 matrix ( rotation matrix indeed )
class CMatrix3x3LG
{
private:
    CVector3LG p[3];

public:
    // constructors
    CMatrix3x3LG()
    {
        for(int i=0;i<3;i++)
        {
		    for(int j=0;j<3;j++)
            {
			    p[i][j] = 0.0;
		    }
        }
    }

	CMatrix3x3LG( CVector3LG const& a, CVector3LG const& b, CVector3LG const& c )
	{
		p[0][0]=a.x(); p[0][1]=a.y(); p[0][2]=a.z();
		p[1][0]=b.x(); p[1][1]=b.y(); p[1][2]=b.z();
		p[2][0]=c.x(); p[2][1]=c.y(); p[2][2]=c.z();
	}

	CMatrix3x3LG& operator=(const CMatrix3x3LG& a);
		
	// inquiry functions

	float getValue( int row, int col ) const 
    { 
        return p[row].getValue( col );
    }
	
	float setValue( int row, int col, float x )
	{ 
        return p[row][col]=x; 
    }

    CVector3LG& operator[](int i) 
    { 
        return p[i]; 
    }


    //----------------------------------------------------------------------------
    // friend global routines
    //----------------------------------------------------------------------------
    friend  CMatrix3x3LG operator*( CMatrix3x3LG const& a, CMatrix3x3LG const& b );

    friend  CQuaternion Matrix2Quater( CMatrix3x3LG const& m );
    friend  CMatrix3x3LG Quater2Matrix( CQuaternion const& q );

    friend  CMatrix3x3LG  EulerAngle2Matrix( const CVector3LG& v );
    friend  CVector3LG Matrix2EulerAngle( const CMatrix3x3LG& m );
};


#endif  // __LG_MATRIX_3x3__

