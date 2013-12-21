#pragma once 

#include "ZHRenderDecl.h"

namespace ZH
{


//****************************************
//class name:		SubMesh
//author:				Hui Zhang
//latest modified:	2007.6.22
//description:			
//****************************************

class ZH_DLL SubMesh
{
public:
	SubMesh(
		std::string textureNameIn,		//subMesh纹理名字
		const D3DMATERIAL9 mateIn,	//subMesh材质
		Mesh* parentIn,						//所在Mesh
		std::string subMeshNameIn,		//subMesh名字
		LPDIRECT3DVERTEXDECLARATION9 vertexDeclIn,//顶点声明
		D3DPRIMITIVETYPE primitiveTypeIn	//primitive 类型
		);

	~SubMesh();
public:

	bool CreateVertex(
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfVerticesIn,
			unsigned int vertexSizeIn,
			BYTE* dataIn,
			std::string bufferName
		);

	bool CreateIndex(
			D3DFORMAT formatIn,
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfIndicesIn,
			BYTE* dataIn,
			std::string bufferName
		);

	inline bool ifUseIndex()const{return _ifUseIndex;}
	size_t getNumOfFaces()const;
	inline std::string getSubMeshName()const{return _subMeshName;}
	bool render();

private:
	//SubMesh 名字
	std::string _subMeshName;

	//顶点数目
	size_t _numOfVertices;
	size_t _numOfFaces;

	//该子mesh使用的纹理名字
	std::string _textureName;
	LPDIRECT3DTEXTURE9	_texture;

	//是否使用index
	bool _ifUseIndex;

	//材质
	D3DMATERIAL9	_material;

	//该纹理加载进来？
	bool isTextureLoaded;

	//父Mesh
	Mesh* _parentMesh;

	//数据
	RenderData*  data;
	
	//顶点格式声明
	LPDIRECT3DVERTEXDECLARATION9 _vertexDecl;
	
	//primitive类型
	D3DPRIMITIVETYPE	_primitiveType	;
		

	//设备
	LPDIRECT3DDEVICE9 _device;


};



}