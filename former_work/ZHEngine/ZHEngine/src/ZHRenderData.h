#pragma once
#include "ZHRenderDecl.h"
#include "ZHCommonDefine.h"



//****************************************
//class name:		RenderData
//author:				Hui Zhang
//latest modified:	2007.4.11
//description:			vertex buffer,index buffer,primitive type
//****************************************


namespace ZH
{

	class ZH_DLL RenderData
	{
	public:
		explicit RenderData(			
			LPDIRECT3DVERTEXDECLARATION9 vertexDecl,
			D3DPRIMITIVETYPE	primitiveType	);
		~RenderData();
	
	public:
		bool createVertexBuffer(			
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfVerticesIn,
			unsigned int vertexSizeIn,
			BYTE* dataIn,
			std::string bufferName);

		bool createIndexBuffer(
			D3DFORMAT formatIn,
			DWORD usageIn,
			D3DPOOL poolIn,
			unsigned int numOfIndicesIn,
			BYTE* dataIn,
			std::string bufferName);

		inline size_t getNumOfFaces()const {return numOfFaces;}



		//primitive����
		D3DPRIMITIVETYPE 	_primitiveType;		
		VertexBuffer*		_vertexBuffer;
		IndexBuffer*		_indexBuffer;

	private:				
		//�����ʽ
		LPDIRECT3DVERTEXDECLARATION9 _vertexDeclara;
		
		//�Ƿ�ʹ��index buffer
		bool	useIndex;
	
		//�ܹ�����Ƭ��Ŀ
		size_t numOfFaces;

		//�豸
		LPDIRECT3DDEVICE9	_device;
		



	};




}