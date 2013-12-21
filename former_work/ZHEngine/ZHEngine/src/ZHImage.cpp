#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHImage.h"
using namespace Gdiplus;

namespace ZH
{


Image::Image()
{
	GdiplusStartup(&gdiToken,&gdiInput,NULL);
}


Image::~Image()
{
	GdiplusShutdown(gdiToken);
}





//interfaces

bool Image::getHeightData(std::string fileName,int** heightDataOut,int& imageWidth,int& imageHeight)
{
	const char*	temp	=	fileName.c_str();
	//change TCHAR	 to WCHAR 
	WCHAR name[MAX_PATH];
	
	int ni=0;
	for(;(fileName[ni]!='\0')&&(ni<MAX_PATH);ni++)
		name[ni]	=	temp[ni];
	name[ni]	=	'\0';


	Bitmap tempBitmap(name);

	int w		=	tempBitmap.GetWidth();
	int h		=	tempBitmap.GetHeight();
	
	if(w<=1||h<=1)
		return false;

	imageWidth	=	w;
	imageHeight	=	h;

	PixelFormat format	=	tempBitmap.GetPixelFormat();
	
	switch	(format)
	{
	case	PixelFormat8bppIndexed :
			{
			if(!_read8bitImage(&tempBitmap,heightDataOut,w,h))
				return false;
			break;
			}
	case	PixelFormat24bppRGB:
			{
			if(!_read24bitImage(&tempBitmap,heightDataOut,w,h))
				return false;
			break;
			}
	default:
		return false;
		break;
	}




	return true;
}


//load image data from 8 bit image
bool Image::_read8bitImage(Bitmap* image,int**  heightDataOut,const int& imageWidth,const int&imageHeight)
{

int w	=	imageWidth;
int h	=	imageHeight;

if(w<1||h<1)	return false;

if (image->GetPixelFormat()!=PixelFormat8bppIndexed)	return false;


BitmapData *data = new BitmapData();
if(!data) return false;

	image->LockBits(new Rect(0,0,w,h),ImageLockModeRead,PixelFormat8bppIndexed,data);
	int stride = data->Stride;
	BYTE *point = (BYTE*)data->Scan0;

	try{
		*heightDataOut	=	new int[imageWidth*imageHeight];
	}
	catch(std::bad_alloc)
	{
		std::cout<<"Image::_read8bitImage()-->memory alloc failed!"<<std::endl;
		return false;
	}

	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
		{	
			int index	=	i*stride+j;
			(*heightDataOut)[index]	=	(*(point+index));

		}



		image->UnlockBits(data);
		SAFE_DELETE(data);
	return true;

}



//load image data from 24 bit image
bool Image::_read24bitImage(Bitmap* image,int**  heightDataOut,const int& imageWidth,const int&imageHeight)
{

int w	=	imageWidth;
int h	=	imageHeight;

if(w<1||h<1)	return false;

if (image->GetPixelFormat()!=PixelFormat24bppRGB)	return false;


BitmapData *data = new BitmapData();
if(!data) return false;

	image->LockBits(new Rect(0,0,w,h),ImageLockModeRead,PixelFormat24bppRGB,data);
	int stride = data->Stride;
	BYTE *point = (BYTE*)data->Scan0;

	

	try{
		*heightDataOut	=	new int[imageWidth*imageHeight];
	}
	catch(std::bad_alloc)
	{
		std::cout<<"Image::_read24bitImage()-->memory alloc failed!"<<std::endl;
		return false;
	}

	
	int t=0;
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
		{	
			int index	=	i*stride+j*3;
			(*heightDataOut)[t++]	=	static_cast<int>((*(point+index)+*(point+index+1)+*(point+index+2))/3.0f);

		}



		image->UnlockBits(data);
		SAFE_DELETE(data);
return true;

}


//get image size
bool Image::getSize(std::string fileName,int& w,int& h)
{
	const char* temp	=	fileName.c_str();
	//change TCHAR	 to WCHAR 
	WCHAR name[MAX_PATH];
	
	int ni=0;
	for(;(fileName[ni]!='\0')&&(ni<MAX_PATH);ni++)
		name[ni]	=	temp[ni];
	name[ni]	=	'\0';



	Bitmap tempBitmap(name);

	w	=	tempBitmap.GetWidth();
	h	=	tempBitmap.GetHeight();

	if(w<1||h<1)
		return false;
	
	return true;
	

}

bool Image::saveToFile(wchar_t* fileName, int* data,int width,int height)
{
	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
			*(point+i*stride+j*3)	=	*(data+i*width+j);
			*(point+i*stride+j*3+1)	=	*(data+i*width+j);
			*(point+i*stride+j*3+2)	=	*(data+i*width+j);
		}

	_map.UnlockBits(b_data);

	CLSID bmpClsid;
	GetEncoderClsid(L"image/bmp", &bmpClsid);

	_map.Save(fileName,&bmpClsid,NULL);    

	return true;
}


//获取图片编码格式////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Image::GetEncoderClsid(WCHAR* format, CLSID* pClsid)
{  
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure

}








}
