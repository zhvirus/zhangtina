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
//建立窗口，初始化D3D，进入消息循环
//
//---------------------------------------------------------
ZHResult initialize(	
					LPCTSTR windowTitle,//窗口标题
					ZHRect window,//窗口尺寸和位置
					bool isFullScreen,//是否全屏
					SceneManager* _sceneManager);



	//渲染
	void present();
	//开始帧
	void beginFrame();
	//结束帧
	void endFrame();

	//设置世界变换矩阵
	void setWorldMatrix(const ZH::Matrix4& _world);
	//设置视点变换矩阵
	void setViewMatrix(const ZH::Matrix4& _view);
	//设置投影变换矩阵
	void setProjectionMatrix(const ZH::Matrix4& _proj);
	//计算FPS
	void updateFPS();
	//渲染FPS
	void printFPS();
	//清屏
	void clear();

	void setVertexShader(IDirect3DVertexShader9* shader);

	inline LPDIRECT3DDEVICE9	getDevice(){return mpD3DDevice;}

	inline const D3DCAPS9& getDeviceCaps()const{return capacity;}
public:

	//进入消息循环
	void mainLoop();
	//清除资源
	ZHResult Cleanup();





private:
	//查询当前系统可用的显示适配器和每种显示适配器支持的视频模式
	bool _captureAdapterAbility();
	//获取当前合适的视频模式
	bool _captureCurVideoMode(bool ifFullScreen,ZHRect window);


private:

	ID3D11Device* mpD3DDevice;
	ID3D11DeviceContext* pd3dDeviceContext;
	IDXGIAdapter* pAdapter;
	
	//设备能力
	//D3DCAPS9 capacity;
	
	//字体
	Font textRender;
	//FPS
	FPS m_fps;
	//显示适配器
	std::vector<ZHDisplayAdapter*> _adapterList;
	//当前活动显示模式
	VideoMode curVideoMode;

	//
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9   indexBuffer;
	

};

};
