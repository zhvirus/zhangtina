#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHMesh.h"
#include "ZHSubMesh.h"
#include "ZHSingleton.h"
#include "ZHRenderSystem.h"
#include "ZHResource.h"


namespace ZH
{


	Mesh::Mesh():
		ifD3dMesh(false),
		_numOfSubMesh(0),
		d3dMesh(NULL),
		textures(NULL),
		materials(NULL),
		_resource(NULL)
	{
		//��ѯ��ǰ��ʾ�豸
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();

		_resource	=	Singleton<Resource>::getSingletonPtr();
	}

	//Mesh::Mesh(const Mesh& inMesh)
	//{
	//	//ifD3dMesh	=	


	//}


	Mesh::~Mesh()
	{

		for(_iterator=_subMeshMap.begin();_iterator!=_subMeshMap.end();_iterator++)
		{
			SAFE_DELETE((*_iterator).second);


		}

		_subMeshMap.clear();
		_numOfSubMesh	=	0;

		if(d3dMesh!=NULL)
		{
			d3dMesh->Release();
		}
		
		

	}

	bool Mesh::addSubMesh(ZH::SubMesh *subMeshIn)
	{
		if(subMeshIn==NULL)
			return false;

		_subMeshMap.insert(std::make_pair(subMeshIn->getSubMeshName(),subMeshIn));
		_numOfSubMesh	=	static_cast<unsigned int>(_subMeshMap.size());

		return true;
	}


	bool Mesh::LoadFromXFile(std::string xFileName)
	{
		LPD3DXBUFFER pD3DMtrBuffer;
		DWORD materialNum;
		

		if(D3D_OK!=D3DXLoadMeshFromX(
					xFileName.c_str(),
					D3DXMESH_SYSTEMMEM,
					_device,
					NULL,
					&pD3DMtrBuffer,
					NULL,
					&materialNum,
					&d3dMesh))
		{
			
			std::cout<<"X file "<<xFileName<<" not found!"<<std::endl;

			return false;
	
		}
		
		D3DXMATERIAL* d3dMaterials	=	(D3DXMATERIAL*)(pD3DMtrBuffer->GetBufferPointer());
		
		textures		=	new LPDIRECT3DTEXTURE9[materialNum];
		materials	=	new D3DMATERIAL9[materialNum];

		for(DWORD i=0;i<materialNum;i++)
		{
			//���ò���
			materials[i]	=	d3dMaterials[i].MatD3D;
			
			//��������
			materials[i].Ambient	=	materials[i].Diffuse;

			textures[i]	=	NULL;

			//�����ļ���Ϊ�գ�
			if(d3dMaterials[i].pTextureFilename!=NULL)
			{
				//�����Ѿ�����Դ������������ڣ�
				if(_resource->_textureManager->getTextureByName(d3dMaterials[i].pTextureFilename,textures[i])==false)
				{
					
						//���ļ��´�������
						if(
							FAILED(D3DXCreateTextureFromFile(_device,
							d3dMaterials[i].pTextureFilename,&textures[i]
											)
										)
							)
						{

							std::cout<<"Texture file "<<d3dMaterials[i].pTextureFilename<<" not found!"<<std::endl;

						}//end if ,create texture successfully
					
					//���´��������������Դ������
					_resource->_textureManager->addTexture(d3dMaterials[i].pTextureFilename,textures[i]);



				}//end if ,texture not found in resource
			}//end if ,
		}

		pD3DMtrBuffer->Release();
		
		ifD3dMesh	=	true;
		_numOfSubMesh	=	materialNum;

		


		return true;
	}


	void  Mesh::render()
	{
		//�����d3d��mesh
		if(ifD3dMesh)
		{
			for(int i	= 0;i<_numOfSubMesh;i++)
			{	
				_device->SetMaterial(&materials[i]);
				_device->SetTexture(0,textures[i]);
				d3dMesh->DrawSubset(i);
			}

		}
		else
		{
			for(_iterator=_subMeshMap.begin();_iterator!=_subMeshMap.end();_iterator++)
			{
				(*_iterator).second->render();


			}


		}






	}

	

}