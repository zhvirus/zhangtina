#ifndef _ZH_DLL
#define _ZH_DLL
#endif



#include "ZHVideoMode.h"

namespace ZH
{

	VideoMode::VideoMode(unsigned int width,unsigned int height,unsigned int refreshRate,D3DFORMAT format):
	_width(width),
	_height(height),
	_refreshRate(refreshRate),
	_format(format)
	{


	}

	VideoMode::VideoMode(VideoMode& in)
	{
		memcpy(this,&in,sizeof(VideoMode));
	}

	VideoMode::VideoMode(D3DDISPLAYMODE displayMode)
	{
		_width	=	displayMode.Width;
		_height	=	displayMode.Height;
		_refreshRate	=	displayMode.RefreshRate;
		_format	=	displayMode.Format;


	}


	VideoMode::VideoMode()
	{


	}


	VideoMode::~VideoMode()
	{


	}

//-----------------------------------------
//videoMode List
//-----------------------------------------

	VideoModeList::VideoModeList()
	{

		_vModeList.clear();
	}

	VideoModeList::~VideoModeList()
	{
		std::vector<VideoMode*>::iterator ite;

		for(ite=_vModeList.begin();ite!=_vModeList.end();ite++)
		{
			SAFE_DELETE(*(ite));
		
		}

		_vModeList.clear();



	}

	bool VideoModeList::addVideoMode(ZH::VideoMode *vModeIn)
	{
		_vModeList.push_back(vModeIn);
		return true;

	}

	int VideoModeList::getVideoModeNum()const
	{
		return static_cast<int>(_vModeList.size());

	}

	bool VideoModeList::ifExist(const D3DDISPLAYMODE& displayMode)
	{
		VideoMode In(displayMode);
			std::vector<VideoMode*>::iterator ite;
			for( ite = _vModeList.begin(); ite != _vModeList.end(); ite++ )
			{
				
				if( (*ite)->getWidth() == In.getWidth()&&
					(*ite)->getHeight()==In.getHeight()&&
					(*ite)->getPixelFormat()==In.getPixelFormat())
				{
					// Check refresh rate and favour higher if poss
					if ((*ite)->getRefreshRate()<In.getRefreshRate())
						(*ite)->increaseRefreshRate(In.getRefreshRate());
					{
						return true;
					}
				}
			}

			return false;

	}

	VideoMode VideoModeList::getSuitableVideoMode()
	{
		std::vector<VideoMode*>::iterator ite;
		for(ite=_vModeList.begin();ite!=_vModeList.end();ite++)
		{
			if((*ite)->getWidth()==1280&&(*ite)->getHeight()==800)
				return VideoMode(1280,800,60,D3DFMT_X8R8G8B8);


		}
		

			return VideoMode(1024,768,60,D3DFMT_X8R8G8B8);


	}





}