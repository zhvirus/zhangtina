/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Implementation of utility routines for the vector, matrix, and quaternion

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#include "Utilities.h"
#include "MathHeader.h"


bool FloatsEqual( float a, float b )
{
    if( fabs( a - b ) < EPSILON )
        return true ;
    else
        return false;
}


CQuaternion operator/(CQuaternion const& a, float b) 
{
    CQuaternion c;
    for(int i = 0; i < 4; i++)
    {
        c.p[i] = a.p[i] / b;
    }
    return c;
}


CQuaternion Matrix2Quater( CMatrix3x3LG const& m )
{
    CQuaternion q;

    float tr, s;
    int    i, j, k;
    static int next[3] = { 1, 2, 0 };

    tr = m.getValue(0,0) + m.getValue(1,1) + m.getValue(2,2);
    if ( tr > 0.0 )
    {
        s = (float)sqrt( tr + 1.0 );
        q.p[0] = (float)( s * 0.5 );
        s = (float)(0.5 / s);
        q.p[1] = ( m.getValue(1,2) - m.getValue(2,1) ) * s;
        q.p[2] = ( m.getValue(2,0) - m.getValue(0,2) ) * s;
        q.p[3] = ( m.getValue(0,1) - m.getValue(1,0) ) * s;
    }
    else
    {
        i = 0;
        if ( m.getValue(1,1) > m.getValue(0,0) ) i = 1;
        if ( m.getValue(2,2) > m.getValue(i,i) ) i = 2;

        j = next[i];
        k = next[j];

        s = (float)sqrt( (m.getValue(i,i)
					- (m.getValue(j,j) + m.getValue(k,k))) + 1.0 );
        q.p[i+1] = (float)(s * 0.5);
        s = (float)(0.5 / s);
        q.p[0]   = ( m.getValue(j,k) - m.getValue(k,j) ) * s;
        q.p[j+1] = ( m.getValue(i,j) + m.getValue(j,i) ) * s;
        q.p[k+1] = ( m.getValue(i,k) + m.getValue(k,i) ) * s;
    }

    return q;
}


CMatrix3x3LG Quater2Matrix( CQuaternion const& q )
{
    CMatrix3x3LG m;
    float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    s  = (float)(2.0/(q.Length()));
    xs = q.x() * s;  ys = q.y() * s;  zs = q.z() * s;
    wx = q.w() * xs; wy = q.w() * ys; wz = q.w() * zs;
    xx = q.x() * xs; xy = q.x() * ys; xz = q.x() * zs;
    yy = q.y() * ys; yz = q.y() * zs; zz = q.z() * zs;

	
    m.p[0][0] = (float)(1.0 - (yy + zz));
    m.p[1][0] = xy - wz;
    m.p[2][0] = xz + wy;
    m.p[0][1] = xy + wz;
    m.p[1][1] = (float)(1.0 - (xx + zz));
    m.p[2][1] = yz - wx;
    m.p[0][2] = xz - wy;
    m.p[1][2] = yz + wx;
    m.p[2][2] = (float)(1.0 - (xx + yy));

    return m;
}

CQuaternion  Slerp( CQuaternion const& a, CQuaternion const& b, float t )
{
	CQuaternion aa, bb;
	
    float c = a % b;
	if(c>=0)
    {
		aa = a;
		bb = b;
	}
    else
    {
		aa = a;
		bb = -b;
		c = -c;
	}


	float theta = (float)acos( c );
	float sinom = (float)sin( theta );

	if(sinom>EPSILON)
    {
	    return ((aa*((float)sin((1-t)*theta)) + bb*((float)sin(t*theta)))/sinom).Normalize();
	}
    else
    {
		return (aa*(1-t) + bb*t).Normalize();
		//return aa.Normalize();
	}
}

// dot product between 2 quaternions
float operator%(CQuaternion const& a, CQuaternion const& b)
{
    float c = 0;
    for(int i = 0; i < 4; i++)
        c += a.p[i] * b.p[i];
    return c;
}

// negation of quaternion
CQuaternion operator-(CQuaternion const& a)
{
    return CQuaternion( -a.p[0], -a.p[1], -a.p[2], -a.p[3] );
}

CQuaternion operator*(float a, CQuaternion const& b)
{
    CQuaternion c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a * b.p[i];
    return c;
}

CQuaternion operator*(CQuaternion const& a, float b)
{
    CQuaternion c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] * b;
    return c;
}

CQuaternion operator+(CQuaternion const& a, CQuaternion const& b)
{
    CQuaternion c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] + b.p[i];
    return c;
}


CVector3LG Matrix2EulerAngle( const CMatrix3x3LG& m )
{
    float siny = (float)(-m.getValue(0,2));
    float cosy = (float)(sqrt( 1.0f - siny*siny ));

	float sinx;
	float cosx;

	float sinz;
	float cosz;

	if ( cosy>EPSILON )
	{
	    sinx = m.getValue(1,2) / cosy;
		cosx = m.getValue(2,2) / cosy;

		sinz = m.getValue(0,1) / cosy;
		cosz = m.getValue(0,0) / cosy;
	}
	else
	{
	    sinx = - m.getValue(2,1);
		cosx =   m.getValue(1,1);

		sinz = 0.0;
		cosz = 1.0;
	}

	float x = (float)atan2( sinx, cosx );
	float y = (float)atan2( siny, cosy );
	float z = (float)atan2( sinz, cosz );

    return CVector3LG(x,y,z);
}

CMatrix3x3LG  EulerAngle2Matrix( const CVector3LG& v )
{
    CMatrix3x3LG m = 
        Rotation( v.x(), CVector3LG(1,0,0) ) *
        Rotation( v.y(), CVector3LG(0,1,0) ) *
        Rotation( v.z(), CVector3LG(0,0,1) );

    return m;
}

CVector3LG Normalize( CVector3LG const& a )
{
    float s = a.Length();

    if ( fabs(s)>EPSILON )
        return CVector3LG( a.p[0]/s, a.p[1]/s, a.p[2]/s );
    else
        return CVector3LG( 0.0, 0.0, 0.0);
}

CMatrix3x3LG Rotation( float angle, CVector3LG const& axis )
{
    CVector3LG v = angle * Normalize( axis );
    CQuaternion q = Exp( v/2.0 );
    return Quater2Matrix( q );
}

CQuaternion Exp(CVector3LG const& w)
{
    float theta = (float)sqrt(w % w);
    float sc;

    if(theta < EPSILON) 
        sc = 1.0f;
    else 
        sc = (float)(sin(theta) / theta);

    CVector3LG v = sc * w;
    return CQuaternion((float)cos(theta), v.x(), v.y(), v.z());
}

float operator%( CVector3LG const& a, CVector3LG const& b )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}

CVector3LG LinearInterpolate( float t, CVector3LG const& a, CVector3LG const& b )
{
	return (1.0f-t)*a + t*b;
}

float LinearInterpolate( float t, float a, float b)
{
    return (1.0f-t)*a + t*b;
}

CVector3LG operator+( CVector3LG const& a, CVector3LG const& b )
{
    CVector3LG c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

CQuaternion operator-(CQuaternion const& a, CQuaternion const& b)
{
    CQuaternion c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] - b.p[i];
    return c;
}

CQuaternion  Doublequater(CQuaternion const& a, CQuaternion const& b)
{
	//a and b should be unit quaternion
	CQuaternion result;
	result = 2.0f*(a%b)*b-a;

//	result.Normalize();
	return result;
}

float DeCasteljau( float q1, float q2, float q3, float q4, float t )
{
	float p1,p2,p3,p12,p23,p;
	
	p1 = LinearInterpolate(t,q1,q2);
	p2 = LinearInterpolate(t,q2,q3);
	p3 = LinearInterpolate(t,q3,q4);

	p12 = LinearInterpolate(t,p1,p2);
	p23 = LinearInterpolate(t,p2,p3);

	p= LinearInterpolate(t,p12,p23);

	return p;
}

CQuaternion  DeCasteljau(CQuaternion const& q1, CQuaternion const& q2,CQuaternion const& q3, CQuaternion const& q4,float t)
{
// simplified version
//	CQuaternion pp = Slerp(Slerp(q1,q4,t),Slerp(q2,q3,t),2*t*(1-t));

	CQuaternion p1,p2,p3,p12,p23,p;
	
	p1 = Slerp(q1,q2,t);
	p2 = Slerp(q2,q3,t);
	p3 = Slerp(q3,q4,t);

	p12 = Slerp(p1,p2,t);
	p23 = Slerp(p2,p3,t);

	p= Slerp(p12,p23,t);

	return p;
}


CQuaternion  Bisect(CQuaternion const& a, CQuaternion const& b)
{
	CQuaternion result;
	result = a+b;

	return result.Normalize();
}

CQuaternion  Closetofirst(CQuaternion const& a, CQuaternion const & b)
{
	float t;
	t = a%b;
	if (t>=0.0f) 
        return b;
	else 
        return -b;
}


CVector3LG operator*( CVector3LG const& a, float b )
{
    CVector3LG c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}


CVector3LG operator*( float b, CVector3LG const& a )
{
    CVector3LG c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}

CVector3LG operator/( CVector3LG const& a, float b )
{
    CVector3LG c;

    c.p[0] = a.p[0] / b;
    c.p[1] = a.p[1] / b;
    c.p[2] = a.p[2] / b;

    return c;
}


CMatrix3x3LG operator*( CMatrix3x3LG const& a, CMatrix3x3LG const& b )
{

	CVector3LG v1(  a.getValue(0,0)*b.getValue(0,0)
				+ a.getValue(0,1)*b.getValue(1,0)
				+ a.getValue(0,2)*b.getValue(2,0),
				  a.getValue(0,0)*b.getValue(0,1)
				+ a.getValue(0,1)*b.getValue(1,1)
				+ a.getValue(0,2)*b.getValue(2,1),
				  a.getValue(0,0)*b.getValue(0,2)
				+ a.getValue(0,1)*b.getValue(1,2)
				+ a.getValue(0,2)*b.getValue(2,2) );

	CVector3LG v2(  a.getValue(1,0)*b.getValue(0,0)
				+ a.getValue(1,1)*b.getValue(1,0)
				+ a.getValue(1,2)*b.getValue(2,0),
				  a.getValue(1,0)*b.getValue(0,1)
				+ a.getValue(1,1)*b.getValue(1,1)
				+ a.getValue(1,2)*b.getValue(2,1),
				  a.getValue(1,0)*b.getValue(0,2)
				+ a.getValue(1,1)*b.getValue(1,2)
				+ a.getValue(1,2)*b.getValue(2,2) );

	CVector3LG v3(  a.getValue(2,0)*b.getValue(0,0)
				+ a.getValue(2,1)*b.getValue(1,0)
				+ a.getValue(2,2)*b.getValue(2,0),
				  a.getValue(2,0)*b.getValue(0,1)
				+ a.getValue(2,1)*b.getValue(1,1)
				+ a.getValue(2,2)*b.getValue(2,1),
				  a.getValue(2,0)*b.getValue(0,2)
				+ a.getValue(2,1)*b.getValue(1,2)
				+ a.getValue(2,2)*b.getValue(2,2) );

    return CMatrix3x3LG( v1, v2, v3 );
}

