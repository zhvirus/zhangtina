#include <math.h>
#include "qmath.h"


#ifndef _VECTOR_3F_YY_
#define _VECTOR_3F_YY_

#define EPS  1e-7
#define PI   3.1415926f
#define MAXR 1e20
#define	EQUAL(a,b)	((a)>((b)-EPS) && (b)>((a)-EPS))

#define VECTOR_RANDOM

class CVector3f
{
public:
	union	{
		struct	{	float	data[3];	};
		struct	{	float	x, y, z;	};
		struct	{	float	n, m, l;	};
		struct	{	float	r, g, b;	};
		struct	{	float	u, v;		};

	};

	// constructor
	CVector3f ()	// normal constructor
	{	x = 0; y = 0; z = 0;	}
	
	CVector3f (float xx, float yy, float zz)	// normal constructor
	{	x = xx; y = yy; z = zz;	}

	CVector3f (BOOL bRandom)
	{
		if (bRandom)
		{
			float fTheta = (PI * rand() / RAND_MAX);
			float fPhi   = (2 * PI * rand() / RAND_MAX);
			float fTemp  = (float)qmathSin (fTheta);

			z = (float)qmathCos (fTheta);
			x = (float)qmathCos (fPhi) * fTemp;
			y = (float)qmathSin (fPhi) * fTemp;
		}
		else x = y = z = 0.f;
	}
	
	// destructor
	CVector3f::~CVector3f(void){}

	const float& at(const int id) const
	{
		return (id >=0 && id < 3) ? data[id] : data[0];
	}
	
	float& at(const int id)
	{
		return (id >=0 && id < 3) ? data[id] : data[0];
	}

	const float& operator [] (const int id) const
	{
		return (id >=0 && id < 3) ? data[id] : data[0];
	}
	float& operator [] (const int id)
	{
		return (id >=0 && id < 3) ? data[id] : data[0];
	}

	const BOOL operator == (const CVector3f &v) const
	{
		return ((this->x == v.x) && (this->y == v.y) && (this->z == v.z));
	}

	// vectors plus --------------------------------------------------------------
	const CVector3f operator + (const CVector3f& v) const
	{
		CVector3f r;
		
		r.x = this->x + v.x;
		r.y = this->y + v.y;
		r.z = this->z + v.z;
		
		return	( r );
	}
	void operator += (const CVector3f &v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}
	
	// vectors subtraction -------------------------------------------------------
	const CVector3f operator - (const CVector3f& v) const
	{
		CVector3f r;
		
		r.x = this->x - v.x;
		r.y = this->y - v.y;
		r.z = this->z - v.z;
		
		return	( r );
	}
	void operator -= (const CVector3f &v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
	}
	
	// vector scale --------------------------------------------------------------
	CVector3f operator * (const CVector3f& v) const
	{
		return CVector3f (x * v.x, y * v.y, z * v.z);
	}
	void operator *= (const CVector3f& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}
	const CVector3f operator * (float k) const
	{
		return CVector3f (x * k, y * k, z * k);
	}
	const CVector3f operator * (int k) const
	{
		return CVector3f (x * k, y * k, z * k);
	}
	void operator *= (const float k)
	{
		this->x *= k;
		this->y *= k;
		this->z *= k;
	}

	// vector division -----------------------------------------------------------
	CVector3f operator / (const CVector3f &v) const
	{
		return CVector3f (x / v.x, y / v.y, z / v.z);
	}
	void operator /= (const CVector3f &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

	CVector3f operator / (const float k) const
	{
		float fTemp = 1.f/k;
		return CVector3f (x * fTemp, y * fTemp, z * fTemp);
	}
	void operator /= (float k)
	{
		float fTemp = 1.f/k;
		x *= fTemp;
		y *= fTemp;
		z *= fTemp;
	}

	// vectors cross product -----------------------------------------------------
	CVector3f Cross (const CVector3f& v) const
	{
		return CVector3f (y*v.z - v.y*z, v.x*z - x*v.z, x*v.y - v.x*y);
	}
	
	// vectors dot product -------------------------------------------------------
	const float Dot (const CVector3f & v ) const
	{
		return (this->x * v.x + this->y * v.y + this->z * v.z );
	}

	// vector length -------------------------------------------------------------
	const float Length() const
	{
		return ((float) qmathSqrt(x*x + y*y + z*z));
	}
	
	// vector unify --------------------------------------------------------------
	void Normalize()
	{
		float len = Length();
		if (len < 1.0e-10f)
		{
			x = y = z = 0.f;
		}
		else
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
			z *= len;
		}
	}

	const CVector3f Normalized() const 
	{
		CVector3f rsl = *this;
		rsl.Normalize();
		return rsl;
	}

	// Inverse vector ---------------------------------------------------------
	CVector3f Inversed() const
	{
		return CVector3f (-x, -y, -z);
	}
	void Inverse()
	{
		x = -x, y = -y, z = -z;
	}

	CVector3f Mirror_Vector (const CVector3f& Nml) const
	{
		CVector3f vIn(x, y, z), tempv, rsl;

		tempv = Nml.Normalized();
		tempv = tempv * (vIn.Dot(tempv) * 2.f);
		return tempv - vIn;
	}

	void VecExchange (CVector3f &v)
	{
		float ff;
		ff = x; x = v.x; v.x = ff;
		ff = y; y = v.y; v.y = ff;
		ff = z; z = v.z; v.z = ff;
	}
};

inline CVector3f operator * (float f, CVector3f v)	{return v * f;}
#endif
