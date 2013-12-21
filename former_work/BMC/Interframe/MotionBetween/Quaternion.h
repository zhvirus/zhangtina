/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Interfaces of class CQuaternion

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#ifndef __LG_QUATERNION__
#define __LG_QUATERNION__

class CVector3LG;

// class of quaternion
class CQuaternion
{
private:
    float p[4];

public:
    CQuaternion() {};
	CQuaternion(const CQuaternion& qua);
    CQuaternion( float w, float x, float y, float z )
    { 
        p[0]=w; p[1]=x; p[2]=y; p[3]=z; 
    }
    CQuaternion( float a[4] ) 
    { 
        p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; p[3]=a[3]; 
    }

	CQuaternion  Normalize() const;
	float  Length() const;

    CQuaternion& operator=(const CQuaternion& qua);

    void getValue( float d[4] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; d[2]=p[2]; }
    void setValue( float d[4] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; p[2]=d[2]; }

    float w() const { return p[0]; }
    float x() const { return p[1]; }
    float y() const { return p[2]; }
    float z() const { return p[3]; }


    //----------------------------------------------------------------------------
    // friend global routines
    //----------------------------------------------------------------------------
    friend  CQuaternion operator/(CQuaternion const& a, float b);
    friend  CQuaternion Matrix2Quater( CMatrix3x3LG const& m );
    friend  CMatrix3x3LG Quater2Matrix( CQuaternion const& q );
    friend  CQuaternion  Slerp( CQuaternion const& a, CQuaternion const& b, float t );
    
    friend  float operator% (CQuaternion const& a, CQuaternion const& b);
    friend  CQuaternion operator-(CQuaternion const& a);
    friend  CQuaternion operator* (float a, CQuaternion const& b);
    friend  CQuaternion operator* (CQuaternion const& a, float b);
    friend  CQuaternion operator+ (CQuaternion const& a, CQuaternion const& b);

    friend  CQuaternion Exp(CVector3LG const& w);
    friend  CQuaternion  Closetofirst(CQuaternion const& a, CQuaternion const & b);
    friend  CQuaternion  Doublequater(CQuaternion const& a, CQuaternion const& b);
    friend  CQuaternion  Bisect(CQuaternion const& a, CQuaternion const& b);
    friend  CQuaternion operator-(CQuaternion const& a, CQuaternion const& b);
    friend  CQuaternion  DeCasteljau(CQuaternion const& q1, CQuaternion const& q2,CQuaternion const& q3, CQuaternion const& q4,float t);
};

#endif  // __LG_QUATERNION__
