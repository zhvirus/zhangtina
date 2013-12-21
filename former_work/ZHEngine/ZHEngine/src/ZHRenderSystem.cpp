#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHRenderSystem.h"
#include "ZHColor.h"
#include "ZHSceneManager.h"


namespace ZH
{
	//-------------------------------------------------------------
	//class FPS
	//-------------------------------------------------------------
	FPS::FPS(ZHfloat updateTime	)
	{	
		if(updateTime>0.005f)
			upTime	=	updateTime;
		else
			upTime	=	0.5f;

		lastTime	=	static_cast<ZHfloat>(timeGetTime())*0.001f;
		numOfFrame	=	0.0f;
		costTime	=	0.0f;
	}

	void FPS::update()
	{
		ZHfloat curTime	=	static_cast<ZHfloat>(timeGetTime())*0.001f;//get current time
		ZHfloat timePass	=	curTime	-	 lastTime;	//time cost for last frame
		lastTime	=	curTime;	//record current time for next calculation
		numOfFrame++;			//increase the num of frames between two calculation
		costTime+=timePass;	//increase total time cost between two calculation 
		if(costTime>upTime)		//when calculation is taken
		{
			ZHfloat _fps	=	numOfFrame/costTime;
			sprintf(fps_str,"FPS: %0.2f",_fps);
			fps_str[14]	=	'\0';
			
			numOfFrame	=	0.0f;
			costTime			=	0.0f;

		}



	}


	//-------------------------------------------------------------
	//class RenderSystem
	//-------------------------------------------------------------

	//构造函数
	RenderSystem::RenderSystem():
		mpD3DDevice (NULL),
		 pd3dDeviceContext(NULL),
		pAdapter(NULL)
							m_fps(0.5f),
							vertexBuffer(NULL),
							indexBuffer(NULL)
							
							
	{						
	}


	RenderSystem::~RenderSystem()
	{
	//释放显示适配器列表资源
		std::vector<ZHDisplayAdapter*>::iterator ite;
		for(ite=_adapterList.begin();ite!=_adapterList.end();ite++)
		{
			SAFE_DELETE(*(ite));


		}
		_adapterList.clear();
	
	
	
	}
	
	void RenderSystem::mainLoop()
	{
		Window::_ZHMainLoop();

	}

	void RenderSystem::updateFPS()
	{
		m_fps.update();

	}


	ZHResult RenderSystem::Cleanup()
	{
		SAFE_RELEASE(mpD3D);
		SAFE_RELEASE(mpD3DDevice);

		return ZH_OK;
	}
	
	void RenderSystem::setVertexShader(IDirect3DVertexShader9* shader)
	{

		mpD3DDevice->SetVertexShader(shader);

	}

	//=======================================================
	//建立窗口，初始化D3D，进入消息循环
	//=======================================================
	ZHResult RenderSystem::initialize(LPCTSTR windowTitle,ZHRect window,bool isFullScreen,SceneManager* _sceneManager)
	{
		//传入场景管理指针
		Window::setSceneManager(_sceneManager);
		
		//将RenderSystem地址传入Window，方便window退出时候消除D3D资源
		Window::setRenderSystem(this);
		//创建窗口
		Window::_ZHCreateWindow(windowTitle,window,isFullScreen);



		
	// Step 1: Create the IDirect3D9 object.
	
	// Create the D3D object, which is needed to create the D3DDevice.
	//if( NULL == (mpD3D= Direct3DCreate9( D3D_SDK_VERSION ) ) )
	//{
	//	::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
	//	ZH_OUT(TEXT("Direct3DCreate9()-failed!\n"));
	//	return ZH_D3D_Direct3DCreate9_FAILED;
	//}

	//ZH_OUT(TEXT("Direct3DCreate9()-  successful!\n"));
	// Step 2: Check for hardware vp.

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. 

	//D3DCAPS9 caps;
	//mpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//ZHint vp = 0;
	//if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	//{
	//	vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	//	ZH_OUT(TEXT("Hardware vertex process support-----YES\n"));
	//}
	//else
	//{
	//	vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	//	ZH_OUT(TEXT("Hardware vertex process support-----NO\n"));
	//}
	//
	////查询显卡信息
	//_captureAdapterAbility();
	//_captureCurVideoMode(isFullScreen,window);

 //   D3DPRESENT_PARAMETERS d3dpp; 
 //   ZeroMemory( &d3dpp, sizeof(d3dpp) );
	

	//d3dpp.BackBufferWidth		=	curVideoMode.getWidth();
	//d3dpp.BackBufferHeight		=	curVideoMode.getHeight();
	//d3dpp.BackBufferFormat		=	curVideoMode.getPixelFormat();
	//d3dpp.BackBufferCount		=	1;
	//d3dpp.MultiSampleType		= D3DMULTISAMPLE_NONE;
	//d3dpp.MultiSampleQuality	= 0;
	//d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD; 
	//d3dpp.hDeviceWindow        = Window::getWindowHandle();
	//d3dpp.Windowed					=	isFullScreen?false:true;
	//d3dpp.EnableAutoDepthStencil	=	true;
	//d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	//d3dpp.Flags                      = 0;
	//d3dpp.FullScreen_RefreshRateInHz = curVideoMode.getRefreshRate();
	//d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Create the Direct3D device. 
	if( FAILED(	D3D11CreateDevice( 
					D3DADAPTER_DEFAULT, 
					D3DDEVTYPE_HAL, 
					Window::getWindowHandle(),
					vp,
					&d3dpp, 
					&mpD3DDevice) ) )
    {
		::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
		ZH_OUT(TEXT("Create Device failed!\n"));
		return ZH_D3D_CreateDevice_FAILED;
    }
	
	//获取设备的能力属性
	mpD3DDevice->GetDeviceCaps(&capacity);
	//shader model 版本
	std::cout<<"Vertex Shader Version: "<<D3DXGetVertexShaderProfile(mpD3DDevice)<<std::endl;
	std::cout<<"Pixel Shader Version: "<<D3DXGetPixelShaderProfile(mpD3DDevice)<<std::endl;

	//初始化字体显示
	textRender.InitFont(mpD3DDevice);
	
	ZH_OUT(TEXT("D3D initialized successfully!\n"));



//for(int test=0;test<150;test++)
//{
		HRESULT hh	=	mpD3D->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
D3DFMT_X8R8G8B8,
D3DUSAGE_DYNAMIC|D3DUSAGE_QUERY_VERTEXTEXTURE,
D3DRTYPE_TEXTURE,
D3DFMT_R32F);

		if(hh==D3D_OK)
		{

			std::cout<<"Vertex Texture Supported!"<<"..."<<"D3DFMT_R32F"<<std::endl;
		}
//}

		return ZH_OK;


	}

	void RenderSystem::clear()
	{


		mpD3DDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, Color::BLACK, 1.0f, 0);

	}

	void RenderSystem::beginFrame()
	{

		mpD3DDevice->BeginScene();
	}

	void RenderSystem::endFrame()
	{

		mpD3DDevice->EndScene();
	}

	void RenderSystem::printFPS()
	{
		textRender.Render(m_fps.fps_str,ZHRect(0,0,300,300),0xff00ff00);

	}


	void RenderSystem::present()
	{
		//
		//


		//
		//
		mpD3DDevice->Present(0,0,0,0);
	
	}

	//查询当前系统安装的显示适配器信息
	bool RenderSystem::_captureAdapterAbility()
	{	
		for( UINT iAdapter=0; iAdapter < mpD3D->GetAdapterCount(); ++iAdapter )
		{

			D3DADAPTER_IDENTIFIER9 adapterIdentifier;
			D3DDISPLAYMODE d3ddm;
			mpD3D->GetAdapterIdentifier( iAdapter, 0, &adapterIdentifier );
			mpD3D->GetAdapterDisplayMode( iAdapter, &d3ddm );
			
			ZHDisplayAdapter* adapterTemp		=	new ZHDisplayAdapter(iAdapter,mpD3D);
			if(!adapterTemp)
				return false;

			if(!(adapterTemp->initialize(adapterIdentifier)))
				return false;

			_adapterList.push_back(adapterTemp);


		}		

	return true;

	}

	//查询当前活动视频模式
	bool RenderSystem::_captureCurVideoMode(bool ifFullScreen,ZHRect window)
	{
		//窗口模式
		if(!ifFullScreen)
		{
			curVideoMode.setWidth(static_cast<unsigned int>(window.width));
			curVideoMode.setHeight(static_cast<unsigned int>(window.height));
			curVideoMode.setRefreshRate(0);
			curVideoMode.setPixelFormat(D3DFMT_X8R8G8B8);

		}
		//全屏模式
		else
		{
			curVideoMode	=	(_adapterList[0])->getSuitableVideoMode();



		}


		return true;

	}





	//设置世界变换矩阵
	void RenderSystem::setWorldMatrix(const ZH::Matrix4& _world)
	{
		D3DMATRIX wMat;
		memcpy(&wMat,&_world,sizeof(ZH::Matrix4));
		mpD3DDevice->SetTransform(D3DTS_WORLD,&wMat);

	}
	//设置视点变换矩阵
	void RenderSystem::setViewMatrix(const ZH::Matrix4& _view)
	{
		D3DMATRIX vMat;
		memcpy(&vMat,&_view,sizeof(ZH::Matrix4));
		mpD3DDevice->SetTransform(D3DTS_VIEW,&vMat);

	}
	//设置投影变换矩阵
	void RenderSystem::setProjectionMatrix(const ZH::Matrix4& _proj)
	{
		D3DMATRIX	pMat;
		memcpy(&pMat,&_proj,sizeof(ZH::Matrix4));
		mpD3DDevice->SetTransform(D3DTS_PROJECTION,&pMat);


	}




}