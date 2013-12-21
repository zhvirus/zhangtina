#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHDisplayAdapter.h"

namespace ZH
{


ZHDisplayAdapter::ZHDisplayAdapter(unsigned int ID,const LPDIRECT3D9& inD3DDevice):
_adapterID(ID),
_mpD3D(inD3DDevice)
{
	strcpy(description,TEXT(""));
	strcpy(driver,TEXT(""));
	strcpy(deviceName,TEXT(""));


}

ZHDisplayAdapter::~ZHDisplayAdapter(void)
{
}

bool ZHDisplayAdapter::initialize(const D3DADAPTER_IDENTIFIER9 &adapterIn)
{
	_adapterInfo	=	adapterIn;
	strcpy(description,adapterIn.Description);
	strcpy(driver,adapterIn.Driver);
	strcpy(deviceName,adapterIn.DeviceName);
	
	//查询显示模式
	enumerateVideoMode();

	return true;

}

int ZHDisplayAdapter::enumerateVideoMode()
{
		for(unsigned int iMode=0; iMode <(_mpD3D->GetAdapterModeCount(_adapterID, D3DFMT_R5G6B5 )); iMode++ )
		{
			D3DDISPLAYMODE displayMode;
			_mpD3D->EnumAdapterModes( _adapterID, D3DFMT_R5G6B5, iMode, &displayMode );

			// Filter out low-resolutions
			if( displayMode.Width < 640 || displayMode.Height < 400 )
				continue;
			
			//查询到了新的显示模式
			if( !_videoModeList.ifExist(displayMode) )
			{
				VideoMode* vM	=	new VideoMode(displayMode);
				_videoModeList.addVideoMode(vM);
		
			}
		}


		for(unsigned int iMode=0; iMode <(_mpD3D->GetAdapterModeCount(_adapterID, D3DFMT_R5G6B5 )); iMode++ )
		{
			D3DDISPLAYMODE displayMode;
			_mpD3D->EnumAdapterModes( _adapterID, D3DFMT_X8R8G8B8, iMode, &displayMode );

			// Filter out low-resolutions
			if( displayMode.Width < 640 || displayMode.Height < 400 )
				continue;
			
			//查询到了新的显示模式
			if( !_videoModeList.ifExist(displayMode) )
			{
				VideoMode* vM	=	new VideoMode(displayMode);
				_videoModeList.addVideoMode(vM);
		
			}
		}


return 1;
}


VideoMode  ZHDisplayAdapter::getSuitableVideoMode()
{


	return _videoModeList.getSuitableVideoMode();

}




}