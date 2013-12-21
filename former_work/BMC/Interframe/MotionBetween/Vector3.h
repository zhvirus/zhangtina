/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Interfaces of class CVector3LG

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#ifndef __LG_VECTOR_3__
#define __LG_VECTOR_3__

#include "MathHeader.h"

class CMatrix3x3LG;

// class of 3D vector
class CVector3LG
{
private:
    float p[3];

public:
    // constructors
    CVector3LG() { p[0]=0; p[1]=0; p[2]=0; }
    CVector3LG( float x, float y, float z ) { p[0]=x; p[1]=y; p[2]=z; }
    CVector3LG( float a[3] ) { p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; }
	CVector3LG(const CVector3LG& a);
	CVector3LG& operator=(const CVector3LG& a);

    // inquiry functions
    float& operator[](int i)  { return p[i]; }

    float x() const { return p[0]; };
    float y() const { return p[1]; };
    float z() const { return p[2]; };
    void   getValue( float d[3] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; }
    void   setValue( float d[3] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; }
	float getValue( int n ) const { return p[n]; }
	CVector3LG setValue( float x, float y, float z )
		   { p[0]=x, p[1]=y, p[2]=z; return *this; }
	float setValue( int n, float x )
		   { return p[n]=x; }

	float Length() const;
	float Length2() const; // return the square of the length

    // change functions
    void set_x( float x ) { p[0]=x; };
    void set_y( float x ) { p[1]=x; };
    void set_z( float x ) { p[2]=x; };



    //----------------------------------------------------------------------------
    // friend global routines
    //----------------------------------------------------------------------------
    friend  CMatrix3x3LG Rotation( float angle, CVector3LG const& axis );
    friend  CVector3LG Normalize( CVector3LG const& a );
    friend  CVector3LG operator/( CVector3LG const& a, float b );
    friend  CVector3LG operator*( CVector3LG const& a, float b );
    friend  CVector3LG operator*( float b, CVector3LG const& a );
    friend  float operator%( CVector3LG const& a, CVector3LG const& b );
    friend  CVector3LG operator+( CVector3LG const& a, CVector3LG const& b );
};


#endif  // __LG_VECTOR_3__

