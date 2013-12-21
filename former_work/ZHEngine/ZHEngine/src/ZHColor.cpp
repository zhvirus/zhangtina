#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHColor.h"




namespace ZH
{

	Color::Color(){R=0;G=0;B=0;alpha=0;}
	
	Color::Color(ZHfloat r,ZHfloat g,ZHfloat b,ZHfloat inAlpha)
	{	
	
		R=r;
		G=g;
		B=b;
		alpha=inAlpha;

		adjustTo01();


	}


	Color::Color(Color& inColor)
	{
		memcpy(this,&inColor,sizeof(Color));
		
		adjustTo01();
	}
	
	Color::~Color(){}
	
	void Color::operator =(Color inColor)
	{
		memcpy(this,&inColor,sizeof(Color));
		
		adjustTo01();
	}


	bool Color::operator==(Color inColor)
	{
		if(memcmp(this,&inColor,sizeof(Color))==0)
			return true;
		else
			return false;
	}


	void Color::adjustTo01()
	{
		if(R<0.0f) R=0.0f;
		if(R>1.0f) R=1.0f;

		if(G<0.0f) G=0.0f;
		if(G>1.0f) G=1.0f;
		
		if(B<0.0f) B=0.0f;
		if(B>1.0f) B=1.0f;
		
		if(alpha<0.0f) alpha=0.0f;
		if(alpha>1.0f) alpha=1.0f;
	}


	Color Color::operator+(Color& inColor)
	{
		return Color(R+inColor.R,
						G+inColor.G,
						B+inColor.B,
						alpha+inColor.alpha
						);

	}


	Color Color::operator-(Color& inColor)
	{
		return Color(R-inColor.R,
						G-inColor.G,
						B-inColor.B,
						alpha-inColor.alpha
						);

	}


	Color Color::operator*(Color& inColor)
	{
		return Color(R*inColor.R,
						G*inColor.G,
						B*inColor.B,
						alpha*inColor.alpha
						);

	}

	Color Color::operator/(Color& inColor)
	{

		return Color((inColor.R!=0.0f)?(R/inColor.R):1.0f,
						(inColor.G!=0.0f)?(G/inColor.G):1.0f,
						(inColor.B!=0.0f)?(B/inColor.B):1.0f,
						(inColor.alpha!=0.0f)?(alpha/inColor.alpha):1.0f
						);

	}

	Color Color::operator*(ZHfloat& a)
	{
		return Color(a*R,a*G,a*B,a*alpha);
	}

	Color Color::operator/(ZHfloat&  b)
	{
		if (b==0.0f) return Color(1.0f,1.0f,1.0f,1.0f);
		return Color(R/b,G/b,B/b,alpha/b);
	}

	//返回颜色值，比如0xffee3311之类
	ZHint Color::getValue()
	{
		adjustTo01();
		ZHint value = static_cast<ZHint>(
											255*(
													R*pow(2.0f,15)+
													G*pow(2.0f,7)+
													B+
													alpha*pow(2.0f,23)
													)
													
													);
		
		return value;
	} 










const D3DXCOLOR Color::WHITE(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
const D3DXCOLOR Color::BLACK(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
const D3DXCOLOR Color::RED(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
const D3DXCOLOR Color::GREEN(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f));
const D3DXCOLOR Color::BLUE(D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));
const D3DXCOLOR Color::YELLOW(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));
const D3DXCOLOR Color::GREY(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));



}