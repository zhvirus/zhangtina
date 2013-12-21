#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHTextureManager.h"
#include "ZHSingleton.h"
#include "ZHRenderSystem.h"
#include "ZHResource.h"

namespace ZH
{

	TextureManager::TextureManager():
		numOfTexture(0),
		_device(NULL)
	{
		//查询当前显示设备
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();

		_texMap.clear();
	}


	TextureManager::~TextureManager()
	{
		//释放所有纹理资源		
		for(_texIterator	=	_texMap.begin();_texIterator!=_texMap.end();_texIterator++)
		{
			SAFE_RELEASE((*_texIterator).second->data);
			SAFE_DELETE((*_texIterator).second);

		}

		_texMap.clear();


	}

	bool TextureManager::addTexture(std::string texName,LPDIRECT3DTEXTURE9 tex)
	{
		_texIterator	=	_texMap.find(texName);

		//在资源管理中找到了纹理文件
		if(_texIterator!=_texMap.end())
		{
			(*_texIterator).second->count+=1;

		}
		else
		{
			RES_UNIT<LPDIRECT3DTEXTURE9>*	temp	=	new RES_UNIT<LPDIRECT3DTEXTURE9>;
			if(temp==NULL)
				return false;

			temp->data	=	tex;
			temp->count	=	1;

			_texMap.insert(std::make_pair(texName,temp));


		}

		return true;



	}

	bool	TextureManager::getTextureByName(std::string texName,LPDIRECT3DTEXTURE9& texToReceive)
	{
		_texIterator	=	_texMap.find(texName);
		
		//找到纹理
		if(_texIterator!=_texMap.end())
		{
			texToReceive	=	(*_texIterator).second->data;
			(*_texIterator).second->count+=1;
			return true;

		}
		else
		{
			return false;
		}


	}

	bool TextureManager::removeTexture(const std::string& texName)
	{
		_texIterator	=	_texMap.find(texName);
		
		//如果找到纹理
		if(_texIterator!=_texMap.end())
		{
			int c_temp	=	(*_texIterator).second->count-1;
			if(c_temp<1)
			{
				SAFE_RELEASE((*_texIterator).second->data);
				SAFE_DELETE((*_texIterator).second);
				_texMap.erase(_texIterator);
			}

			return true;


		}

		return false;



	}



}