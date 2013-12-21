#pragma once

#include "ZHRenderDecl.h"
#include "ZHWindow.h"
#include "ZHFont.h"
#include "ZHDisplayAdapter.h"
#include "ZHVideoMode.h"

namespace ZH
{


//****************************************
//class name:		FPS
//author:				Hui Zhang
//latest modified:	2007.2.2
//description:			calculate FPS
//****************************************

class ZH_DLL FPS
{
public:
	ZHfloat lastTime;	//last time in second, counted from system started
	ZHfloat numOfFrame;	//num of frame rendered  between two calculation
	ZHfloat costTime;	//time cost between two calculation
	ZHfloat upTime;	//calculation is taken every upTime time;
	TCHAR fps_str[15];//current fps in char
public:
	FPS(ZHfloat updateTime	=	0.5f);

	void update();	//to calculate fps

};



//****************************************
//class name:		RenderSystem
//author:				Hui Zhang
//latest modified:	2007.2.2
//description:			
//****************************************

class ZH_DLL RenderSystem
{
public:
		explicit RenderSystem();
		~RenderSystem();
public:

//---------------------------------------------------------
//�������ڣ���ʼ��D3D��������Ϣѭ��
//
//---------------------------------------------------------
ZHResult initialize(	
					LPCTSTR windowTitle,//���ڱ���
					ZHRect window,//���ڳߴ��λ��
					bool isFullScreen,//�Ƿ�ȫ��
					SceneManager* _sceneManager);



	//��Ⱦ
	void present();
	//��ʼ֡
	void beginFrame();
	//����֡
	void endFrame();

	//��������任����
	void setWorldMatrix(const ZH::Matrix4& _world);
	//�����ӵ�任����
	void setViewMatrix(const ZH::Matrix4& _view);
	//����ͶӰ�任����
	void setProjectionMatrix(const ZH::Matrix4& _proj);
	//����FPS
	void updateFPS();
	//��ȾFPS
	void printFPS();
	//����
	void clear();

	void setVertexShader(IDirect3DVertexShader9* shader);

	inline LPDIRECT3DDEVICE9	getDevice(){return mpD3DDevice;}

	inline const D3DCAPS9& getDeviceCaps()const{return capacity;}
public:

	//������Ϣѭ��
	void mainLoop();
	//�����Դ
	ZHResult Cleanup();





private:
	//��ѯ��ǰϵͳ���õ���ʾ��������ÿ����ʾ������֧�ֵ���Ƶģʽ
	bool _captureAdapterAbility();
	//��ȡ��ǰ���ʵ���Ƶģʽ
	bool _captureCurVideoMode(bool ifFullScreen,ZHRect window);


private:

	ID3D11Device* mpD3DDevice;
	ID3D11DeviceContext* pd3dDeviceContext;
	IDXGIAdapter* pAdapter;
	
	//�豸����
	//D3DCAPS9 capacity;
	
	//����
	Font textRender;
	//FPS
	FPS m_fps;
	//��ʾ������
	std::vector<ZHDisplayAdapter*> _adapterList;
	//��ǰ���ʾģʽ
	VideoMode curVideoMode;

	//
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9   indexBuffer;
	

};

};
