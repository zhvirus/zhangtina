#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHIndexBuffer.h"
#include "ZHRenderSystem.h"
#include "ZHSingleton.h"
#include "ZHResource.h"


namespace ZH
{

	IndexBuffer::IndexBuffer(
					D3DFORMAT formatIn,
					DWORD usageIn,
					D3DPOOL poolIn,
					unsigned int numOfIndicesIn,
					BYTE* dataIn,
					std::string bufferName
					):

		_format(formatIn),
		_usage(usageIn),
		_pool(poolIn),
		_NumOfIndices(numOfIndicesIn),
		_indexBuffer(NULL),
		ifEmpty(true),
		_bufferName(bufferName),
		_resource(NULL)
	{

		_resource	=	Singleton<Resource>::getSingletonPtr();

		if(_format==D3DFMT_INDEX16)
		{
			_IndexDataSizeInBytes	=	2;
			

		}
		else
		{

			_IndexDataSizeInBytes	=	4;
		}
		
		//计算 buffer总大小（字节数）
		_BufferSizeInBytes	=	_IndexDataSizeInBytes*_NumOfIndices;	

		//查询当前显示设备
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();

		_indexBuffer	=	NULL;
		//查询 buffer是否在资源管理器中存在
		_resource->_bufferManager->getIndexBufferByName(_bufferName,_indexBuffer);
		//buffer没有存在于buffer资源中，那么重新创建
		if(_indexBuffer==NULL)
		{
				//创建 索引缓冲区
				if(FAILED(_device->CreateIndexBuffer(
													_BufferSizeInBytes,
													_usage,
													_format,
													_pool,
													&_indexBuffer,
													NULL)))
				{
					ZH_ERR(TEXT("indexBuffer--->create---->failed!"));

				}
				else
				{
					//记录到Resource中去
					_resource->_bufferManager->addIndexBuffer(_bufferName,_indexBuffer);


				}
		}

		if(!writeData(0,_BufferSizeInBytes,dataIn))
		{
			ZH_ERR("IndexBuffer::IndexBuffer()----failed!");
		}
		else
		{
			ifEmpty	=	false;

		}
		

	}


	IndexBuffer::~IndexBuffer()
	{
		_resource->_bufferManager->releaseIndexBufferByName(_bufferName);

	}



	bool IndexBuffer::readData(unsigned int offsetIn, unsigned int lengthIn, void* pDest)
	{
		//读取数据越界
		if(offsetIn+lengthIn>_BufferSizeInBytes||pDest==NULL||ifEmpty	==true)
			return false;

		BYTE* source;
		if(FAILED(_indexBuffer->Lock(offsetIn,lengthIn,(void**)(&source),D3DLOCK_READONLY)))
		{

			ZH_ERR(TEXT("vertexBuffer-->readData--->lock data failed!"));
			return false;
		}
		memcpy(pDest,source,lengthIn);
		
		_indexBuffer->Unlock();

		return true;


	}

	bool IndexBuffer::writeData(unsigned int offsetIn,unsigned int lengthIn,void* pSource)
	{
		//写入数据越界
		if(offsetIn+lengthIn>_BufferSizeInBytes||pSource==NULL)
			return false;

		BYTE* to;
		if(FAILED(_indexBuffer->Lock(offsetIn,lengthIn,(void**)(&to),D3DLOCK_DISCARD)))
		{

			ZH_ERR(TEXT("vertexBuffer-->writeData--->lock data failed!"));
			return false;
		}
		memcpy(to,pSource,lengthIn);
		ifEmpty	=	false;
		_indexBuffer->Unlock();

		ifEmpty	=	false;

		return true;


	}










}