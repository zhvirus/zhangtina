#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHPass.h"
#include "ZHSingleton.h"
#include "ZHRenderSystem.h"

namespace	ZH
{

	Pass::Pass():
		_renderSystem(NULL),
		_device(NULL),
		
		_blendEnable(false),
		_srcFactor(ZH_BLEND_SRCALPHA),
		_destFactor(ZH_BLEND_INVSRCALPHA),
		
		_depthCheckEnable(true),
		_depthWriteEnable(true),
		_depthFunc(ZH_CMP_LESS),

		_cullMode(ZH_CULL_NONE),
		_lightingEnable(false),
		_shadeOption(ZH_SHADE_GOURAUD),

		_fogEnable(false),
		_fogMode(ZH_FOG_LINEAR),
		_fogColor(0x00ffffff),
		_fogStart(0.5f),
		_fogEnd(20.8f),
		_fogDensity(0.5f),

		_magFilter(ZH_TEXF_LINEAR),
		_minFilter(ZH_TEXF_LINEAR),
		_maxAnisotropy(1),
		_addressMode(ZH_TADDRESS_WRAP),
		_textureColorOp(ZH_TOP_SELECTARG1),
		_textureAlphaOp(ZH_TOP_DISABLE)

	{
		_renderSystem	=	Singleton<RenderSystem>::getSingletonPtr();
		_device	=	_renderSystem->getDevice();

	}

	Pass::~Pass()
	{




	}

	bool Pass::establish()
	{
		if(_device==NULL)
			return false;
		
		//belending
		_device->SetRenderState(D3DRS_ALPHABLENDENABLE,_blendEnable);
		_device->SetRenderState(D3DRS_SRCBLEND,_srcFactor);
		_device->SetRenderState(D3DRS_DESTBLEND,_destFactor);

		//depth
		_device->SetRenderState(D3DRS_ZENABLE,_depthCheckEnable);
		_device->SetRenderState(D3DRS_ZWRITEENABLE,_depthWriteEnable);
		_device->SetRenderState(D3DRS_ZFUNC,_depthFunc);

		//cull mode
		_device->SetRenderState(D3DRS_CULLMODE,_cullMode);

		//lighting
		_device->SetRenderState(D3DRS_LIGHTING,_lightingEnable);

		//shadeOption
		_device->SetRenderState(D3DRS_SHADEMODE,_shadeOption);

		//Fog
		_device->SetRenderState(D3DRS_FOGENABLE,_fogEnable);
		_device->SetRenderState(D3DRS_FOGTABLEMODE,_fogMode);
		_device->SetRenderState(D3DRS_FOGCOLOR,_fogColor);
		_device->SetRenderState(D3DRS_FOGSTART,*((DWORD*)(&_fogStart)));
		_device->SetRenderState(D3DRS_FOGEND,*((DWORD*)(&_fogEnd)));
		_device->SetRenderState(D3DRS_FOGDENSITY,*((DWORD*)(&_fogDensity)));

		//texture
		_device->SetSamplerState(0,D3DSAMP_MAGFILTER,_magFilter);
		_device->SetSamplerState(0,D3DSAMP_MINFILTER,_minFilter);
		_device->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,_renderSystem->getDeviceCaps().MaxAnisotropy);
		
		_device->SetSamplerState(0,D3DSAMP_ADDRESSU,_addressMode);
		_device->SetSamplerState(0,D3DSAMP_ADDRESSV,_addressMode);
		
		_device->SetTextureStageState(0,D3DTSS_COLOROP,_textureColorOp);
		_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
		_device->SetTextureStageState(0,D3DTSS_ALPHAOP,_textureAlphaOp);

		return true;
	}





}