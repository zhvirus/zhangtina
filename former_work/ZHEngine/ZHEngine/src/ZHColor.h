#pragma once

#include "ZHRenderDecl.h"

namespace ZH
{



class ZH_DLL Color
{
public:
	Color();
	Color(Color& inColor);
	Color(ZHfloat r,ZHfloat g,ZHfloat b,ZHfloat inAlpha=1);
	~Color();
	void operator=(Color inColor);
	bool operator==(Color inColor);

	Color operator+(Color& inColor);
	Color operator-(Color& inColor);
	Color operator*(Color& inColor);
	Color operator/(Color& inColor);
	Color operator*(ZHfloat& a);
	Color operator/(ZHfloat&  b);
	
	ZHint getValue();

private:
	
	//ÏÞ¶¨ÑÕÉ«·¶Î§
	void adjustTo01();

	


public:
	ZHfloat R;
	ZHfloat G;
	ZHfloat B;
	ZHfloat alpha;
	
	

public:
static const D3DXCOLOR WHITE;
static const D3DXCOLOR BLACK;
static const D3DXCOLOR RED;
static const D3DXCOLOR GREEN;
static const D3DXCOLOR BLUE;
static const D3DXCOLOR YELLOW;
static const D3DXCOLOR GREY;

};

}