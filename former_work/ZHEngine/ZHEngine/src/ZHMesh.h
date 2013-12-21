#pragma once

#include "ZHRenderDecl.h"

namespace ZH
{


	class ZH_DLL Mesh
	{
		typedef std::map<std::string,SubMesh*> SUBMESH_MAP;
		typedef std::map<std::string,SubMesh*>::iterator SUBMESH_ITERATOR;

		public:	
			explicit Mesh();
			//Mesh(const Mesh& inMesh);
			~Mesh();




	public:
		//����subMesh��Ŀ
		inline int getSubMeshNum()const{return _numOfSubMesh;}
		inline bool ifD3DMesh()const{return ifD3dMesh;}
	public:
		//����һ��subMesh
		bool addSubMesh(SubMesh* subMeshIn);
		//��x�ļ���ʼ��mesh
		bool LoadFromXFile(std::string xFileName);
		//����mesh
		void  render();

	private:
		SUBMESH_MAP _subMeshMap;
		SUBMESH_ITERATOR _iterator;
		

		//�Ƿ�ʹ��d3d��subMesh
		bool ifD3dMesh;
		LPD3DXMESH d3dMesh;
		LPDIRECT3DTEXTURE9* textures;
		D3DMATERIAL9* materials;

		//subMesh����
		int _numOfSubMesh;
		//D3D �豸
		LPDIRECT3DDEVICE9 _device;
		Resource* _resource;
	};









}