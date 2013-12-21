#pragma once

#include "ZHRenderDecl.h"


namespace ZH
{
	class ZH_DLL TextureManager
	{
		typedef std::map<std::string,RES_UNIT<LPDIRECT3DTEXTURE9>* > TEX_MAP;
		typedef std::map<std::string,RES_UNIT<LPDIRECT3DTEXTURE9>* >::iterator	TEX_MAP_ITERATOR;


	public:
		TextureManager();
		~TextureManager();
	public:
		bool addTexture(std::string texName,LPDIRECT3DTEXTURE9 tex);
		bool removeTexture(const std::string& texName);
		inline size_t getTextureCount()const{return numOfTexture;}
		bool	getTextureByName(std::string texName,LPDIRECT3DTEXTURE9& texToReceive);
	private:
		LPDIRECT3DDEVICE9 _device;
		TEX_MAP _texMap;
		TEX_MAP_ITERATOR	_texIterator;

		//当前纹理个数
		size_t numOfTexture;



	};





}