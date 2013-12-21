/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Interfaces of utility routines for the vector, matrix, and quaternion

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#ifndef __LG_UTILITIES__
#define __LG_UTILITIES__

class CQuaternion;
class CMatrix3x3LG;
class CVector3LG;

bool FloatsEqual( float a, float );

CQuaternion operator/(CQuaternion const& a, float b);
CQuaternion Matrix2Quater( CMatrix3x3LG const& m );
CMatrix3x3LG Quater2Matrix( CQuaternion const& q );
CQuaternion  Slerp( CQuaternion const& a, CQuaternion const& b, float t );

float operator% (CQuaternion const& a, CQuaternion const& b);
CQuaternion operator-(CQuaternion const& a);
CQuaternion operator* (float a, CQuaternion const& b);
CQuaternion operator* (CQuaternion const& a, float b);
CQuaternion operator+ (CQuaternion const& a, CQuaternion const& b);

CMatrix3x3LG  EulerAngle2Matrix( const CVector3LG& v );
CVector3LG Matrix2EulerAngle( const CMatrix3x3LG& m );
CMatrix3x3LG Rotation( float angle, CVector3LG const& axis );

CMatrix3x3LG operator*( CMatrix3x3LG const& a, CMatrix3x3LG const& b );
CVector3LG normalize( CVector3LG const& a );
CVector3LG operator/( CVector3LG const& a, float b );
CQuaternion Exp(CVector3LG const& w);

CVector3LG operator*( CVector3LG const& a, float b );
CVector3LG operator*( float b, CVector3LG const& a );
float operator%( CVector3LG const& a, CVector3LG const& b );
CVector3LG LinearInterpolate( float t, CVector3LG const& a, CVector3LG const& b );
float LinearInterpolate( float t, float a, float b);

CVector3LG operator+( CVector3LG const& a, CVector3LG const& b );
CQuaternion  Closetofirst(CQuaternion const& a, CQuaternion const & b);
CQuaternion  Doublequater(CQuaternion const& a, CQuaternion const& b);
CQuaternion  Bisect(CQuaternion const& a, CQuaternion const& b);
CQuaternion operator-(CQuaternion const& a, CQuaternion const& b);

float DeCasteljau( float q1, float q2, float q3, float q4, float t );
CQuaternion  DeCasteljau(CQuaternion const& q1, CQuaternion const& q2,CQuaternion const& q3, CQuaternion const& q4,float t);

#endif  // __LG_UTILITIES__

