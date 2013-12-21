#pragma once

#include "ZHRenderDecl.h"
#include "ZHCommonDefine.h"


namespace ZH
{


//****************************************
//class name:			VertexBuffer
//author:				Hui Zhang
//latest modified:	2007.6.22
//description:			VertexBuffer
//****************************************

class ZH_DLL VertexBuffer
{                                                                                                                                                               
	public:
		
		explicit 
			VertexBuffer(
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfVerticesIn,
			unsigned int vertexSizeIn,
			BYTE* dataIn,
			std::string bufferName
			);

		~VertexBuffer();
	private:
		//D3D VertexBuffer
		LPDIRECT3DVERTEXBUFFER9	_D3DBuffer;
		LPDIRECT3DDEVICE9	_device;
		DWORD	_usage;
		D3DPOOL		_pool;
		
		//vertex buffer ���ܴ�С���ֽ�����
		unsigned int	_BufferSizeInBytes;

		//vertex buffer �еĶ�����Ŀ
		unsigned int	_NumOfVertices; 
		
		//vertex buffer �е���������ֽ���
		unsigned int	_VertexSizeInBytes;

		//buffer ����
		std::string _bufferName;

		//�Ƿ�������
		bool ifEmpty;

		//resource
		Resource* _resource;

	public:
		inline unsigned int	getBufferSize()const	{return _BufferSizeInBytes;}
		inline unsigned int	getVertexNum()const	{return _NumOfVertices;}
		inline unsigned int	getVertexSize()const	{return _VertexSizeInBytes;}
		inline DWORD		getUsage()const				{return _usage;}
		inline D3DPOOL		getPool()const				{return _pool;}
		inline bool getIfEmpty()const{return ifEmpty;}
		inline LPDIRECT3DVERTEXBUFFER9 getVertexBuffer()const {return _D3DBuffer;}
	public:
		//��buffer�ж�ȡ����
	    bool readData(unsigned int offsetIn, unsigned int lengthIn, void* pDest);
		//��bufer��д����
		bool writeData(unsigned int offsetIn,unsigned int lengthIn,void* pSource);
		

};

}