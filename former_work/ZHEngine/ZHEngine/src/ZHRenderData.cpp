#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHRenderData.h"
#include "ZHVertexBuffer.h"
#include "ZHIndexBuffer.h"
#include "ZHRenderSystem.h"
#include "ZHSingleton.h"

namespace ZH
{

	RenderData::RenderData(
			LPDIRECT3DVERTEXDECLARATION9 vertexDecl,
			D3DPRIMITIVETYPE	primitiveType

			):
		_vertexBuffer(0),
		_indexBuffer(0),
		_primitiveType(primitiveType ),		
		_vertexDeclara(vertexDecl),
		useIndex(false),
		numOfFaces(0)
	{
		
		//查询当前设备
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();
		
		
	

	}

	RenderData::~RenderData()
	{

		SAFE_DELETE(_vertexBuffer);
		SAFE_DELETE(_indexBuffer);


	}

	bool RenderData::createVertexBuffer(			
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfVerticesIn,
			unsigned int vertexSizeIn,
			BYTE* dataIn,
			std::string bufferName)
	{
		_vertexBuffer	=	new VertexBuffer(
			usageIn,
			poolIn,
			numOfVerticesIn,
			vertexSizeIn,
			dataIn,
			bufferName);

		if(_vertexBuffer!=NULL)
		{
			//计算总的三角片数目
			numOfFaces	=	static_cast<size_t>(numOfVerticesIn/3.0f);
			return true;
		}
		else
		{
			return false;
		}

	}


	bool RenderData::createIndexBuffer(
			D3DFORMAT formatIn,
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfIndicesIn,
			BYTE* dataIn,
			std::string bufferName)
	{
		_indexBuffer	=	new IndexBuffer(
			formatIn,
			usageIn,
			poolIn,
			numOfIndicesIn,
			dataIn,
			bufferName);

		if(_indexBuffer!=NULL)
		{
			//计算总的三角片数目
			numOfFaces	=	static_cast<size_t>(numOfIndicesIn/3.0f);
			useIndex	=	true;
			return true;
		}
		else
		{

			return false;
		}

	}










}