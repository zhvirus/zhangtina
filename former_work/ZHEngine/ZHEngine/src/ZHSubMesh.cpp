#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSubMesh.h"
#include "ZHRenderData.h"
#include "ZHMesh.h"
#include "ZHSingleton.h"
#include "ZHRenderSystem.h"
#include "ZHResource.h"
#include "ZHVertexBuffer.h"
#include "ZHIndexBuffer.h"

namespace ZH
{

	SubMesh::SubMesh(
		std::string textureNameIn,
		const D3DMATERIAL9 mateIn,
		Mesh* parentIn,
		std::string subMeshNameIn,
		LPDIRECT3DVERTEXDECLARATION9 vertexDeclIn,
		D3DPRIMITIVETYPE primitiveTypeIn
		):
		_subMeshName(subMeshNameIn),
		_primitiveType(primitiveTypeIn),
		_vertexDecl(vertexDeclIn),
		_textureName(textureNameIn),
		_texture(NULL),
		_material(mateIn),
		isTextureLoaded(false),
		_parentMesh(parentIn),
		_ifUseIndex(false)
	{
		//查询当前显示设备
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();
		
		//创建纹理
		if(!(Singleton<Resource>::getSingletonPtr()->_textureManager->getTextureByName(_textureName,_texture)))
		{
			//如果在资源中没有找到，那么就自己文件中创建
			if(FAILED(D3DXCreateTextureFromFile(_device,_textureName.c_str(),&_texture)))
			{
				//如果自己从文件创建纹理失败
				ZH_ERR("SubMesh::SubMesh()---->D3DXCreateTextureFromFile()-failed!");
				isTextureLoaded	=	false;
			}
			//将创建的纹理加入Resource
			Singleton<Resource>::getSingletonPtr()->_textureManager->addTexture(_textureName,_texture);
			isTextureLoaded	=	true;
		}
		else
		{
			isTextureLoaded	=	true;

		}

		
		data	=	new RenderData(_vertexDecl,_primitiveType);
		

	}

	SubMesh::~SubMesh()
	{
	
		SAFE_DELETE(data);

	}



	bool SubMesh::CreateVertex(
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfVerticesIn,
			unsigned int vertexSizeIn,
			BYTE* dataIn,
			std::string bufferName
			)
	{

		_numOfVertices	=	numOfVerticesIn;
		_numOfFaces	=static_cast<unsigned int>(numOfVerticesIn/3);

		if(data)
		{

				if(data->createVertexBuffer(
					usageIn,
					poolIn,
					numOfVerticesIn,
					vertexSizeIn,
					dataIn,
					_subMeshName))
				{
					return true;
				}
				else
				{
					return false;

				}
		}
		else
		{
			return false;
		}

	}



	bool SubMesh::CreateIndex(
			D3DFORMAT formatIn,
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfIndicesIn,
			BYTE* dataIn,
			std::string bufferName)
	{

		_numOfFaces	=static_cast<unsigned int>(numOfIndicesIn/3);
		if(data)
		{
			if(data->createIndexBuffer(
				formatIn,
				usageIn,
				poolIn,
				numOfIndicesIn,
				dataIn,
				_subMeshName))
			{
				_ifUseIndex	= true;
				return true;

			}
			else
			{
				return false;
			}


		}
		else
		{

			return false;
		}
	



	}

	size_t SubMesh::getNumOfFaces()const
	{
		return data->getNumOfFaces();

	}

	bool SubMesh::render()
	{
		_device->SetMaterial(&_material);
		if(_texture)
			_device->SetTexture(0,_texture);
		_device->SetVertexDeclaration(_vertexDecl);
		
		_device->SetStreamSource( 0, data->_vertexBuffer->getVertexBuffer(), 0,32);   

		_device->SetIndices(data->_indexBuffer->getIndexBufer());
		_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,static_cast<UINT>(_numOfVertices),0,static_cast<UINT>(_numOfFaces));
		
		return true;

	}



}