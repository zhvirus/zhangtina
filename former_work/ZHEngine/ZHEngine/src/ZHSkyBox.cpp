#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHSkyBox.h"
#include "ZHSingleton.h"
#include "ZHCamera.h"

namespace ZH
{

	SkyBox::SkyBox():
	m_device(NULL),
	m_boxSize(1.0f),
	m_camera(NULL)
	{
	
		m_camera	=	Singleton<Camera>::getSingletonPtr();
	}

	SkyBox::~SkyBox()
	{
	
	}

	bool SkyBox::intialize(LPDIRECT3DDEVICE9 _device, float boxSize,LPCTSTR up,LPCTSTR down,LPCTSTR left,LPCTSTR right,LPCTSTR front,LPCTSTR back)
	{
		m_boxSize	=	boxSize;
		m_device = _device;
		if(D3D_OK!=D3DXCreateTextureFromFile(m_device, up, &(m_texVector[0])))
			return false;
		if(D3D_OK!=D3DXCreateTextureFromFile(m_device, down, &(m_texVector[1])))
			return false;
		if(D3D_OK!=D3DXCreateTextureFromFile(m_device, left, &(m_texVector[2])))
			return false;
		if(D3D_OK!=D3DXCreateTextureFromFile(m_device, right, &(m_texVector[3])))
			return false;
		if(D3D_OK!=D3DXCreateTextureFromFile(m_device, front, &(m_texVector[4])))
			return false;
		if(D3D_OK!=D3DXCreateTextureFromFile(m_device, back, &(m_texVector[5])))
			return false;

		return _prepareBuffers();
	}

	bool SkyBox::_prepareBuffers()
	{
		//顶点格式说明
		D3DVERTEXELEMENT9 decl[]=
		{
			{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
			{0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
			D3DDECL_END()
		};

		m_device->CreateVertexDeclaration(decl,&vertexDecl);

		//准备正方块的vertex buffer-------------Up
		if( FAILED( m_device->CreateVertexBuffer( 
			6*5*sizeof(float),
			D3DUSAGE_WRITEONLY, 
			D3DFVF_XYZ|D3DFVF_TEX0,
			D3DPOOL_DEFAULT,
			&m_vtxBuffer[0], NULL ) ) )
		{
			ZH_ERR("Create vertex buffer failed!");
			return false;
		}

		void* vPointer = NULL;

		HRESULT hr = m_vtxBuffer[0]->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);

			float _dataUp[]={
			-m_boxSize/2.0f, m_boxSize/2.0f, -m_boxSize/2.0f, 0.0f,0.0f,
			-m_boxSize/2.0f, m_boxSize/2.0f,  m_boxSize/2.0f, 0.0f,1.0f,
			 m_boxSize/2.0f, m_boxSize/2.0f, -m_boxSize/2.0f, 1.0f,0.0f,
			 m_boxSize/2.0f, m_boxSize/2.0f, -m_boxSize/2.0f, 1.0f,0.0f,
			-m_boxSize/2.0f, m_boxSize/2.0f,  m_boxSize/2.0f, 0.0f,1.0f,
			 m_boxSize/2.0f, m_boxSize/2.0f,  m_boxSize/2.0f, 1.0f,1.0f
			};

			memcpy(vPointer, _dataUp, 6*5*sizeof(float));
		m_vtxBuffer[0]->Unlock();

		//准备正方块的vertex buffer-------------Down
		if( FAILED( m_device->CreateVertexBuffer( 
			6*5*sizeof(float),
			D3DUSAGE_WRITEONLY, 
			D3DFVF_XYZ|D3DFVF_TEX0,
			D3DPOOL_DEFAULT,
			&m_vtxBuffer[1], NULL ) ) )
		{
			ZH_ERR("Create vertex buffer failed!");
			return false;
		}

		hr = m_vtxBuffer[1]->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);

		float _dataDown[]={
			-m_boxSize/2.0f, -m_boxSize/2.0f,  m_boxSize/2.0f, 0.0f,0.0f,
			-m_boxSize/2.0f, -m_boxSize/2.0f,  -m_boxSize/2.0f, 0.0f,1.0f,
			m_boxSize/2.0f, -m_boxSize/2.0f, -m_boxSize/2.0f, 1.0f,1.0f,
			-m_boxSize/2.0f, -m_boxSize/2.0f,  m_boxSize/2.0f, 0.0f,0.0f,
			 m_boxSize/2.0f, -m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,1.0f,
			m_boxSize/2.0f, -m_boxSize/2.0f,  m_boxSize/2.0f, 1.0f,0.0f
		};

		memcpy(vPointer, _dataDown, 6*5*sizeof(float));
		m_vtxBuffer[1]->Unlock();

		//准备正方块的vertex buffer-------------Left
		if( FAILED( m_device->CreateVertexBuffer( 
			6*5*sizeof(float),
			D3DUSAGE_WRITEONLY, 
			D3DFVF_XYZ|D3DFVF_TEX0,
			D3DPOOL_DEFAULT,
			&m_vtxBuffer[2], NULL ) ) )
		{
			ZH_ERR("Create vertex buffer failed!");
			return false;
		}

		hr = m_vtxBuffer[2]->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);

		float _dataLeft[]={
			-m_boxSize/2.0f,  m_boxSize/2.0f,  -m_boxSize/2.0f, 0.0f,0.0f,
			-m_boxSize/2.0f, -m_boxSize/2.0f,  -m_boxSize/2.0f, 0.0f,1.0f,
			-m_boxSize/2.0f, -m_boxSize/2.0f,  m_boxSize/2.0f, 1.0f,1.0f,
			-m_boxSize/2.0f,  m_boxSize/2.0f,  -m_boxSize/2.0f, 0.0f,0.0f,
			-m_boxSize/2.0f, -m_boxSize/2.0f,   m_boxSize/2.0f, 1.0f,1.0f,
			-m_boxSize/2.0f,  m_boxSize/2.0f,  m_boxSize/2.0f, 1.0f,0.0f
		};

		memcpy(vPointer, _dataLeft, 6*5*sizeof(float));
		m_vtxBuffer[2]->Unlock();

		//准备正方块的vertex buffer-------------right
		if( FAILED( m_device->CreateVertexBuffer( 
			6*5*sizeof(float),
			D3DUSAGE_WRITEONLY, 
			D3DFVF_XYZ|D3DFVF_TEX0,
			D3DPOOL_DEFAULT,
			&m_vtxBuffer[3], NULL ) ) )
		{
			ZH_ERR("Create vertex buffer failed!");
			return false;
		}

		hr = m_vtxBuffer[3]->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);

		float _dataRight[]={
			 m_boxSize/2.0f,  m_boxSize/2.0f,   m_boxSize/2.0f, 0.0f,0.0f,
			 m_boxSize/2.0f, -m_boxSize/2.0f,   m_boxSize/2.0f, 0.0f,1.0f,
			 m_boxSize/2.0f,  m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,0.0f,
			m_boxSize/2.0f,  m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,0.0f,
			m_boxSize/2.0f, -m_boxSize/2.0f,   m_boxSize/2.0f, 0.0f,1.0f,
			 m_boxSize/2.0f,  -m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,1.0f
		};

		memcpy(vPointer, _dataRight, 6*5*sizeof(float));
		m_vtxBuffer[3]->Unlock();

		//准备正方块的vertex buffer-------------front
		if( FAILED( m_device->CreateVertexBuffer( 
			6*5*sizeof(float),
			D3DUSAGE_WRITEONLY, 
			D3DFVF_XYZ|D3DFVF_TEX0,
			D3DPOOL_DEFAULT,
			&m_vtxBuffer[4], NULL ) ) )
		{
			ZH_ERR("Create vertex buffer failed!");
			return false;
		}

		hr = m_vtxBuffer[4]->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);

		float _dataFront[]={
			m_boxSize/2.0f,  m_boxSize/2.0f,   -m_boxSize/2.0f, 0.0f,0.0f,
			m_boxSize/2.0f, -m_boxSize/2.0f,   -m_boxSize/2.0f, 0.0f,1.0f,
			-m_boxSize/2.0f,  m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,0.0f,
			-m_boxSize/2.0f,  m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,0.0f,
			m_boxSize/2.0f, -m_boxSize/2.0f,   -m_boxSize/2.0f, 0.0f,1.0f,
			-m_boxSize/2.0f,  -m_boxSize/2.0f,  -m_boxSize/2.0f, 1.0f,1.0f
		};

		memcpy(vPointer, _dataFront, 6*5*sizeof(float));
		m_vtxBuffer[4]->Unlock();

		//准备正方块的vertex buffer-------------back
		if( FAILED( m_device->CreateVertexBuffer( 
			6*5*sizeof(float),
			D3DUSAGE_WRITEONLY, 
			D3DFVF_XYZ|D3DFVF_TEX0,
			D3DPOOL_DEFAULT,
			&m_vtxBuffer[5], NULL ) ) )
		{
			ZH_ERR("Create vertex buffer failed!");
			return false;
		}

		hr = m_vtxBuffer[5]->Lock(0,0,(void**)(&vPointer),D3DLOCK_DISCARD);

		float _dataBack[]={
			-m_boxSize/2.0f,  m_boxSize/2.0f,    m_boxSize/2.0f, 0.0f,0.0f,
			-m_boxSize/2.0f, -m_boxSize/2.0f,    m_boxSize/2.0f, 0.0f,1.0f,
			m_boxSize/2.0f,  -m_boxSize/2.0f,   m_boxSize/2.0f, 1.0f,1.0f,
			-m_boxSize/2.0f,  m_boxSize/2.0f,   m_boxSize/2.0f, 0.0f,0.0f,
			m_boxSize/2.0f, -m_boxSize/2.0f,    m_boxSize/2.0f, 1.0f,1.0f,
			 m_boxSize/2.0f,   m_boxSize/2.0f,   m_boxSize/2.0f, 1.0f,0.0f
		};

		memcpy(vPointer, _dataBack, 6*5*sizeof(float));
		m_vtxBuffer[5]->Unlock();
		return true;
	}



	void SkyBox::render()
	{
		m_device->SetVertexShader(NULL);
		m_device->SetPixelShader(NULL);
		D3DXMATRIX	t_world, t_view, t_proj;
		D3DXMatrixTranslation(&t_world, m_camera->_pos.x,0.0f , m_camera->_pos.z);
		memcpy(&t_view,&(m_camera->getViewMatrix()),sizeof(D3DXMATRIX) );
		memcpy(&t_proj,&(m_camera->getProjMatrix()),sizeof(D3DXMATRIX) );	
		
		m_device->SetTransform(D3DTS_WORLD, &t_world);
		m_device->SetTransform(D3DTS_VIEW, &t_view);
		m_device->SetTransform(D3DTS_PROJECTION, &t_proj);
		

		m_device->SetVertexDeclaration(vertexDecl);

		for(int i=0;i<6;i++)
		{	
			m_device->SetStreamSource(0,m_vtxBuffer[i],0,20);
			m_device->SetTexture(0,m_texVector[i]);
			m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		}




	}


}