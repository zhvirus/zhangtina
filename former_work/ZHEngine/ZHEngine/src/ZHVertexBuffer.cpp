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

		//����buffuer���ֽ���
		_BufferSizeInBytes		=	_NumOfVertices*_VertexSizeInBytes;
		
		//��ѯ��ǰ��ʾ�豸
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();

		//����D3D ���㻺��
		if(_device)
		{
			_D3DBuffer	=	NULL;
			//��ѯ buffer�Ƿ�����Դ�������д���
			_resource->_bufferManager->getVertexBufferByName(_bufferName,_D3DBuffer);
			//bufferû�д�����buffer��Դ�У���ô���´���
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

				//��¼��Resource��ȥ
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

		//�ͷŶ��㻺��
		_resource->_bufferManager->releaseVertexBufferByName(_bufferName);

	}

	bool VertexBuffer::readData(unsigned int offsetIn, unsigned int lengthIn, void* pDest)
	{

		//��ȡ���ݹ���
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

		//д�����ݹ���
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