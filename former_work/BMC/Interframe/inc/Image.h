#pragma once
#include "ZHDecl.h"
#include "ZHSTD.h"
#include "Common.h"
#include "Frame.h"

namespace ZH
{

class ZH_DLL ImageUtility
{
public:
	ImageUtility();
	~ImageUtility();

private:
	ULONG_PTR gdiToken;
	Gdiplus::GdiplusStartupInput gdiInput;

private:
	bool _read8bitImage(Gdiplus::Bitmap* image,int**  DataOut,const int& imageWidth,const int&imageHeight);
	bool _read24bitImage(Gdiplus::Bitmap* image,int**  DataOut,const int& imageWidth,const int&imageHeight);
	int GetEncoderClsid(WCHAR* format, CLSID* pClsid);
public:
    //interfaces
    bool getHeightData(std::string fileName,int** DataOut,int& imageWidth,int& imageHeight);
    bool getSize(std::string fileName,int& w,int& h);

	bool saveToFileFromInt(char* fileName, int* data,int width,int height);
	bool saveToFileFromChar(char* fileName, unsigned char* data,int width,int height);
	bool saveToFileFromFloat(char* fileName, float* data,int width,int height);
	bool saveToFileFromFloat3(char* fileName, float* data,int width,int height);
    bool saveToFileFromVec3f(char* fileName, CVector3f* data,int width,int height);
	bool saveToFileFromColor4f(char* fileName, CColor4f* data,int width,int height,float scale=1.0f);

    bool saveToFileFromPixelVec(char* fileName, std::vector<ZH::reSamplePixel*>&  _colVec,int width,int height);

    void charToWchar(char* _in, wchar_t* _out);

	bool startBilateralFilter();
	bool setBilateralFilterData(
		CColor4f* _sourceImage,
		CColor4f* _resultImage,
		int _imageWidth,
		int _imageHeight,
		int _filterBoxSize,
		PixelInfo* _pixelInfo,
		float _torPixelPositionDistance,
		float _torPerceptionDistance,
		float _torNormalDistance,
		float _torDepthDistance,
		float _torMaterialDistance,
		bool _usePixelDistance	=	true,
		bool _usePerceptionDistance = true,
		bool _useNormalDistance = true,
		bool _useDepthDistance = true,
		bool _usematerialDistance =true
		);
public:
	bool CIELAB_to_RGB(CColor4f* _imageInCIELAB, CColor4f* _imageInRGB, int _width, int _height);
	bool RGB_to_CIELAB(CColor4f* _imageInRGB, CColor4f* _imageInCIELAB, int _width, int _height);
private:
	float _f_RGB2CIELab(float _inValue);
	float _clamp01(float _inValue);

private:
	BilateralFilter* m_filter;
};

}





