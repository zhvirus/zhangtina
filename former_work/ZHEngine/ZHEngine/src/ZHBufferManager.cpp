#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHVertexBuffer.h"
#include "ZHIndexBuffer.h"
#include "ZHBufferManager.h"
#include "ZHResource.h"

namespace ZH
{


	BufferManager::BufferManager():
		indexBufferNum(0),
		vertexBufferNum(0)
	{
		indexBufferSource.clear();
		vertexBufferSource.clear();

	}


	BufferManager::~BufferManager()
	{

		//释放所有索引缓冲资源
		
		for(indexIterator =	indexBufferSource.begin(); indexIterator!=indexBufferSource.end(); indexIterator++)
		{
			SAFE_RELEASE((*indexIterator).second->data);//释放d3dBuffer
			SAFE_DELETE((*indexIterator).second);			//删除节点
		}

		indexBufferSource.clear();


		//释放所有顶点缓冲资源

		for(vertexIterator	=	vertexBufferSource.begin();vertexIterator!=vertexBufferSource.end();vertexIterator++)
		{
			
			SAFE_RELEASE(((*vertexIterator).second)->data);
			SAFE_DELETE((*vertexIterator).second);

		}

		vertexBufferSource.clear();


	}


	bool BufferManager::addIndexBuffer(std::string bufferNameIn,LPDIRECT3DINDEXBUFFER9 inIndexBuffer)
	{
		//查找这个buffer是否已经在资源管理面存在
		indexIterator	=	indexBufferSource.find(bufferNameIn);

		//如果buffer已经在资源管理里面存在了，则buffer计数j加1
		if(indexIterator!=indexBufferSource.end())
		{
			((*indexIterator).second->count)+=1;
			return true;
		}
		
		//如果这个buffer在资源管理器里面不存在
		RES_UNIT<LPDIRECT3DINDEXBUFFER9>* temp	=	new RES_UNIT<LPDIRECT3DINDEXBUFFER9>;
		if(temp=NULL)
			return false;
		temp->data	=	inIndexBuffer;
		temp->count	=	1;

		indexBufferSource.insert(std::make_pair(bufferNameIn,temp));
		
		indexBufferNum	=	indexBufferSource.size();

		return true;

	}

	bool BufferManager::addVertexBuffer(std::string bufferNameIn,LPDIRECT3DVERTEXBUFFER9 inVertexBuffer)
	{
		
		//查找这个buffer是否已经在资源管理面存在
		vertexIterator	=	vertexBufferSource.find(bufferNameIn);

		//如果buffer已经在资源管理里面存在了，则buffer计数j加1
		if(vertexIterator!=vertexBufferSource.end())
		{
			((*vertexIterator).second->count)+=1;
			return true;
		}
		
		//如果这个buffer在资源管理器里面不存在
		RES_UNIT<LPDIRECT3DVERTEXBUFFER9>* temp	=	new RES_UNIT<LPDIRECT3DVERTEXBUFFER9>;
		if(temp=NULL)
			return false;
		temp->data	=	inVertexBuffer;
		temp->count	=	1;

		vertexBufferSource.insert(std::make_pair(bufferNameIn,temp));
		
		vertexBufferNum	=	vertexBufferSource.size();

		return true;

	}

	//vertex
	bool BufferManager::ifExistVertex(std::string bufferNameIn)
	{

		if((vertexBufferSource.find(bufferNameIn))!=vertexBufferSource.end())
		{

			return true;	
		}

		else
		{

			return false;
		}


	}

	
	//index
	bool BufferManager::ifExistIndex(std::string bufferNameIn)
	{

		if((indexBufferSource.find(bufferNameIn))!=indexBufferSource.end())
		{

			return true;
		}
		else
		{
			return false;

		}


	}



	//vertex
	bool BufferManager::getVertexBufferByName(std::string bufferNameIn, LPDIRECT3DVERTEXBUFFER9 &bufferReturn)
	{

		
		vertexIterator	=	vertexBufferSource.find(bufferNameIn);
		//没有找到
		if(vertexIterator==vertexBufferSource.end())
		{
			bufferReturn	=	NULL;
			return false;

		}
		//找到
		else
		{
			bufferReturn	=	(*vertexIterator).second->data;
			(*vertexIterator).second->count+=1;
			return true;
		}




	}
	
	//index
	bool BufferManager::getIndexBufferByName(std::string bufferNameIn, LPDIRECT3DINDEXBUFFER9 &bufferReturn)
	{
		
		indexIterator	=	indexBufferSource.find(bufferNameIn);
		//没有找到
		if(indexIterator==indexBufferSource.end())
		{
			bufferReturn	=	NULL;
			return false;

		}
		//找到
		else
		{
			bufferReturn	=	(*indexIterator).second->data;
			(*indexIterator).second->count+=1;
			return true;
		}


	}


	bool BufferManager::releaseVertexBufferByName(std::string bufferNameIn)
	{
		vertexMapIterator ite	=	vertexBufferSource.find(bufferNameIn);

		if(ite==vertexBufferSource.end())
		{
			return false;

		}
		else
		{
			int used	=	(*ite).second->count-1;
			if(used<1)
			{
				SAFE_RELEASE((*ite).second->data);
				SAFE_DELETE((*ite).second);
				vertexBufferSource.erase(ite);
				vertexBufferNum	=	vertexBufferSource.size();
				
			}

			return true;
		
		}


	}

	bool BufferManager::releaseIndexBufferByName(std::string bufferNameIn)
	{
		indexMapIterator ite	=	indexBufferSource.find(bufferNameIn);

		if(ite==indexBufferSource.end())
		{
			return false;

		}
		else
		{
			int used	=	(*ite).second->count-1;
			if(used<1)
			{
				SAFE_RELEASE((*ite).second->data);
				SAFE_DELETE((*ite).second);
				indexBufferSource.erase(ite);
				indexBufferNum	=	indexBufferSource.size();
			}

			return true;
		
		}


	}






}