#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHMesh.h"
#include "ZHTerrain.h"
#include "ZHImage.h"
#include "ZHSingleton.h"
#include "ZHRenderSystem.h"
#include "ZHSubMesh.h"
#include "ZHCamera.h"
#include "ZHFont.h"
#include "ZHHeightData.h"
#include "ZHSkyBox.h"

namespace ZH
{

	Terrain::Terrain():
		_hasTexture(false),
		_device(NULL),
		_img(NULL),
		_width(0.0f),
		_height(0.0f),
		_triangleNum(0),
		_camera(NULL),
		_font(NULL),
		_dataSource(NULL),
		_SizeOfClipmapStack(0),
		_vTable(NULL),
		vertexShader(NULL),
		h_ModelViewProjMatrix(NULL),
		h_HeightTexture(NULL),
		h_blockTransAndScale(NULL),
		h_texCoordClamp(NULL),
		h_camera(NULL),
		vertexDecl(NULL),

		_pTable(NULL),
		pixelShader(NULL),
		h_colorMap(NULL),

		_colorTexture(NULL),
		_blockVertexBuf(0),
		_blockIndexBuf(0),

		_skyBox(0)
	{

		//获取图像操作指针
		_img	=	Singleton<Image>::getSingletonPtr();

		//查询当前显示设备
		_device	=	Singleton<RenderSystem>::getSingletonPtr()->getDevice();

		//获取全局唯一摄像机
		_camera	=	Singleton<Camera>::getSingletonPtr();
		memcpy(&pMatrix,&(_camera->getProjMatrix()),sizeof(pMatrix));

        //字体显示
        _font = Singleton<Font>::getSingletonPtr();
        if(_font&&_device)
        {
            _font->InitFont(_device);
        }

		_dataSource = Singleton<HeightData>::getSingletonPtr();
		_skyBox	=	Singleton<SkyBox>::getSingletonPtr();

		_SizeOfClipmapStack	=	LEVEL;
	}
	//设置渲染环境和变换矩阵
	void Terrain::estabilsh()
	{


	  //D3DXVECTOR3 vEYE	=	D3DXVECTOR3(0.0f,400.0f,-700.0f);
	  //D3DXVECTOR3 vLOOKAT	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
	  //D3DXVECTOR3 vUP(0.0f,0.0f,1.0f);

	  //D3DXMatrixLookAtLH(&vMatrix,&vEYE,&vLOOKAT,&vUP);
	  //_device->SetTransform(D3DTS_VIEW,&vMatrix);

		//set projection matrix

	  //_device->SetTransform(D3DTS_PROJECTION,&pMatrix);

	  //D3DXMatrixOrthoLH(&pMatrix,500,500,1.0f,1000.0f);
	 //_device->SetTransform(D3DTS_PROJECTION,&pMatrix);


		_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		//_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
        //  _device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_PHONG);
		// Turn off D3D lighting, since we are providing our own vertex colors
		_device->SetRenderState( D3DRS_LIGHTING, FALSE );

	}



	Terrain::~Terrain()
	{
		//释放纹理

		for(int i=0;i<_SizeOfClipmapStack;i++)
		{
			SAFE_RELEASE(_ClipmapStack[i]);
		}

		SAFE_DELETE(_dataSource);
		SAFE_DELETE(_skyBox);

	}

	bool Terrain::initialize()
	{



		//准备地形数据
		//(开始的数据块大小,一共几个层次,u方向的偏移,v方向的偏移,每一层的数据块大小)
		bool result = _dataSource->initialize(MIN_SAMPLER,LEVEL,0,0,RING_TEXTURE_SIZE);
		if(result == false)
		{
			std::cout<<"initialize data failed!"<<std::endl;
			return false;
		}

		//准备shader程序
		if(!_prepareShaders())
		{
			std::cout<<"initialize shader failed!"<<std::endl;
			return false;
		}

		//准备好Block12的顶点buffer和 索引 buffer
		if(!_prepareBuffers())
		{
			std::cout<<"initialize buffer failed!"<<std::endl;
			return false;
		}

		//创建空的纹理
		if(!_createEmptyTextures())
		{
			std::cout<<"build textures failed!"<<std::endl;
			return false;
		}

		//填充纹理数据
		if(!_updateTexturesFromDataSource())
		{
			std::cout<<"filling textures failed!"<<std::endl;
			return false;
		}

		//准备其他相关数据
		_prepareOtherData();
		//根据视点位置更新高度数据
		_updateHeightTextures();

		_width	=	BLOCK_SIZE_AREA * pow(2.0f,_SizeOfClipmapStack-2);
		_height	=	_width;
		
		if(!_skyBox->intialize(
			_device, 
			_width,
			"res\\top.dds",
			"res\\bottom.dds",
			"res\\left.dds",
			"res\\right.dds",
			"res\\front.dds",
			"res\\back.dds"
			))
		{
			std::cout<<"Initialize skyBox failed!"<<std::endl;
			return false;
		}

		return true;
	}

	bool Terrain::_updateTexturesFromDataSource()
	{
		int pitch;	
		float* dstData;
		float* srcData;
		int texS;

		for(int i=0;i<_SizeOfClipmapStack;i++)
		{

			//更新纹理内容
			D3DLOCKED_RECT lockedRect;

			_ClipmapStack[i]->LockRect(0,&lockedRect,0,0);

			pitch	=	static_cast<int>(lockedRect.Pitch/4.0f);
			dstData	=	static_cast<float*>(lockedRect.pBits);
			srcData = _dataSource->m_dataCache[i]->_data;
			texS	=	_dataSource->m_dataCache[i]->_blockSize;

			for(int j=0;j<texS;++j)
			{
				memcpy(dstData+j*pitch,srcData+j*texS,texS*4);
			}

			_ClipmapStack[i]->UnlockRect(0);

		}

		return true;
	}

	bool Terrain::_createEmptyTextures()
	{
		_SizeOfClipmapStack	=	_dataSource->m_stackTexNum;
	
		for(int i =0; i< (_dataSource->m_stackTexNum);i++)
		{
			//创建一副空纹理
			if((D3D_OK!=D3DXCreateTexture(_device,RING_TEXTURE_SIZE,RING_TEXTURE_SIZE,1,D3DUSAGE_DYNAMIC,D3DFMT_R32F,D3DPOOL_DEFAULT,&_ClipmapStack[i])))
			{
				return false;
			}



		}
		
		return true;
	}

	bool Terrain::_prepareOtherData()
	{
		D3DXCreateTextureFromFile(_device,"res\\color3.bmp",&_colorTexture);

		return true;
	}


	bool Terrain::_prepareShaders()
	{
		HRESULT hr	=	0;

		//准备vertex shader程序
		ID3DXBuffer* shader	=	NULL;
		ID3DXBuffer* errBuffer	=	NULL;
		
		//准备 vertex shader
		hr	=	D3DXCompileShaderFromFile(
				"res\\vertex.hlsl",
				0,
				0,
				"main",
				"vs_3_0",
				D3DXSHADER_DEBUG,
				&shader,
				&errBuffer,
				&_vTable
				);

		if(errBuffer)
		{
			//MessageBox(0,static_cast<char*>(errBuffer->GetBufferPointer()),0,0);
			std::cout<<static_cast<char*>(errBuffer->GetBufferPointer())<<std::endl;
			errBuffer->Release();
		
		}

		if(FAILED(hr))
		{
			std::cout<<"Vertex Shader compile failed!"<<std::endl;
			return false;
		}

		hr	=	_device->CreateVertexShader(
			static_cast<DWORD*>(shader->GetBufferPointer()),
			&vertexShader);
		
		if(FAILED(hr))
		{
			std::cout<<"Create Vertex Shader Failed!"<<std::endl;	
			return false;
		}
		else
		{
			shader->Release();
		}

		h_ModelViewProjMatrix	=	_vTable->GetConstantByName(0,"modelViewProj");
		h_HeightTexture	=	_vTable->GetConstantByName(0,"heightMap");
        h_blockTransAndScale    = _vTable->GetConstantByName(0,"blockTransAndScale");
        h_texCoordClamp           = _vTable->GetConstantByName(0,"texCoordClamp");
        h_camera	=	_vTable->GetConstantByName(0,"camera");
		h_ringSamplerNumber	=	_vTable->GetConstantByName(0,"Ring_Sampler_Number");

		UINT count;
		_vTable->GetConstantDesc(h_HeightTexture,&HeightTex_DESC,&count);

		_vTable->SetDefaults(_device);

		//------------------------------------------------------------------------------------------------
		//准备pixel shader程序
		shader	=	NULL;
		errBuffer	=	NULL;

		//准备 vertex shader
		hr	=	D3DXCompileShaderFromFile(
			"res\\pixel.hlsl",
			0,
			0,
			"main",
			"ps_3_0",
			D3DXSHADER_DEBUG,
			&shader,
			&errBuffer,
			&_pTable
			);

		if(errBuffer)
		{
			//MessageBox(0,static_cast<char*>(errBuffer->GetBufferPointer()),0,0);
			std::cout<<static_cast<char*>(errBuffer->GetBufferPointer())<<std::endl;
			errBuffer->Release();

		}

		if(FAILED(hr))
		{
			std::cout<<"Pixel Shader compile failed!"<<std::endl;
			return false;
		}

		hr	=	_device->CreatePixelShader(
			static_cast<DWORD*>(shader->GetBufferPointer()),
			&pixelShader);

		if(FAILED(hr))
		{
			std::cout<<"Create Pixel Shader Failed!"<<std::endl;	
			return false;
		}
		else
		{
			shader->Release();
		}

		h_colorMap	=	_pTable->GetConstantByName(0,"colorMap");

	return true;

	}


	bool Terrain::_prepareBuffers()
	{
        //*******************************************************************//
        //准备12个小块要用到的vertex buffer 和 index buffer
        //*******************************************************************//
		//顶点格式说明
		D3DVERTEXELEMENT9 decl[]=
		{
			{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
			{0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
			D3DDECL_END()

		};

		_device->CreateVertexDeclaration(decl,&vertexDecl);



		//准备正方块的vertex buffer
		int blockGrid		=	BLOCK_SIZE_SAMPLER;
		float fieldSize		=	BLOCK_SIZE_AREA;
		float GridSize	=	GRID_SIZE_AREA;

		if( FAILED( _device->CreateVertexBuffer( 
																blockGrid*blockGrid*5*sizeof(float),
																D3DUSAGE_WRITEONLY, 
																D3DFVF_XYZ|D3DFVF_TEX0,
																D3DPOOL_DEFAULT,
																&_blockVertexBuf, NULL ) ) )
				{
					ZH_ERR("Create vertex buffer failed!");
					
					return false;
				}

		void* vPointer = NULL;

		HRESULT hr = _blockVertexBuf->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);
		
		int index	=	0;
		for(int i=0;i<blockGrid;i++)
			for(int j=0;j<blockGrid;j++)
			{
				index	=	(i*blockGrid+j)*5;
				*((float*)(vPointer)+index)		= j*GridSize - fieldSize/2.0f;
				*((float*)(vPointer)+index+1)	= 0.0f;
				*((float*)(vPointer)+index+2)	= i*GridSize - fieldSize/2.0f;
				*((float*)(vPointer)+index+3)	= static_cast<float>(j)/static_cast<float>(blockGrid-1);
				*((float*)(vPointer)+index+4)	= static_cast<float>(i)/static_cast<float>(blockGrid-1);

			}

		
		_blockVertexBuf->Unlock();

		//准备正方块的index buffer
		if( FAILED( _device->CreateIndexBuffer( 
																(blockGrid-1)*(blockGrid-1)*2*3*sizeof(int),
																D3DUSAGE_WRITEONLY, 
																D3DFMT_INDEX32,
																D3DPOOL_DEFAULT,
																&_blockIndexBuf, NULL ) ) )
				{
					ZH_ERR("Create vertex buffer failed!");
					
					return false;
				}

		void* iPointer = NULL;

		hr = _blockIndexBuf->Lock(0,0,(void**)(&iPointer),D3DLOCK_DISCARD);
		

		int leftUp	=	0;
		for(int i=0;i<blockGrid-1;i++)
			for(int j=0;j<blockGrid-1;j++)
			{
				index	=	(i*(blockGrid-1)+j)*6;
				leftUp	=	i*blockGrid+j;
				*((int*)(iPointer)+index)		= leftUp;
				*((int*)(iPointer)+index+1)	= leftUp+blockGrid;
				*((int*)(iPointer)+index+2)	= leftUp+blockGrid+1;
				*((int*)(iPointer)+index+3)	= leftUp;
				*((int*)(iPointer)+index+4)	= leftUp+blockGrid+1;
				*((int*)(iPointer)+index+5)	= leftUp+1;

			}

		
		_blockIndexBuf->Unlock();



		return true;
	}

	void Terrain::_updateHeightTextures()
	{
		float camX	=	_camera->_pos.x/GRID_SIZE_AREA;
		float camZ	=	_camera->_pos.z/GRID_SIZE_AREA;
		char ooo[100];
		sprintf(ooo,"%f, %f",camX,camZ);
		ZH::ZHRect _rect(0,20,500,100);
		_font->Render(ooo,_rect);
		_dataSource->updateStackData(camX,camZ);
		_updateTexturesFromDataSource();


	}


		bool Terrain::_fillHeightData(LPDIRECT3DTEXTURE9 texForFilled,int texSize,float* heightDataSource,int dataNumber)
		{
			if(texForFilled==NULL||texSize*texSize!=dataNumber||heightDataSource==NULL)
				return false;


			//更新纹理内容
			D3DLOCKED_RECT lockedRect;

			texForFilled->LockRect(0,&lockedRect,0,0);

			int pitch	=	lockedRect.Pitch;
			int pitch_float	=	static_cast<int>(pitch/4);
			float* data	=	static_cast<float*>(lockedRect.pBits);

			for(int i=0;i<texSize;++i)
				{
					memcpy(data+i*pitch_float,heightDataSource+texSize*i,texSize*4);
				}

				texForFilled->UnlockRect(0);
			
				return true;
		}
  
    //计算12个小块，每块的偏移值和缩放值
    bool Terrain::__calculateBlockTransTexClamp(int blockIndex,int level,D3DXVECTOR4& _revVector,D3DXVECTOR4& _TexClamp)
    {

         _revVector.w = static_cast<float>(0); 
         float    t_factor = pow(2.0f,level-1);
        
         switch (blockIndex)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            {

                _revVector.x  = BLOCK_SIZE_AREA*(-1.5f+blockIndex) * t_factor;
                _revVector.y  = -BLOCK_SIZE_AREA*1.5f * t_factor;
                _revVector.z  = static_cast<float>(level);
               // _revVector.w = MIN_BLOCKFIELD*pow(2.0f,_revVector.z+1.0f);
               _TexClamp.x  =   0.25f * blockIndex;
               _TexClamp.y  =   0.0f;
               _TexClamp.z  =   _TexClamp.x + 0.25f;
               _TexClamp.w =   0.25f;
                break;
            }
        case 8:
        case 9:
        case 10:
        case 11:
            {
                _revVector.x  = BLOCK_SIZE_AREA*(-1.5f+blockIndex-8) * t_factor;
                _revVector.y  = BLOCK_SIZE_AREA*1.5f * t_factor;
                _revVector.z  = static_cast<float>(level);
               // _revVector.w = MIN_BLOCKFIELD*pow(2.0f,_revVector.z+1.0f);
               _TexClamp.x  =   0.25f * (blockIndex-8);
               _TexClamp.y  =   0.75f;
               _TexClamp.z  =   _TexClamp.x + 0.25f;
               _TexClamp.w =   1.0f;
                break;
            }
        case 4:
            {
                _revVector.x  = BLOCK_SIZE_AREA*(-1.5f) * t_factor;
                _revVector.y  = -BLOCK_SIZE_AREA*0.5f * t_factor;
                _revVector.z  = static_cast<float>(level);
              //  _revVector.w = MIN_BLOCKFIELD*pow(2.0f,_revVector.z+1.0f);
               _TexClamp.x  =   0.0f;
               _TexClamp.y  =   0.25f;
               _TexClamp.z  =   0.25f;
               _TexClamp.w =   0.5f;
                break;
            }
        case 5:
            {
                _revVector.x  = BLOCK_SIZE_AREA*(1.5f) * t_factor;
                _revVector.y  = -BLOCK_SIZE_AREA*0.5f * t_factor;
                _revVector.z  = static_cast<float>(level);
               // _revVector.w = MIN_BLOCKFIELD*pow(2.0f,_revVector.z+1.0f);
               _TexClamp.x  =   0.75f;
               _TexClamp.y  =   0.25f;
               _TexClamp.z  =   1.0f;
               _TexClamp.w =   0.5f;
                break;
            }
        case 6:
            {
                _revVector.x  = BLOCK_SIZE_AREA*(-1.5f) * t_factor;
                _revVector.y  = BLOCK_SIZE_AREA*0.5f * t_factor;
                _revVector.z  = static_cast<float>(level);
              //  _revVector.w = MIN_BLOCKFIELD*pow(2.0f,_revVector.z+1.0f);
               _TexClamp.x  =   0.0f;
               _TexClamp.y  =   0.5f;
               _TexClamp.z  =   0.25f;
               _TexClamp.w =   0.75f;
                break;
            }
        case 7:
            {
                _revVector.x  = BLOCK_SIZE_AREA*(1.5f) * t_factor;
                _revVector.y  = BLOCK_SIZE_AREA*0.5f * t_factor;
                _revVector.z  = static_cast<float>(level);
               // _revVector.w = MIN_BLOCKFIELD*pow(2.0f,_revVector.z+1.0f);
               _TexClamp.x  =   0.75f;
               _TexClamp.y  =   0.5f;
               _TexClamp.z  =   1.0f;
               _TexClamp.w =   0.75f;
                break;
            }
        default:
            return false;

        }
        return true;
    }

    bool Terrain::__calculateBlockTransTexClamp2(int blockIndex,D3DXVECTOR4& _revVector,D3DXVECTOR4& _TexClamp)
    {
      _revVector.z  = static_cast<float>(0);    
      _revVector.w = static_cast<float>(0);

      switch(blockIndex)
        {
        case 0:
            {   
                _revVector.x  = -0.5f*BLOCK_SIZE_AREA;
                _revVector.y  = -0.5f*BLOCK_SIZE_AREA;
                _TexClamp.x  = 0.0f;
                _TexClamp.y  = 0.0f;
                _TexClamp.z  = 0.5f;
                _TexClamp.w = 0.5f;
                break;
            }
        case 1:
            {
                _revVector.x  = 0.5f*BLOCK_SIZE_AREA;
                _revVector.y  = -0.5f*BLOCK_SIZE_AREA;
                _TexClamp.x  = 0.5f;
                _TexClamp.y  = 0.0f;
                _TexClamp.z  = 1.0f;
                _TexClamp.w = 0.5f; 
                break;
            }
        case 2:
            {
                _revVector.x  = -0.5f*BLOCK_SIZE_AREA;
                _revVector.y  = 0.5f*BLOCK_SIZE_AREA;
                _TexClamp.x  = 0.0f;
                _TexClamp.y  = 0.5f;
                _TexClamp.z  = 0.5f;
                _TexClamp.w = 1.0f;
                break;
            }
        case 3:
            {
                _revVector.x  = 0.5f*BLOCK_SIZE_AREA;
                _revVector.y  = 0.5f*BLOCK_SIZE_AREA;
                _TexClamp.x  = 0.5f;
                _TexClamp.y  = 0.5f;
                _TexClamp.z  = 1.0f;
                _TexClamp.w = 1.0f;
                break;
            }
        default:
            return false;
        };

        return true;
    }

	void Terrain::updateData()
	{
        //根据视点位置更新高度数据
		_updateHeightTextures();

	}

	void Terrain::render()
	{	
		//根据视点位置更新高度数据
		_updateHeightTextures();
		_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		_device->SetVertexShader(vertexShader);
		_device->SetPixelShader(pixelShader);

		_device->SetTexture(0,_colorTexture);
		D3DXVECTOR4 _cam(_camera->_pos,1);
		_vTable->SetVector(_device,h_camera,&_cam);
		_vTable->SetFloat(_device,h_ringSamplerNumber,RING_SIZE_SAMPLER);

		memcpy(&vpMatrix,&(_camera->_v),sizeof(vpMatrix));
		vpMatrix =vpMatrix*pMatrix;
		_vTable->SetMatrix(_device,h_ModelViewProjMatrix,&vpMatrix);		
		_device->SetSamplerState(HeightTex_DESC.RegisterIndex, D3DSAMP_MAGFILTER, 1);
		_device->SetSamplerState(HeightTex_DESC.RegisterIndex, D3DSAMP_MINFILTER, 1);
		_device->SetSamplerState(HeightTex_DESC.RegisterIndex, D3DSAMP_MIPFILTER, 1);

		_device->SetStreamSource(0,_blockVertexBuf,0,20);
		_device->SetVertexDeclaration(vertexDecl);
		_device->SetIndices(_blockIndexBuf);

        //************************************************************************************
        //Draw center region
        //************************************************************************************
        _device->SetTexture(D3DVERTEXTEXTURESAMPLER0,_ClipmapStack[0]);

        D3DXVECTOR4 t_TransScale;//x,y  for x,z translation; z for scale;w is not used
        D3DXVECTOR4 t_texCoordClamp;     

		//包围球中心 和 半径
		D3DXVECTOR3	_center;
		float _r	= 0.0f;
		int block_number = 0;


        for(int i_blockNumber=0;i_blockNumber<4;++i_blockNumber)
        {            
            __calculateBlockTransTexClamp2(i_blockNumber,t_TransScale,t_texCoordClamp);
            
			if(_camera->ifVisible(D3DXVECTOR3(t_TransScale.x + _camera->_pos.x, 0, t_TransScale.y + _camera->_pos.z ), BLOCK_SIZE_AREA*0.3f))
			{
				_vTable->SetVector(_device,h_blockTransAndScale,&t_TransScale);
				_vTable->SetVector(_device,h_texCoordClamp,&t_texCoordClamp);
				_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,BLOCK_SIZE_SAMPLER*BLOCK_SIZE_SAMPLER,0,(BLOCK_SIZE_SAMPLER-1)*(BLOCK_SIZE_SAMPLER-1)*2);
				block_number++;
			}

		}


        //************************************************************************************
        //Draw RINGs 
        //************************************************************************************
  

       for(int _level  =  1;_level<_SizeOfClipmapStack;++_level) 
       {
            for(int i_blockNumber=0;i_blockNumber<12;++i_blockNumber)
			{ 
				__calculateBlockTransTexClamp(i_blockNumber,_level,t_TransScale,t_texCoordClamp);

				if(_camera->ifVisible(D3DXVECTOR3(t_TransScale.x + _camera->_pos.x, 0, t_TransScale.y + _camera->_pos.z), BLOCK_SIZE_AREA*0.3f*pow(2,t_TransScale.z -1)))
				{     
					_vTable->SetVector(_device,h_blockTransAndScale,&t_TransScale);
					_vTable->SetVector(_device,h_texCoordClamp,&t_texCoordClamp);
					_device->SetTexture(D3DVERTEXTEXTURESAMPLER0,_ClipmapStack[_level]);
		            
					//_device->SetTexture(D3DVERTEXTEXTURESAMPLER0,_ClipmapPyramid[1]);	
					_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,BLOCK_SIZE_SAMPLER*BLOCK_SIZE_SAMPLER,0,(BLOCK_SIZE_SAMPLER-1)*(BLOCK_SIZE_SAMPLER-1)*2);
					block_number++;
				}

			}

       }
        
      _triangleNum =  (BLOCK_SIZE_SAMPLER-1)*(BLOCK_SIZE_SAMPLER-1)*2*(4+12*_SizeOfClipmapStack);
       sprintf(t_string,"rendered blocks: %d",block_number);

	   //_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	   //_skyBox->render();
       _font->Render(t_string,ZHRect(0,40,300,300),0xffffffff);
	}





}
