#pragma once

#include "ZHRenderDecl.h"
#include "ZHVideoMode.h"

namespace ZH
{

class ZH_DLL ZHDisplayAdapter
{
public:
	ZHDisplayAdapter(unsigned int ID,const LPDIRECT3D9& inD3DDevice );
public:
	~ZHDisplayAdapter(void);
public:
	bool initialize(const D3DADAPTER_IDENTIFIER9& adapterIn);
	VideoMode  getSuitableVideoMode();

private:
	int enumerateVideoMode();
private:

	// Direct3D
	LPDIRECT3D9			_mpD3D;

	unsigned int _adapterID;
	D3DADAPTER_IDENTIFIER9 _adapterInfo;
	TCHAR description[MAX_DEVICE_IDENTIFIER_STRING];
	TCHAR driver[MAX_DEVICE_IDENTIFIER_STRING];
	TCHAR deviceName[32];

	VideoModeList _videoModeList;


};


}
