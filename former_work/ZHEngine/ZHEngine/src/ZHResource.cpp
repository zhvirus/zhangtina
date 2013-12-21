#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHResource.h"
#include "ZHBufferManager.h"
#include "ZHTextureManager.h"
#include "ZHSingleton.h"

namespace ZH
{

	Resource::Resource():
		_bufferManager(NULL)
	{
	
		_bufferManager	=	Singleton<BufferManager>::getSingletonPtr();
		_textureManager	=	Singleton<TextureManager>::getSingletonPtr();

	}

	Resource::~Resource()
	{
	
		SAFE_DELETE(_bufferManager);
		SAFE_DELETE(_textureManager);

	}













}