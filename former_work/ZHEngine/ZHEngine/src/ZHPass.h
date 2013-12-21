#pragma once

#include "ZHRenderDecl.h"
#include "ZHCommonDefine.h"




namespace ZH
{

	class ZH_DLL  Pass
	{
	public:
		explicit Pass();
		~Pass();

	public:
		//设置渲染环境
		bool establish();
	private:
		RenderSystem*			_renderSystem;
		LPDIRECT3DDEVICE9	_device;

	public:
		//是否允许混合
		inline bool ifBlendEnable()const{return _blendEnable;}
		inline void setBlendEnable(bool boolin){_blendEnable	=	boolin;}
		
		//混合源参数
		inline ZH_BLEND_FACTOR getSourceBlendFactor()const{return _srcFactor;}
		inline void setSourceBlendFactor(const ZH_BLEND_FACTOR& inFactor){_srcFactor	=	inFactor;}
		
		//混合目标参数
		inline ZH_BLEND_FACTOR	getDestBlendFactor()const{return _destFactor;}
		inline void setDestBlendFactor(const ZH_BLEND_FACTOR& inFactor){_destFactor	=	inFactor;}
	
		//是否允许深度检测
		inline bool ifEnableDepthCheck()const{return _depthCheckEnable;}
		inline void setDepthCheckEnable(bool inbool){_depthCheckEnable	=	inbool;}

		inline bool ifEnableDepthWrite()const{return _depthWriteEnable;}
		inline void setDepthWriteEnable(bool inbool){_depthWriteEnable	=	inbool;}
		
		inline ZH_DEPTH_FUNC getDepthFunc()const{return _depthFunc;}
		inline void setDepthFunc(const ZH_DEPTH_FUNC& inFunc){_depthFunc	=	inFunc;}


		inline ZH_CULLMODE	getCullMode()const{return _cullMode;}
		inline void setCullMode(const ZH_CULLMODE& inMode){_cullMode	=	inMode;}


		inline bool getLightEnable()const{return _lightingEnable;}
		inline void setLightingEnable(bool inBool){_lightingEnable	=	inBool;}

		inline ZH_SHADE_OPTION getShadeOption()const{return _shadeOption;}
		inline void setShadeOption(const ZH_SHADE_OPTION& inOption){_shadeOption	=	inOption;}
		
		inline bool getFogEnable()const{return _fogEnable;}
		inline void setFogEnable(bool inBool){_fogEnable	=	inBool;}

		inline ZH_FOG_MODE getFogMode()const{return _fogMode;}
		inline void setFogMode(const ZH_FOG_MODE& inFogMode){_fogMode	=	inFogMode;}

		inline ZHCOLOR getFogColor()const{return _fogColor;}
		inline void setFogColor(const ZHCOLOR& inColor){_fogColor	=	inColor;}

		inline float getFogStart()const{return _fogStart;}
		inline void setFogStart(float inFogStart){_fogStart	=	inFogStart;}

		inline float getFogEnd()const{return _fogEnd;}
		inline void setFogEnd(float inFogEnd){_fogEnd	=	inFogEnd;}

		inline float getFogDensity()const{return _fogDensity;}
		inline void setFogDensity(float inFogDensity){_fogDensity	=	inFogDensity;}

		inline ZH_TEXTURE_FILTERTYPE getTextureMagFilter()const{return _magFilter;}
		inline void setTextureMagFilter(const ZH_TEXTURE_FILTERTYPE& inFilter){_magFilter =inFilter;}
	
		inline ZH_TEXTURE_FILTERTYPE getTextureMinFilter()const{return _minFilter;}
		inline void setTextureMinTilter(const ZH_TEXTURE_FILTERTYPE& inFilter){_minFilter	=	inFilter;}

		inline int getTextureMaxAnisotropy()const{return _maxAnisotropy;}
		inline void setTextureMaxAnisotropy(int inValue){_maxAnisotropy	=	inValue;}

		inline ZH_TEXTURE_ADDRESSMODE getTextureAddrMode()const{return _addressMode;}
		inline void setTextureAddressMode(const ZH_TEXTURE_ADDRESSMODE& inAddMode){_addressMode	=	inAddMode;}

		inline ZH_TEXTURE_COLOROP getTextureColorOp()const{return _textureColorOp;}
		inline void setTextureColorOp(const ZH_TEXTURE_COLOROP& inColorOp){_textureColorOp	=	inColorOp;}
	
		inline ZH_TEXTURE_COLOROP getTextureAlphaOp()const{return _textureAlphaOp;}
		inline void setTextureAlphaOp(const ZH_TEXTURE_COLOROP& inAlphaOp){_textureAlphaOp	=	inAlphaOp;}
	
	private:
		
		//Blending
		bool _blendEnable;
		ZH_BLEND_FACTOR _srcFactor;
		ZH_BLEND_FACTOR _destFactor;
		
		//Depth
		bool _depthCheckEnable;
		bool _depthWriteEnable;
		ZH_DEPTH_FUNC _depthFunc;


		//Culling mode
		ZH_CULLMODE _cullMode;

		//Lighting
		bool _lightingEnable;

		//Shade option,FLAT,GOURAUD,PHONG
		ZH_SHADE_OPTION _shadeOption;

		//Fog
		bool _fogEnable;
		ZH_FOG_MODE	_fogMode;
		ZHCOLOR _fogColor;
		float _fogStart;
		float _fogEnd;
		float _fogDensity;
		
		//Texture
		ZH_TEXTURE_FILTERTYPE _magFilter;
		ZH_TEXTURE_FILTERTYPE _minFilter;
				//ZH_TEXTURE_FILTERTYPE _mipFilter;
		int  _maxAnisotropy;//1,2,3,4
		ZH_TEXTURE_ADDRESSMODE _addressMode;
		ZH_TEXTURE_COLOROP _textureColorOp;
		ZH_TEXTURE_COLOROP _textureAlphaOp;


	};

};