#pragma once

#include "ZHRenderDecl.h"

namespace ZH
{

	class ZH_DLL IndexBuffer
	{

	public:
		explicit IndexBuffer(D3DFORMAT formatIn,
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfIndicesIn,
			BYTE* dataIn,
			std::string bufferName
			);
		~IndexBuffer();
	private:
		//D3D 索引缓冲
		LPDIRECT3DINDEXBUFFER9	_indexBuffer;

		//D3D设备
		LPDIRECT3DDEVICE9	_device;
		
		//缓冲区大小
		unsigned int _BufferSizeInBytes;

		//索引数据大小
		unsigned int	_IndexDataSizeInBytes; 

		//索引数据个数
		unsigned int _NumOfIndices;
		
		//
		DWORD _usage;

		//索引数据格式
		D3DFORMAT _format;

		//
		D3DPOOL _pool;

		//是否为空
		bool ifEmpty;

		//buffer 名字
		std::string _bufferName;

		//资源
		Resource* _resource;

	public:
		inline LPDIRECT3DINDEXBUFFER9 getIndexBufer()const{return _indexBuffer;}
		inline unsigned int getBufferSizeInBytes()const{return _BufferSizeInBytes;}
		inline unsigned int getIndexDataSizeInBytes()const{return _IndexDataSizeInBytes;}
		inline unsigned int getIndexDataNum()const{return _NumOfIndices;}
		inline DWORD getUsage()const{return _usage;}
		inline D3DFORMAT getFormat()const{return _format;}
		inline D3DPOOL getPool()const{return _pool;}

	public:

		//从buffer中读取数据
	    bool readData(unsigned int offsetIn, unsigned int lengthIn, void* pDest);
		//向bufer中写数据
		bool writeData(unsigned int offsetIn,unsigned int lengthIn,void* pSource);



	};
}