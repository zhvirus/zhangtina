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

		//增加一个索引缓冲
		bool addIndexBuffer(std::string bufferNameIn,LPDIRECT3DINDEXBUFFER9 inIndexBuffer);
		//增加一个顶点缓冲
		bool addVertexBuffer(std::string bufferNameIn,LPDIRECT3DVERTEXBUFFER9 inVertexBuffer);
		
		//根据buffer名字查询 该buffer是否已经存在
		bool	ifExistVertex(std::string bufferNameIn);
		bool	ifExistIndex(std::string bufferNameIn);

		//根据buffer名字返回buffer
		bool getIndexBufferByName(std::string bufferNameIn,LPDIRECT3DINDEXBUFFER9& bufferReturn);
		bool getVertexBufferByName(std::string bufferNameIn,LPDIRECT3DVERTEXBUFFER9& bufferReturn);

		//根据名字来释放buffer
		bool releaseVertexBufferByName(std::string bufferNameIn);
		bool releaseIndexBufferByName(std::string bufferNameIn);
	
	private:		

		//当前索引缓冲的个数
		size_t	indexBufferNum;

		//当前顶点缓冲的个数
		size_t	vertexBufferNum;

		//索引缓冲map
		indexMap	indexBufferSource;
		indexMapIterator	indexIterator;
		//顶点数据缓冲map
		vertexMap vertexBufferSource;
		vertexMapIterator	vertexIterator;

	};


}