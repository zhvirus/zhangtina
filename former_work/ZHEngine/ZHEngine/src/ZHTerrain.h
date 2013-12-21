#pragma once

#include "ZHRenderDecl.h"



namespace ZH
{
	class HeightData;
	class SkyBox;		

	//�������ܲ���,Ҳ����ring�ĸ���
	const int LEVEL	=	6;
	//��С����Ĵ�С
	const int MIN_SAMPLER	=	256;
	//ÿһ��Ring������ߴ�
	const int RING_TEXTURE_SIZE	=	128;

	//С��Ĳ�������
	const int BLOCK_SIZE_SAMPLER	=	35;
	// С����ĳ���ߴ�
	const float BLOCK_SIZE_AREA	=	200.0f;

	//-------------
	//ÿһ��Ring�ĳ����������
	const int RING_SIZE_SAMPLER	 = BLOCK_SIZE_SAMPLER*4-3;	
	//ÿһ��Ring�ĳ���ߴ�
	const float RING_SIZE_AREA	=	BLOCK_SIZE_AREA * 4; 
	//ÿһ��С��ĳ�,��
    const float GRID_SIZE_AREA = BLOCK_SIZE_AREA/(BLOCK_SIZE_SAMPLER-1);


class ZH_DLL Terrain
{

	public:
		explicit Terrain();
		~Terrain();
	
	private:

		//��ǰ��ʾ�豸
		LPDIRECT3DDEVICE9 _device;
		
		//���α����Ƿ��еر�����
		bool _hasTexture;
		
		//ͼ���������
		Image* _img;
		
		//���η�Χ
		float _width;
		float _height;

		//����Ƭ��Ŀ
		int _triangleNum;

		//Camera
		Camera* _camera;

        //��ʾ����
        Font* _font; 
        char t_string[100];

		//data

		//Clipmap Stack
		//����Դ,���������и߶ȳ�����
		HeightData* _dataSource;
		//�߶�����
		LPDIRECT3DTEXTURE9 _ClipmapStack[LEVEL];
		int _SizeOfClipmapStack;

		//Matrix
		D3DXMATRIX vMatrix,pMatrix,vpMatrix;
		
		//vertex shader
		LPD3DXCONSTANTTABLE _vTable;

		IDirect3DVertexShader9* vertexShader;
		D3DXHANDLE h_ModelViewProjMatrix,h_HeightTexture;
		D3DXCONSTANT_DESC HeightTex_DESC;
		IDirect3DVertexDeclaration9* vertexDecl;

        D3DXHANDLE h_blockTransAndScale;
        D3DXHANDLE h_texCoordClamp;
		D3DXHANDLE h_camera;
		D3DXHANDLE h_ringSamplerNumber;

		//pixel shader
		LPD3DXCONSTANTTABLE _pTable;

		IDirect3DPixelShader9* pixelShader;
		D3DXHANDLE h_colorMap;

		LPDIRECT3DTEXTURE9 _colorTexture;


		//block vertex&index buffer
		LPDIRECT3DVERTEXBUFFER9 _blockVertexBuf;
		LPDIRECT3DINDEXBUFFER9 _blockIndexBuf;

		//sky box
		SkyBox* _skyBox;

	public:
		bool initialize();
		void render();
		void estabilsh();
		void updateData();

	private:
		//���ݵ�ǰCamera��λ��������Clipmap��ClipmapStack��ClipmapPymarid��
		void _updateHeightTextures();
		bool _prepareShaders();
		bool _createEmptyTextures();
		bool _prepareBuffers();
		bool _updateTexturesFromDataSource();

		//bool _prepareHeightTextures();
		bool _fillHeightData(LPDIRECT3DTEXTURE9 texForFilled,int texSize,float* heightDataSource,int dataNumber);
        bool __calculateBlockTransTexClamp(int blockIndex,int level,D3DXVECTOR4& _revVector,D3DXVECTOR4& _TexClamp);
        bool __calculateBlockTransTexClamp2(int blockIndex,D3DXVECTOR4& _revVector,D3DXVECTOR4& _TexClamp);
		bool _prepareOtherData();
};

}


