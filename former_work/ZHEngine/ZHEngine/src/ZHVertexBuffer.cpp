#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHVertexBuffer.h"
#include "ZHRenderSystem.h"
#include "ZHSingleton.h"
#include "ZHResource.h"

namespace ZH
{

	VertexBuffer::VertexBuffer(
		DWORD usageIn,
		D3DPOOL poolIn,
		unsigned int numOfVerticesIn,
		unsigned int vertexSizeIn,
		BYTE* dataIn,
		std::string bufferName
		):
		_bufferName(bufferName),
		_D3DBuffer(NULL),
		_usage(usageIn),
		_pool(poolIn),
		_BufferSizeInBytes(0),
		_NumOfVertices(numOfVerticesIn),
		_VertexSizeInBytes(vertexSizeIn),
		ifEmpty(true),
		_resource(NULL)
	{
		
		_resource	=	Singleton<Resource>::getSingletonPtr();

		//计算buffuer总字节数
		_BufferSizeInBytes		=	_NumOfVertices*_VertexSizeInBytes;
		
		//查询当前显示设备
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();

		//创建D3D 顶点缓存
		if(_device)
		{
			_D3DBuffer	=	NULL;
			//查询 buffer是否在资源管理器中存在
			_resource->_bufferManager->getVertexBufferByName(_bufferName,_D3DBuffer);
			//buffer没有存在于buffer资源中，那么重新创建
			if(_D3DBuffer==NULL)
			{

				if( FAILED( _device->CreateVertexBuffer( 
																_BufferSizeInBytes,
																_usage, 
																0,
																_pool,
																&_D3DBuffer, NULL ) ) )
				{
					ZH_ERR("Create vertex buffer failed!");
				}
				else
				{

				//记录到Resource中去
					_resource->_bufferManager->addVertexBuffer(_bufferName,_D3DBuffer);


				}
			}

		}

		if(!writeData(0,_BufferSizeInBytes,dataIn))
		{
			ZH_ERR("VertexBuffer::VertexBuffer()----failed!");
		}
		else
		{
			ifEmpty	=	false;
		}

	}


	VertexBuffer::~VertexBuffer()
	{

		//释放顶点缓存
		_resource->_bufferManager->releaseVertexBufferByName(_bufferName);

	}

	bool VertexBuffer::readData(unsigned int offsetIn, unsigned int lengthIn, void* pDest)
	{

		//读取数据过大
		if(offsetIn+lengthIn>_BufferSizeInBytes||pDest==NULL||ifEmpty	==true)
			return false;

		BYTE* source;
		if(FAILED(_D3DBuffer->Lock(offsetIn,lengthIn,(void**)(&source),D3DLOCK_READONLY)))
		{

			ZH_ERR(TEXT("vertexBuffer-->readData--->lock data failed!"));
			return false;
		}
		memcpy(pDest,source,lengthIn);
		
		_D3DBuffer->Unlock();

		return true;


	}

	bool VertexBuffer::writeData(unsigned int offsetIn,unsigned int lengthIn,void* pSource)
	{

		//写入数据过大
		if((offsetIn+lengthIn>_BufferSizeInBytes)||pSource==NULL)
			return false;

		BYTE* to;
		if(FAILED(_D3DBuffer->Lock(offsetIn,lengthIn,(void**)(&to),D3DLOCK_DISCARD)))
		{

			ZH_ERR(TEXT("vertexBuffer-->writeData--->lock data failed!"));
			return false;
		}
		memcpy(to,pSource,lengthIn);
		ifEmpty	=	false;

		_D3DBuffer->Unlock();

		ifEmpty	=	false;

		return true;


	}






}