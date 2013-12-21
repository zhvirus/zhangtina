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
		//返回subMesh数目
		inline int getSubMeshNum()const{return _numOfSubMesh;}
		inline bool ifD3DMesh()const{return ifD3dMesh;}
	public:
		//增加一个subMesh
		bool addSubMesh(SubMesh* subMeshIn);
		//从x文件初始化mesh
		bool LoadFromXFile(std::string xFileName);
		//绘制mesh
		void  render();

	private:
		SUBMESH_MAP _subMeshMap;
		SUBMESH_ITERATOR _iterator;
		

		//是否使用d3d的subMesh
		bool ifD3dMesh;
		LPD3DXMESH d3dMesh;
		LPDIRECT3DTEXTURE9* textures;
		D3DMATERIAL9* materials;

		//subMesh个数
		int _numOfSubMesh;
		//D3D 设备
		LPDIRECT3DDEVICE9 _device;
		Resource* _resource;
	};









}