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

		//�ͷ���������������Դ
		
		for(indexIterator =	indexBufferSource.begin(); indexIterator!=indexBufferSource.end(); indexIterator++)
		{
			SAFE_RELEASE((*indexIterator).second->data);//�ͷ�d3dBuffer
			SAFE_DELETE((*indexIterator).second);			//ɾ���ڵ�
		}

		indexBufferSource.clear();


		//�ͷ����ж��㻺����Դ

		for(vertexIterator	=	vertexBufferSource.begin();vertexIterator!=vertexBufferSource.end();vertexIterator++)
		{
			
			SAFE_RELEASE(((*vertexIterator).second)->data);
			SAFE_DELETE((*vertexIterator).second);

		}

		vertexBufferSource.clear();


	}


	bool BufferManager::addIndexBuffer(std::string bufferNameIn,LPDIRECT3DINDEXBUFFER9 inIndexBuffer)
	{
		//�������buffer�Ƿ��Ѿ�����Դ���������
		indexIterator	=	indexBufferSource.find(bufferNameIn);

		//���buffer�Ѿ�����Դ������������ˣ���buffer����j��1
		if(indexIterator!=indexBufferSource.end())
		{
			((*indexIterator).second->count)+=1;
			return true;
		}
		
		//������buffer����Դ���������治����
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
		
		//�������buffer�Ƿ��Ѿ�����Դ���������
		vertexIterator	=	vertexBufferSource.find(bufferNameIn);

		//���buffer�Ѿ�����Դ������������ˣ���buffer����j��1
		if(vertexIterator!=vertexBufferSource.end())
		{
			((*vertexIterator).second->count)+=1;
			return true;
		}
		
		//������buffer����Դ���������治����
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
		//û���ҵ�
		if(vertexIterator==vertexBufferSource.end())
		{
			bufferReturn	=	NULL;
			return false;

		}
		//�ҵ�
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
		//û���ҵ�
		if(indexIterator==indexBufferSource.end())
		{
			bufferReturn	=	NULL;
			return false;

		}
		//�ҵ�
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