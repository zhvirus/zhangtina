#pragma once
#include "ZHRenderDecl.h"

namespace ZH
{

//****************************************
//class name:		VideoMode
//author:				Hui Zhang
//latest modified:	2007.3.27
//description:			
//****************************************

	class ZH_DLL VideoMode
	{
	public:
		VideoMode(unsigned int width,unsigned int height,unsigned int refreshRate,D3DFORMAT format);
		VideoMode(VideoMode& in);
		VideoMode(D3DDISPLAYMODE displayMode);
		VideoMode();
		~VideoMode();
		
		inline unsigned int getWidth()const{return _width;}
		inline unsigned int getHeight()const{return _height;}
		inline unsigned int getRefreshRate()const{return _refreshRate;}
		inline D3DFORMAT getPixelFormat()const{return _format;}
		inline void increaseRefreshRate(unsigned int inRefreshRate){_refreshRate	=	inRefreshRate;}
		inline void setWidth(const unsigned int& width){_width	=	width;}
		inline void setHeight(const unsigned int& height){_height	=	height;}
		inline void setRefreshRate(const unsigned int& refreshRate){_refreshRate	=	refreshRate;}
		inline void setPixelFormat(const D3DFORMAT& inFormat){_format	=	inFormat;}
	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _refreshRate;
		D3DFORMAT _format;



	};


	class ZH_DLL VideoModeList
	{

	public:
		VideoModeList();
		~VideoModeList();
		bool addVideoMode(VideoMode* vModeIn);
		int getVideoModeNum()const;
		bool ifExist(const D3DDISPLAYMODE& displayMode);
		VideoMode getSuitableVideoMode();

	private:
		std::vector<VideoMode*> _vModeList;

	};








}