#pragma once 

#include "ZHRenderDecl.h"
#include "ZHBufferManager.h"
#include "ZHTextureManager.h"

namespace ZH
{
	template<class T>
	class RES_UNIT
	{
		public:
			RES_UNIT():data(NULL),count(0){}
			T data;
			int count;

	};

	class	ZH_DLL Resource
	{
	
		public:
			explicit Resource();
			~Resource();

		public:	
			BufferManager* _bufferManager;
			TextureManager* _textureManager;

	};

}