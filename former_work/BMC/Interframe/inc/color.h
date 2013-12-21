/*
 * Internet Graphics Group, MSR Asia
 * Microsoft Confidential.  All Rights Reserved.             

 * --------------------------------------------------------------------
 * Permission to use, copy, or modify this software and its documentation
 * for educational and research purposes only and without fee is hereby
 * granted, provided that this copyright notice appear on all copies and
 * supporting documentation.  For any other uses of this software, in
 * original or modified form, including but not limited to distribution
 * in whole or in part, specific prior permission must be obtained from
 * Microsoft research and the authors.  These programs shall not be used,
 * rewritten, or adapted as the basis of a commercial software or hardware
 * product without first obtaining appropriate licenses from MSRCN.  MSRCN
 * makes no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 * --------------------------------------------------------------------
 * 
 * Sept. 2001
 */

//////////////////////////////////////////////////////////////////////////////////////////
//	CColor  : Color 3d class for Computer Graphics 

//	Sep.2001	Base Framework Provided by Chen Yanyun

//	Aug.2002	Modified by Zhang Zhunping		i-zhzh@microsoft.com
//				derivative class from CVector3f
//		Added : 
//
//


// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MY_COLOR_CLASS__
#define __MY_COLOR_CLASS__

#include <math.h>
//#include "Vector3f.h"
#include "vtl/vtl_matrix.h"
#include <vector>


#define _R    0
#define _G    1
#define _B    2

/////////////////////////////////////////////////////////////////////////////////////
//
template<class T>
	class CColor4 : public vtl::Vector<T, vtl::VectorTraitsStatic<4, T> > 
{
	typedef vtl::Vector<T, vtl::VectorTraitsStatic<4, T> >  ParentType;
public:
    CONSTRUCTOR_VECTOR_DEFAULT(CColor4, T)
    CONSTRUCTOR_FROM_PARENT(CColor4, ParentType)

    T& r() { return at(0);}
    T& g() { return at(1);}
    T& b() { return at(2);}
    T& a() { return at(3);}

    const T& r() const { return at(0);}
    const T& g() const { return at(1);}
    const T& b() const { return at(2);}
    const T& a() const { return at(3);}

public:
    CColor4(T _r, T _g, T _b)
    {
        r() = _r;
        g() = _g;
        b() = _b;
        a() = 1.0f;
    }

    CColor4(float _r, float _g, float _b, float _a)
    {
        r() = _r;
        g() = _g;
        b() = _b;
        a() = _a;
    }

//    CColor4(T _rgba)    
//    {
//        r() = g() = b() = a() = _rgba;
//    }

/////////////////////////////////////////////////////////////////////////
// TODO : move here useful functions from Original CColor (see below) 

	void Unify()													{
        Clamp (CColor4f(0.0f), CColor4f(1.0f));
    }
    
   	int	R()			{
		int ret = int(r() * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
	int	G()			{
		int ret = int(g() * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
	int	B()			{
		int ret = int(b() * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
	int	A()			{
		int ret = int(a() * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}

    // max( r(), max( g(), b())) 
    float   Get_Max_RGB_Value   ()  const
    {
        return  max( r(), max( g(), b()));
    }

    // r() + g() + b()
    float   Get_Sum_RGB_Value   ()  const
    {
        return  r() + g() + b();
    }


	// ----------------------------------------------------------------------------------------------------
	// RGB space and HSV space
	// H = Hue : A unit that specifies the "colour" of the colour 
	// S = Saturation : Specifies how much black, white or gray is mixed with the colour. 
	// V = Value : Corresponds to Brightness. 
	void RGB_2_HSV()
	{
		float h, s, v;
		float min, max, delta;

		min = (r() < g()) ? r() : g();
		min = (min < b()) ? min : b();

		max = (r() > g()) ? r() : g();
		max = (max > b()) ? max : b();;

		v = max;				// v
		delta = max - min;
		if( max != 0 )
			s = delta / max;		// s
		else
		{
			// r() = g() = b() = 0		// s = 0, v is undefined
			s = 0;
			h = -1;
			return;
		}

		if( r() == max )
			h = ( g() - b() ) / delta;		// between yellow & magenta
		else if( g() == max )
			h = 2 + ( b() - r() ) / delta;	// between cyan & yellow
		else
			h = 4 + ( r() - g() ) / delta;	// between magenta & cyan

		h *= 60;				// degrees
		if( h < 0 )
			h += 360;

		r() = h;
		g() = s;
		b() = v;
	}

	void HSVtoRGB()
	{
		float h = r();
		float s = g();
		float v = b();

		int i;
		float f, p, q, t;
		if( s == 0 )
		{
			// achromatic (grey)
			r() = g() = b() = v;
			return;
		}
		h /= 60;			// sector 0 to 5
		
		//i = floor( h );
		if (h >= 0)
			i = (int) h;
		else	i = (int) h -1;

		f = h - i;			// factorial part of h
		p = v * ( 1 - s );
		q = v * ( 1 - s * f );
		t = v * ( 1 - s * ( 1 - f ) );
		switch( i ) {
		case 0:
			r() = v;
			g() = t;
			b() = p;
			break;
		case 1:
			r() = q;
			g() = v;
			b() = p;
			break;
		case 2:
			r() = p;
			g() = v;
			b() = t;
			break;
		case 3:
			r() = p;
			g() = q;
			b() = v;
			break;
		case 4:
			r() = t;
			g() = p;
			b() = v;
			break;
		default:		// case 5:
			r() = v;
			g() = p;
			b() = q;
			break;
		}
	}

};

typedef CColor4<float>  CColor4f;

/////////////////////////////////////////////////////////////////////////////////////

#define	COLOR_BLACK		(CColor4f(0.0f,0.0f,0.0f))
#define	COLOR_WHITE		(CColor4f(1.0f,1.0f,1.0f))
#define	COLOR_RED		(CColor4f(1.0f,0.0f,0.0f))
#define	COLOR_GREEN		(CColor4f(0.0f,1.0f,0.0f))
#define	COLOR_BLUE		(CColor4f(0.0f,0.0f,1.0f))


///////////////////////////////////////////////////////////////////////////////
//		TColor3b
// Structure of byte rgb color. Using bytes datatype so that the raw data size
// can be reduced
///////////////////////////////////////////////////////////////////////////////
struct TColor3b
{
	BYTE r, g, b;

	TColor3b  *Byte_color_ptr ()
	{
		return this;
	}
	CColor4f Float_color (void)
	{
		return CColor4f (r/255.f, g/255.f, b/255.f, 1);
	}
	COLORREF Get_24bit_color(void)
	{
		return (((int)r)<<16) | (((int)g)<<8) | ((int)b);
	}
};


/*
////////////////////////////////////////////////////////////////////////////////
class CColor																	{
private:
	float _unify(float f)		{
		if (f < 0.f) return 0.f;
		if (f > 1.f) return 1.f;
		return f;
	}

public:
	float	r, g, b, a;


// construction/distruction
	CColor()	{	r = g = b = 0.0f;	a = 1.f;	}

	CColor(int nr, int ng, int nb)									{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		_a_unify ();
		a = 1.f;
	}

	CColor(int nr, int ng, int nb, int na)							{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		a = (float)(na * 0.003921568627450980392156862745098);
		_a_unify ();
	}

	CColor(unsigned int nr, unsigned int ng, unsigned int nb)		{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		_a_unify ();
		a = 1.f;
	}

	CColor(unsigned int nr, unsigned int ng, unsigned int nb, unsigned int na)	{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		a = (float)(na * 0.003921568627450980392156862745098);
		_a_unify ();
	}

	CColor(char nr, char ng, char nb)											{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		_a_unify ();
		a = 1.f;
	}

	CColor(char nr, char ng, char nb, char na)						{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		a = (float)(na * 0.003921568627450980392156862745098);
		_a_unify ();
	}

	CColor(unsigned char nr, unsigned char ng, unsigned char nb)	{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		_a_unify ();
		a = 1.f;
	}

	CColor(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)	{
		r = (float)(nr * 0.003921568627450980392156862745098);
		g = (float)(ng * 0.003921568627450980392156862745098);
		b = (float)(nb * 0.003921568627450980392156862745098);
		a = (float)(na * 0.003921568627450980392156862745098);
		_a_unify ();
	}

	CColor(float fr, float fg, float fb)							{
		r = fr;
		g = fg;
		b = fb;
		_a_unify ();
		a = 1.f;
	}

	CColor(float fr, float fg, float fb, float fa)					{
		r = fr;
		g = fg;
		b = fb;
		a = fa;
		_a_unify ();
	}

	CColor(double fr, double fg, double fb)							{
		r = (float)fr;
		g = (float)fg;
		b = (float)fb;
		_a_unify ();
		a = 1.f;
	}

	CColor(double fr, double fg, double fb, double fa)				{
		r = ((float)fr);
		g = ((float)fg);
		b = ((float)fb);
		a = ((float)fa);
		_a_unify ();
	}

	virtual ~CColor()	{	}

	int		Stream_into_opened_binary_file	(FILE* f);
	int		Stream_from_opened_binary_file	(FILE* f);

// operators
//	void operator =(const CColor& other)							{
//		r = other.r;
//		g = other.g;
//		b = other.b;
//		a = other.a;
//	}

	CColor operator +(const CColor& other)							{
		CColor rslColor;
		rslColor.r = (r + other.r);
		rslColor.g = (g + other.g);
		rslColor.b = (b + other.b);
		rslColor.a = (a + other.a);
		rslColor._a_unify ();
		return (rslColor);
	}

	void operator +=(const CColor& other)							{
		r = (other.r + r);
		g = (other.g + g);
		b = (other.b + b);
		a = (other.a + a);
		_a_unify ();
	}

	CColor operator -(const CColor& other)							{
		CColor rslColor;
		rslColor.r = (r - other.r);
		rslColor.g = (g - other.g);
		rslColor.b = (b - other.b);
		rslColor.a = a;
		rslColor._a_unify ();
		return (rslColor);
	}

	void operator -=(const CColor& other)							{
		r = (r - other.r);
		g = (g - other.g);
		b = (b - other.b);
		_a_unify ();
	}

	CColor operator *(const CColor& other)const						{
		CColor rslColor;
		rslColor.r = r * other.r;
		rslColor.g = g * other.g;
		rslColor.b = b * other.b;
		rslColor.a = a;
		return (rslColor);
	}

	void operator *=(const CColor& other)							{
		r *= other.r;
		g *= other.g;
		b *= other.b;
	}

	CColor operator *(const float scale)							{
		CColor rslColor;
		rslColor.r = (r * scale);
		rslColor.g = (g * scale);
		rslColor.b = (b * scale);
		rslColor.a = a;
		rslColor._a_unify ();
		return (rslColor);
	}

	void operator *=(const float scale)								{
		r = (r * scale);
		g = (g * scale);
		b = (b * scale);
		_a_unify ();
	}

	CColor operator /(float scale)									{
		CColor rslColor;
		if (fabs(scale) >= 1.0e-10f)		{
			scale = 1/scale;
			rslColor.r = (r * scale);
			rslColor.g = (g * scale);
			rslColor.b = (b * scale);
		}
		rslColor.a = a;
		rslColor._a_unify ();
		return (rslColor);
	}

	void operator /=(float scale)									{
		if (fabs(scale) >= 1.0e-10f)		{
			scale = 1/scale;
			r = (r * scale);
			g = (g * scale);
			b = (b * scale);
			_a_unify ();
		}
	}

	bool operator ==(const CColor & other)							{
		if ( (EQUAL(r, other.r)) && (EQUAL(g, other.g)) && (EQUAL(b, other.b)) )
				return true;
		else	return false;
	}

	bool operator !=(const CColor & other)							{
		if ( (r == other.r) && (g == other.g) && (b == other.b) )
				return false;
		else	return true;
	}

	CColor MaxRGB (CColor c1, CColor c2)							{
		if ( (c1.r + c1.g + c1.b) > (c2.r + c2.g + c2.b) )
				return c1;
		else	return c2;
	}

	CColor MaxR (CColor c1, CColor c2)								{
		if (c1.r > c2.r)
				return c1;
		else	return c2;
	}

	CColor MaxG (CColor c1, CColor c2)								{
		if (c1.g > c2.g)
				return c1;
		else	return c2;
	}

	CColor MaxB (CColor c1, CColor c2)								{
		if (c1.b > c2.b)
				return c1;
		else	return c2;
	}

	CColor MinRGB (CColor c1, CColor c2)							{
		if ( (c1.r + c1.g + c1.b) > (c2.r + c2.g + c2.b) )
				return c2;
		else	return c1;
	}

	CColor MinR (CColor c1, CColor c2)								{
		if (c1.r > c2.r)
				return c2;
		else	return c1;
	}

	CColor MinG (CColor c1, CColor c2)								{
		if (c1.g > c2.g)
				return c2;
		else	return c1;
	}

	CColor MinB (CColor c1, CColor c2)								{
		if (c1.b > c2.b)
				return c2;
		else	return c1;
	}

	CColor Blend(CColor c1, CColor c2)								{
		CColor c3;
		float fTemp = c1.a + c2.a;

		if (fTemp < 1.0e-10) return c3;
		fTemp = 1/fTemp;

		c3.r = _unify( (c1.r * c1.a + c2.r * c2.a) * fTemp );
		c3.g = _unify( (c1.g * c1.a + c2.g * c2.a) * fTemp );
		c3.b = _unify( (c1.b * c1.a + c2.b * c2.a) * fTemp );
		c3.a = 1-(1-c1.a)*(1-c2.a);
		return c3;
	}

	CColor O_O(CColor c1, CColor c2)								{
		CColor c3;
		c3.r = c1.r * c1.a + (1-c1.a) * c2.r * c2.a;
		c3.g = c1.g * c1.a + (1-c1.a) * c2.g * c2.a;
		c3.b = c1.b * c1.a + (1-c1.a) * c2.b * c2.a;
		c3.a = 1-(1-c1.a)*(1-c2.a);
		return c3;
	}

	void _a_unify	()		{
//		unify();
	}

	void unify()													{
		if (r > 1.f) r = 1.f;
		if (r < 0.f) r = 0.f;

		if (g > 1.f) g = 1.f;
		if (g < 0.f) g = 0.f;

		if (b > 1.f) b = 1.f;
		if (b < 0.f) b = 0.f;

		if (a > 1.f) a = 1.f;
		if (a < 0.f) a = 0.f;
	}

	int	R()			{
		int ret = int(r * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
	int	G()			{
		int ret = int(g * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
	int	B()			{
		int ret = int(b * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
	int	A()			{
		int ret = int(a * 255);
		if (ret < 0)	ret = 0;
		if (ret > 255)	ret = 255;
		return ret;
	}
};

/**/

#endif // !defined __MY_COLOR_CLASS__
