#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "Image.h"
#include "Filter.h"

using namespace Gdiplus;



namespace ZH
{


ImageUtility::ImageUtility():
m_filter(NULL)
{
	GdiplusStartup(&gdiToken,&gdiInput,NULL);


}


ImageUtility::~ImageUtility()
{
	GdiplusShutdown(gdiToken);
	SAFE_DELETE(m_filter);

}


//interfaces

bool ImageUtility::getHeightData(std::string fileName,int** DataOut,int& imageWidth,int& imageHeight)
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
			if(!_read8bitImage(&tempBitmap,DataOut,w,h))
				return false;
			break;
			}
	case	PixelFormat24bppRGB:
			{
			if(!_read24bitImage(&tempBitmap,DataOut,w,h))
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
bool ImageUtility::_read8bitImage(Bitmap* image,int**  DataOut,const int& imageWidth,const int&imageHeight)
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
		*DataOut	=	new int[imageWidth*imageHeight];
	}
	catch(std::bad_alloc)
	{
		std::cout<<"ImageUtility::_read8bitImage()-->memory alloc failed!"<<std::endl;
		return false;
	}

	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
		{	
			int index	=	i*stride+j;
			(*DataOut)[index]	=	(*(point+index));

		}



		image->UnlockBits(data);
		MY_DELETE(data);
	return true;

}



//load image data from 24 bit image
bool ImageUtility::_read24bitImage(Bitmap* image,int**  DataOut,const int& imageWidth,const int&imageHeight)
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
		*DataOut	=	new int[imageWidth*imageHeight];
	}
	catch(std::bad_alloc)
	{
		std::cout<<"ImageUtility::_read24bitImage()-->memory alloc failed!"<<std::endl;
		return false;
	}

	
	int t=0;
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
		{	
			int index	=	i*stride+j*3;
			(*DataOut)[t++]	=	static_cast<int>((*(point+index)+*(point+index+1)+*(point+index+2))/3.0f);

		}



		image->UnlockBits(data);
		MY_DELETE(data);
return true;

}


//get image size
bool ImageUtility::getSize(std::string fileName,int& w,int& h)
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

bool ImageUtility::saveToFileFromInt(char* fileName, int* data,int width,int height)
{
	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

    int t_color =  0;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
            if(*(data+i*width+j)>255)
                t_color  =  255;
            else if(*(data+i*width+j)<0)
                t_color  =  0;
            else
                t_color  =  *(data+i*width+j);

			*(point+i*stride+j*3)	    =	t_color;
			*(point+i*stride+j*3+1)	=	t_color;
			*(point+i*stride+j*3+2)	=	t_color;
		}

	_map.UnlockBits(b_data);

	CLSID bmpClsid;
	GetEncoderClsid(L"image/bmp", &bmpClsid);

        wchar_t t_fileName[MAX_PATH];
        charToWchar(fileName,t_fileName);

        _map.Save(t_fileName,&bmpClsid,NULL);     

	delete b_data;
	return true;
}


bool ImageUtility::saveToFileFromFloat(char* fileName, float* data,int width,int height)
{
	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

    int t_color;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
            if(*(data+i*width+j)>1.0f)
                t_color  =  255;
            else if(*(data+i*width+j)<0.0f)
            t_color  =  0;
            else
                t_color  =  static_cast<int>(*(data+i*width+j)*255);

			*(point+i*stride+j*3)	    =	t_color;
			*(point+i*stride+j*3+1)	=	t_color;
			*(point+i*stride+j*3+2)	=	t_color;
		}

	_map.UnlockBits(b_data);

	CLSID bmpClsid;
	GetEncoderClsid(L"image/bmp", &bmpClsid);

        wchar_t t_fileName[MAX_PATH];
        charToWchar(fileName,t_fileName);

        _map.Save(t_fileName,&bmpClsid,NULL);     

	MY_DELETE(b_data);
	return true;
}


bool ImageUtility::saveToFileFromFloat3(char* fileName, float* data,int width,int height)
{
    Bitmap _map(width,height,PixelFormat24bppRGB);

    BitmapData* b_data	=	new BitmapData;
    _map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
    int stride = b_data->Stride;
    BYTE *point = (BYTE*)b_data->Scan0;

    int t_color;
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            //R
            if(*(data+i*width*3+j*3)>1.0f)
                t_color  =  255;
            else if(*(data+i*width*3+j*3)<0.0f)
                t_color  =  0;
            else
                t_color  =  static_cast<int>(*(data+i*width*3+j*3)*255);

            *(point+i*stride+j*3+2)	    =	t_color;

            //G
            if(*(data+i*width*3+j*3+1)>1.0f)
                t_color  =  255;
            else if(*(data+i*width*3+j*3+1)<0.0f)
                t_color  =  0;
            else
                t_color  =  static_cast<int>(*(data+i*width*3+j*3+1)*255);

            *(point+i*stride+j*3+1)	=	t_color;

            //B
            if(*(data+i*width*3+j*3+2)>1.0f)
                t_color  =  255;
            else if(*(data+i*width*3+j*3+2)<0.0f)
                t_color  =  0;
            else
                t_color  =  static_cast<int>(*(data+i*width*3+j*3+2)*255);

            *(point+i*stride+j*3)	=	t_color;

        }

        _map.UnlockBits(b_data);

        CLSID bmpClsid;
        GetEncoderClsid(L"image/bmp", &bmpClsid);

        wchar_t t_fileName[MAX_PATH];
        charToWchar(fileName,t_fileName);

        _map.Save(t_fileName,&bmpClsid,NULL);     

        MY_DELETE(b_data);
        return true;
}

bool ImageUtility::saveToFileFromVec3f(char* fileName, CVector3f* data,int width,int height)
{
	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

    int t_color;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
            if( (*(data+i*width+j)).z >1.0f)
                t_color  =  255;
            else if((*(data+i*width+j)).z<0.0f)
                t_color  =  0;
            else
                t_color  =  static_cast<int>((*(data+i*width+j)).z*255);

			*(point+i*stride+j*3)	=	t_color;

            if( (*(data+i*width+j)).y >1.0f)
                t_color  =  255;
            else if((*(data+i*width+j)).y<0.0f)
                t_color  =  0;
            else
                t_color  =  static_cast<int>((*(data+i*width+j)).y*255);

			*(point+i*stride+j*3+1)	=	t_color;

            if( (*(data+i*width+j)).x >1.0f)
                t_color  =  255;
            else if((*(data+i*width+j)).x<0.0f)
                t_color  =  0;
            else
                t_color  =  static_cast<int>((*(data+i*width+j)).x*255);

			*(point+i*stride+j*3+2)	=	t_color;
		}

	_map.UnlockBits(b_data);

	CLSID bmpClsid;
	GetEncoderClsid(L"image/bmp", &bmpClsid);

    wchar_t t_fileName[MAX_PATH];
    charToWchar(fileName,t_fileName);

    _map.Save(t_fileName,&bmpClsid,NULL);      
	
	delete b_data;
	return true;

}

bool ImageUtility::saveToFileFromChar(char* fileName, unsigned char* data,int width,int height)
{

	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

    int t_color;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
            if(*(data+i*width*3+j*3)>255)
                t_color  =  255;
            else if(*(data+i*width*3+j*3)<0)
                t_color  = 0;
            else
                t_color  =  *(data+i*width*3+j*3);

			*(point+i*stride+j*3)	=	t_color;

            if(*(data+i*width*3+j*3+1)>255)
                t_color  =  255;
            else if(*(data+i*width*3+j*3+1)<0)
                t_color  = 0;
            else
                t_color  =  *(data+i*width*3+j*3+1);

			*(point+i*stride+j*3+1)	=	t_color;

            if(*(data+i*width*3+j*3+2)>255)
                t_color  =  255;
            else if(*(data+i*width*3+j*3+2)<0)
                t_color  = 0;
            else
                t_color  =  *(data+i*width*3+j*3+2);

			*(point+i*stride+j*3+2)	=	t_color;
		}

		_map.UnlockBits(b_data);

		CLSID bmpClsid;
		GetEncoderClsid(L"image/bmp", &bmpClsid);

        wchar_t t_fileName[MAX_PATH];
        charToWchar(fileName,t_fileName);

        _map.Save(t_fileName,&bmpClsid,NULL);    

		delete b_data;
		return true;

}

bool ImageUtility::saveToFileFromColor4f(char* fileName, CColor4f* data,int width,int height,float scale)
{
	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

	float temp;

	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
			temp	=	(data+i*width+j)->b()*255*scale;
			if(temp>255)
				*(point+i*stride+j*3)	=	255;
			else if(temp<0)
				*(point+i*stride+j*3)	=	0;
			else
				*(point+i*stride+j*3)	=	static_cast<BYTE>(temp);

			
			temp	=	(data+i*width+j)->g()*255*scale;
			if(temp>255)
				*(point+i*stride+j*3+1)=255;
			else if(temp<0)
				*(point+i*stride+j*3+1)=0;
			else
				*(point+i*stride+j*3+1)	=	static_cast<BYTE>(temp);
			
			temp	=	(data+i*width+j)->r()*255*scale;
			if(temp>255)
				*(point+i*stride+j*3+2)=255;
			else if(temp<0)
				*(point+i*stride+j*3+2)=0;
			else
				*(point+i*stride+j*3+2)	=	static_cast<BYTE>(temp);
		}

		_map.UnlockBits(b_data);

		CLSID bmpClsid;
		GetEncoderClsid(L"image/bmp", &bmpClsid);

        wchar_t t_fileName[MAX_PATH];
        charToWchar(fileName,t_fileName);

		_map.Save(t_fileName,&bmpClsid,NULL);    

		delete b_data;
		return true;

}


//获取图片编码格式////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ImageUtility::GetEncoderClsid(WCHAR* format, CLSID* pClsid)
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

float ImageUtility::_clamp01(float _inValue)
{
	if(_inValue<0.0f)
		return 0.0f;
	else	if(_inValue>1.0f)
		return 1.0f;
	else
		return _inValue;

}

bool ImageUtility::CIELAB_to_RGB(CColor4f* _imageInCIELAB, CColor4f* _imageInRGB, int _width, int _height)
{
	if(_imageInCIELAB==NULL||_imageInRGB==NULL)
		return false;

	float L,a,b,X,Y,Z,R,G,B,fx,fy,fz,delta,deltaM3P2;
	int total	=	_width*_height;
	
	delta	=	0.206897f;// 6/29
	deltaM3P2	=	3.0f * delta *delta;
	
	for(int i= 0;i<total;i++)
	{
		L = _imageInCIELAB[i].r();
		a = _imageInCIELAB[i].g();
		b = _imageInCIELAB[i].b();

		fy = (L+16.0f)/116.0f;
		fx =	fy	+ a/500.0f;
		fz	=	fy	-	b/200.0f;	


		if(fy>delta)
		{
			Y	=	pow(fy,3.0f);
		}
		else
		{
			Y	=	(fy - 0.137931f) * deltaM3P2;
		}

		if(fx>delta)
		{
			X	=	pow(fx,3.0f) * 0.950456f;
		}
		else
		{
			X	=	(fx - 0.137931f) * deltaM3P2 * 0.950456f;
		}

		if(fz>delta)
		{
			Z	=	pow(fz, 3.0f) * 1.088754f;
		}
		else
		{
			Z	=	(fz	 - 0.137931f) * deltaM3P2 * 1.088754f;
		}

		R	=	3.240479f * X	- 1.537150f * Y		- 0.498535f * Z;
		G	=	-0.969256f * X	+ 1.875992f * Y		+	0.041556f * Z;
		B  = 0.055648f * X	- 0.204043f * Y		+1.057311f * Z;
		
		R	=	_clamp01(R);
		G	=	_clamp01(G);
		B  =	_clamp01(B);
		
		_imageInRGB[i].r()	=	R;
		_imageInRGB[i].g()	=	G;
		_imageInRGB[i].b()	=	B;
	}


	return true;
}

float ImageUtility::_f_RGB2CIELab(float _inValue)
{
	if(_inValue>0.008856f)
		return pow(_inValue, 0.333333f);
	else
		return 7.787f * _inValue +16.0f/116.0f;

}


bool ImageUtility::RGB_to_CIELAB(CColor4f* _imageInRGB, CColor4f* _imageInCIELAB, int _width, int _height)
{
	if(_imageInRGB==NULL||_imageInCIELAB==NULL)
		return false;

	float L,a,b,X,Y,Z,R,G,B;
	int total	=	_width*_height;

	for(int i= 0;i<total;i++)
	{
		R	=	_imageInRGB[i].r();
		G	=	_imageInRGB[i].g();
		B  =  _imageInRGB[i].b();

		X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
		Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
		Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;

		X	/=	0.950456f;
		Z	/=	1.088754f;

		if (Y > 0.008856f) 
		{
			L	=	116.0f * pow(Y,0.33333f) - 16;
		}
		else
		{
			L	=	903.3f * Y;
		}

		a	=	500.0f * (_f_RGB2CIELab(X) - _f_RGB2CIELab(Y));
		b  =  200.0f *(_f_RGB2CIELab(Y) - _f_RGB2CIELab(Z));
		
		_imageInCIELAB[i].r()	=	L;
		_imageInCIELAB[i].g()	=	a;
		_imageInCIELAB[i].b()	=	b;

		
	}

	return true;
}


//Filter, CColor4f R,G,B [0,1]
bool ImageUtility::startBilateralFilter()
{
	
	//switch to CIELab space
    //=Source Image stores original RGB
    //=Temp Image stores CIElab color translated from RGB source Image
    //=Result Image stores filtered Color in RGB space
	if(RGB_to_CIELAB(m_filter->getSourceImage(),
								m_filter->getTempImage(),
								m_filter->getImageWidth(),
								m_filter->getImageHeight())
								==false)
	{						
		return false;
	}

	//saveToFileFromColor4f(L"c:\\CIELab.bmp",)

	if(m_filter->startFiltering()==false)
		return false;
	
	//switch back to CIELab
	//if(CIELAB_to_RGB(m_filter->getResultImage(),
	//	m_filter->getResultImage(),
	//	m_filter->getImageWidth(),
	//	m_filter->getImageHeight())
	//	==false)
	//{						
	//	return false;
	//}

	return true;

}

bool ImageUtility::setBilateralFilterData(
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
							bool _usePixelDistance,
							bool _usePerceptionDistance,
							bool _useNormalDistance,
							bool _useDepthDistance,
							bool _usematerialDistance
							)
{

	if(m_filter==NULL)
	{
		m_filter	=	new BilateralFilter(_filterBoxSize);
	}
	if(m_filter==NULL)
		return false;
	
	bool rt	=	
	m_filter->setFilterData(
		_sourceImage,
		_resultImage,
		_imageWidth,
		_imageHeight,
		_filterBoxSize,
		_pixelInfo,
		_torPixelPositionDistance,
		_torPerceptionDistance,
		_torNormalDistance,
		_torDepthDistance,
		_torMaterialDistance,
		_usePixelDistance,
		_usePerceptionDistance,
		_useNormalDistance,
		_useDepthDistance,
		_usematerialDistance
		);

	return rt;


}


void ImageUtility::charToWchar(char* _in, wchar_t* _out)
{
    if(_in==NULL||_out==NULL)
        return;
    
    int i=0;
    for(;_in[i]!='\0';i++)
    {
        _out[i]  =  _in[i];
    }

    _out[i]  =  '\0';


}

bool ImageUtility::saveToFileFromPixelVec(char* fileName,std::vector<ZH::reSamplePixel*>&  _colVec,int width,int height)
{
	Bitmap _map(width,height,PixelFormat24bppRGB);

	BitmapData* b_data	=	new BitmapData;
	_map.LockBits(new Rect(0,0,width,height),ImageLockModeWrite,PixelFormat24bppRGB,b_data);
	int stride = b_data->Stride;
	BYTE *point = (BYTE*)b_data->Scan0;

	std::vector<reSamplePixel*>::iterator	t_ite;

		CColor4f t_tempColor;
		int t_row,t_col,index;
		for(t_ite = _colVec.begin();t_ite!=_colVec.end();t_ite++)
		{
			t_tempColor	=	((*t_ite)->m_color) * 0.4f;
			t_row	=	(*t_ite)->m_nRow;
			t_col		=	(*t_ite)->m_nCol;
			index	=	t_row*stride+3*t_col;
			*(point+index)		=	static_cast<BYTE>(min(max(t_tempColor.b()*255,0),255));
			*(point+index+1)	=	static_cast<BYTE>(min(max(t_tempColor.g()*255,0),255));
			*(point+index+2)	=	static_cast<BYTE>(min(max(t_tempColor.r()*255,0),255));
		}

		_map.UnlockBits(b_data);

		CLSID bmpClsid;
		GetEncoderClsid(L"image/bmp", &bmpClsid);

		wchar_t t_fileName[MAX_PATH];
		charToWchar(fileName,t_fileName);

		_map.Save(t_fileName,&bmpClsid,NULL);      

		delete b_data;
		return true;
}



}


