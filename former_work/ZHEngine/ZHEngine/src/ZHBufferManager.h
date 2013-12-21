#pragma once

#include "ZHRenderDecl.h"

namespace ZH
{

	class ZH_DLL BufferManager
	{
	

		typedef std::map<std::string,RES_UNIT<LPDIRECT3DINDEXBUFFER9>* >	indexMap;
		typedef std::map<std::string,RES_UNIT<LPDIRECT3DINDEXBUFFER9>* >::iterator	indexMapIterator;
		typedef std::map<std::string,RES_UNIT<LPDIRECT3DVERTEXBUFFER9>* >	vertexMap;
		typedef std::map<std::string,RES_UNIT<LPDIRECT3DVERTEXBUFFER9>* >::iterator	vertexMapIterator;

	public:
		
		explicit BufferManager();
		~BufferManager();

		//����һ����������
		bool addIndexBuffer(std::string bufferNameIn,LPDIRECT3DINDEXBUFFER9 inIndexBuffer);
		//����һ�����㻺��
		bool addVertexBuffer(std::string bufferNameIn,LPDIRECT3DVERTEXBUFFER9 inVertexBuffer);
		
		//����buffer���ֲ�ѯ ��buffer�Ƿ��Ѿ�����
		bool	ifExistVertex(std::string bufferNameIn);
		bool	ifExistIndex(std::string bufferNameIn);

		//����buffer���ַ���buffer
		bool getIndexBufferByName(std::string bufferNameIn,LPDIRECT3DINDEXBUFFER9& bufferReturn);
		bool getVertexBufferByName(std::string bufferNameIn,LPDIRECT3DVERTEXBUFFER9& bufferReturn);

		//�����������ͷ�buffer
		bool releaseVertexBufferByName(std::string bufferNameIn);
		bool releaseIndexBufferByName(std::string bufferNameIn);
	
	private:		

		//��ǰ��������ĸ���
		size_t	indexBufferNum;

		//��ǰ���㻺��ĸ���
		size_t	vertexBufferNum;

		//��������map
		indexMap	indexBufferSource;
		indexMapIterator	indexIterator;
		//�������ݻ���map
		vertexMap vertexBufferSource;
		vertexMapIterator	vertexIterator;

	};


}