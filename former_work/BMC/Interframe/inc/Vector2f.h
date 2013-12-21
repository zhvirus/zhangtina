#include <math.h>
#include "qmath.h"
#include <assert.h>

#ifndef _VECTOR_2F_YY_
#define _VECTOR_2F_YY_

#define EPS  1e-7
#define PI   3.1415926f
#define MAXR 1e20
#define	EQUAL(a,b)	((a)>((b)-EPS) && (b)>((a)-EPS))

class CVector2f
{
public:
	union	{
		struct	{	float	data[2];};
		struct	{	float	x, y;	};
		struct	{	float	u, v;	};
	};

	// constructor
	CVector2f ()	// normal constructor
	{	x = 0; y = 0;	}
	
	CVector2f (float xx, float yy)	// normal constructor
	{	x = xx; y = yy;}
	
	// destructor
	CVector2f::~CVector2f(void){}

	const float& at(const int id) const
	{
		assert (id >=0 && id < 2);
		return data[id];
	}
	
	float& at(const int id)
	{
		assert (id >=0 && id < 2);
		return data[id];
	}

	const float& operator [] (const int id) const
	{
		assert (id >=0 && id < 2);
		return data[id];
	}
	float& operator [] (const int id)
	{
		assert (id >=0 && id < 2);
		return data[id];
	}

	const BOOL operator == (const CVector2f &v) const
	{
		return ((this->x == v.x) && (this->y == v.y));
	}

	// vectors plus --------------------------------------------------------------
	const CVector2f operator + (const CVector2f& v) const
	{
		CVector2f r;
		
		r.x = this->x + v.x;
		r.y = this->y + v.y;
		
		return	( r );
	}
	void operator += (const CVector2f &v)
	{
		this->x += v.x;
		this->y += v.y;
	}
	
	// vectors subtraction -------------------------------------------------------
	const CVector2f operator - (const CVector2f& v) const
	{
		CVector2f r;
		
		r.x = this->x - v.x;
		r.y = this->y - v.y;
		
		return	( r );
	}
	void operator -= (const CVector2f &v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}
	
	// vector scale --------------------------------------------------------------
	CVector2f operator * (const CVector2f& v) const
	{
		return CVector2f (x * v.x, y * v.y);
	}
	void operator *= (const CVector2f& v)
	{
		x *= v.x;
		y *= v.y;
	}
	const CVector2f operator * (float k) const
	{
		return CVector2f (x * k, y * k);
	}
	const CVector2f operator * (int k) const
	{
		return CVector2f (x * k, y * k);
	}
	void operator *= (const float k)
	{
		this->x *= k;
		this->y *= k;
	}

	// vector division -----------------------------------------------------------
	CVector2f operator / (const CVector2f &v) const
	{
		return CVector2f (x / v.x, y / v.y);
	}
	void operator /= (const CVector2f &v)
	{
		x /= v.x;
		y /= v.y;
	}

	CVector2f operator / (const float k) const
	{
		float fTemp = 1.f/k;
		return CVector2f (x * fTemp, y * fTemp);
	}
	void operator /= (float k)
	{
		float fTemp = 1.f/k;
		x *= fTemp;
		y *= fTemp;
	}

	// vectors dot product -------------------------------------------------------
	const float Dot (const CVector2f & v ) const
	{
		return (this->x * v.x + this->y * v.y);
	}

	// vector length -------------------------------------------------------------
	const float Length() const
	{
		return ((float) qmathSqrt(x*x + y*y));
	}
	
	// vector unify --------------------------------------------------------------
	void Normalize()
	{
		float len = Length();
		if (len < 1.0e-10f)
		{
			x = y = 0.f;
		}
		else
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
		}
	}

	const CVector2f Normalized() const 
	{
		CVector2f rsl = *this;
		rsl.Normalize();
		return rsl;
	}

	// Inverse vector ---------------------------------------------------------
	CVector2f Inversed() const
	{
		return CVector2f (-x, -y);
	}
	void Inverse()
	{
		x = -x, y = -y;
	}

	CVector2f Mirror_Vector (const CVector2f& Nml) const
	{
		CVector2f vIn(x, y), tempv, rsl;

		tempv = Nml.Normalized();
		tempv = tempv * (vIn.Dot(tempv) * 2.f);
		return tempv - vIn;
	}

	void VecExchange (CVector2f &v)
	{
		float ff;
		ff = x; x = v.x; v.x = ff;
		ff = y; y = v.y; v.y = ff;
	}
};

inline CVector2f operator * (float f, CVector2f v)	{return v * f;}
#endif