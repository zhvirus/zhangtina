#pragma once

#include "ZHRenderDecl.h"



namespace ZH
{
	class HeightData;
	class SkyBox;		

	//金字塔总层数,也就是ring的个数
	const int LEVEL	=	6;
	//最小纹理的大小
	const int MIN_SAMPLER	=	256;
	//每一层Ring的纹理尺寸
	const int RING_TEXTURE_SIZE	=	128;

	//小块的采样点数
	const int BLOCK_SIZE_SAMPLER	=	35;
	// 小方块的长宽尺寸
	const float BLOCK_SIZE_AREA	=	200.0f;

	//-------------
	//每一层Ring的长宽采样点数
	const int RING_SIZE_SAMPLER	 = BLOCK_SIZE_SAMPLER*4-3;	
	//每一层Ring的长宽尺寸
	const float RING_SIZE_AREA	=	BLOCK_SIZE_AREA * 4; 
	//每一个小格的长,宽
    const float GRID_SIZE_AREA = BLOCK_SIZE_AREA/(BLOCK_SIZE_SAMPLER-1);


class ZH_DLL Terrain
{

	public:
		explicit Terrain();
		~Terrain();
	
	private:

		//当前显示设备
		LPDIRECT3DDEVICE9 _device;
		
		//地形表面是否有地表纹理
		bool _hasTexture;
		
		//图像操作对象
		Image* _img;
		
		//地形范围
		float _width;
		float _height;

		//三角片数目
		int _triangleNum;

		//Camera
		Camera* _camera;

        //显示字体
        Font* _font; 
        char t_string[100];

		//data

		//Clipmap Stack
		//数据源,里面有所有高度场数据
		HeightData* _dataSource;
		//高度纹理
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
		//根据当前Camera的位置来更新Clipmap（ClipmapStack和ClipmapPymarid）
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


