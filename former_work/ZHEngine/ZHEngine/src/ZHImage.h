#pragma once

#include "ZHRenderDecl.h"


namespace ZH
{

class ZH_DLL Image
{


public:
	Image();
	~Image();

private:
	ULONG_PTR gdiToken;
	Gdiplus::GdiplusStartupInput gdiInput;

private:
	bool _read8bitImage(Gdiplus::Bitmap* image,int**  heightDataOut,const int& imageWidth,const int&imageHeight);
	bool _read24bitImage(Gdiplus::Bitmap* image,int**  heightDataOut,const int& imageWidth,const int&imageHeight);
	int GetEncoderClsid(WCHAR* format, CLSID* pClsid);
public:
    //interfaces
    bool getHeightData(std::string fileName,int** heightDataOut,int& imageWidth,int& imageHeight);
    bool getSize(std::string fileName,int& w,int& h);
	bool saveToFile(wchar_t* fileName, int* data,int width,int height);

};











}