#ifndef _ZH_DLL
#define _ZH_DLL
#endif



#include "ZHFont.h"
#include "ZHWindow.h"
#include "ZHColor.h"

namespace ZH
{

	Font::Font()
	{
		memset(&lpFont,0,sizeof(LPD3DXFONT));
	
	}
	
	Font::~Font(){}


	

//³õÊ¼»¯×ÖÌå
void Font::InitFont(LPDIRECT3DDEVICE9 device)
{

memset(&fontDesc,0,sizeof(D3DXFONT_DESC));


if(lpFont)
	lpFont->Release();
lpFont	=	NULL;

if(device)
{

	fontDesc.CharSet	=	DEFAULT_CHARSET;
	fontDesc.Height=20;
	fontDesc.Width	=	10;
	fontDesc.Italic	=	false;
	fontDesc.Weight	=200;
	strcpy(fontDesc.FaceName,TEXT("Verdana"));

	if(D3DXCreateFontIndirect(device,&fontDesc,&lpFont)!=D3D_OK)
	{
		ZH_OUT(TEXT("Initialize font failed!\n"));
		lpFont	=	NULL;
	}
	else
		ZH_OUT(TEXT("Font Initialized  successfully!\n"));

}

}

//äÖÈ¾×Ö·û´®ÔÚÆÁÄ»ÉÏ-1
void Font::Render(LPCTSTR str,const ZHRect& rect,DWORD color,DWORD format)
{
	RECT crt;
	crt.left	=	static_cast<long>(rect.x);
	crt.top	=	static_cast<long>(rect.y);
	crt.bottom	=	static_cast<long>(rect.height);
	crt.right	=	static_cast<long>(rect.width);

if(lpFont)
{
	if(str)
	{	
		RECT _crt={0,0,0,0};
		if(!memcmp(&crt,&_crt,sizeof(_crt)))
		{	
			ZHint w=0,h=0;
			
			
			_crt.right	=	static_cast<long>(Window::getWindowSize().width);
			_crt.bottom=	static_cast<long>(Window::getWindowSize().height);

		}
		else
		{
			memcpy(&_crt,&crt,sizeof(crt));
		}

		lpFont->DrawText(NULL,str,-1,&_crt,format,color);

	}

}




}








//äÖÈ¾×Ö·û´®ÔÚÆÁÄ»ÉÏ-3
void Font::Render(LPCTSTR str,const ZHRect& rect,D3DXCOLOR& color ,DWORD format)
{

RECT crt;
	crt.left	=	static_cast<long>(rect.x);
	crt.top	=	static_cast<long>(rect.y);
	crt.bottom	=	static_cast<long>(rect.height);
	crt.right	=	static_cast<long>(rect.width);

if(lpFont)
{
	if(str)
	{	
		RECT _crt={0,0,0,0};
		if(!memcmp(&crt,&_crt,sizeof(_crt)))
		{	
			ZHint w=0,h=0;
			
			
			_crt.right	=	static_cast<long>(Window::getWindowSize().width);
			_crt.bottom=	static_cast<long>(Window::getWindowSize().height);

		}
		else
		{
			memcpy(&_crt,&crt,sizeof(crt));
		}

		lpFont->DrawText(NULL,str,-1,&_crt,format,color);

	}

}





}






}