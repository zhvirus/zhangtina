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
		std::string textureNameIn,		//subMesh��������
		const D3DMATERIAL9 mateIn,	//subMesh����
		Mesh* parentIn,						//����Mesh
		std::string subMeshNameIn,		//subMesh����
		LPDIRECT3DVERTEXDECLARATION9 vertexDeclIn,//��������
		D3DPRIMITIVETYPE primitiveTypeIn	//primitive ����
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
	//SubMesh ����
	std::string _subMeshName;

	//������Ŀ
	size_t _numOfVertices;
	size_t _numOfFaces;

	//����meshʹ�õ���������
	std::string _textureName;
	LPDIRECT3DTEXTURE9	_texture;

	//�Ƿ�ʹ��index
	bool _ifUseIndex;

	//����
	D3DMATERIAL9	_material;

	//��������ؽ�����
	bool isTextureLoaded;

	//��Mesh
	Mesh* _parentMesh;

	//����
	RenderData*  data;
	
	//�����ʽ����
	LPDIRECT3DVERTEXDECLARATION9 _vertexDecl;
	
	//primitive����
	D3DPRIMITIVETYPE	_primitiveType	;
		

	//�豸
	LPDIRECT3DDEVICE9 _device;


};



}