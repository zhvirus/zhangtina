#pragma once

#include "ZHRenderDecl.h"


namespace ZH
{


class Font
{
public:
	Font();
	~Font();


void InitFont(LPDIRECT3DDEVICE9 device);

void Render(LPCTSTR str,const ZHRect& rect,DWORD color=0xffffffff,DWORD format=DT_LEFT|DT_TOP);
//void Render(LPCTSTR str,const ZHRect& rect,Color& color ,DWORD format=DT_LEFT|DT_TOP);
void Render(LPCTSTR str,const ZHRect& rect,D3DXCOLOR& color ,DWORD format=DT_LEFT|DT_TOP);

private:
	D3DXFONT_DESC fontDesc;
	LPD3DXFONT lpFont;


};


}