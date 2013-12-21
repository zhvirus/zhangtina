#pragma once

#include "ZHRenderDecl.h"


namespace ZH
{

	class ZH_DLL SkyBox
	{
	public:
		SkyBox();
		~SkyBox();
		bool intialize(LPDIRECT3DDEVICE9 _device, float boxSize,LPCTSTR up,LPCTSTR down,LPCTSTR left,LPCTSTR right,LPCTSTR front,LPCTSTR back);
		void render();
	private:
		bool _prepareBuffers();
	private:
		LPDIRECT3DVERTEXBUFFER9 m_vtxBuffer[6];
		LPDIRECT3DTEXTURE9 m_texVector[6];
		LPDIRECT3DDEVICE9 m_device;
		Camera* m_camera;
		IDirect3DVertexDeclaration9* vertexDecl;
		float m_boxSize;



	};





}