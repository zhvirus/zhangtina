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
		//D3D ��������
		LPDIRECT3DINDEXBUFFER9	_indexBuffer;

		//D3D�豸
		LPDIRECT3DDEVICE9	_device;
		
		//��������С
		unsigned int _BufferSizeInBytes;

		//�������ݴ�С
		unsigned int	_IndexDataSizeInBytes; 

		//�������ݸ���
		unsigned int _NumOfIndices;
		
		//
		DWORD _usage;

		//�������ݸ�ʽ
		D3DFORMAT _format;

		//
		D3DPOOL _pool;

		//�Ƿ�Ϊ��
		bool ifEmpty;

		//buffer ����
		std::string _bufferName;

		//��Դ
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

		//��buffer�ж�ȡ����
	    bool readData(unsigned int offsetIn, unsigned int lengthIn, void* pDest);
		//��bufer��д����
		bool writeData(unsigned int offsetIn,unsigned int lengthIn,void* pSource);



	};
}