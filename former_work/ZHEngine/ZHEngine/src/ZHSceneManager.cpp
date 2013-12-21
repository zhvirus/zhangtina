#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHSceneManager.h"
#include "ZHTerrain.h"
#include "ZHImage.h"
#include "ZHMesh.h"
#include "ZHPass.h"
#include "ZHResource.h"
#include "ZHRenderable.h"
#include "ZHCamera.h"

namespace ZH
{

	SceneManager::SceneManager():
	renderSystem(NULL),
	_resource(NULL),
	_terrain(NULL),
	_camera(NULL)
	{
		_camera	=	Singleton<Camera>::getSingletonPtr();


	}


	SceneManager::~SceneManager()
	{
	
		_destroyPassMap();


		SAFE_DELETE(_terrain);
		SAFE_DELETE(Singleton<Image>::getSingletonPtr());
		SAFE_DELETE(_resource);
		SAFE_DELETE(renderSystem);
		
		SAFE_DELETE(_camera);

	}

	void SceneManager::_destroyPassMap()
	{
		PassMapIte	  ite;
		for(ite=_passMap.begin();ite!=_passMap.end();ite++)
		{
			//delete pass
			SAFE_DELETE((*ite).first);

			RenderableList::iterator _renderableIte;
			for(_renderableIte=(*ite).second.begin();_renderableIte!=(*ite).second.end();_renderableIte++)
			{
				SAFE_DELETE(*_renderableIte);

			}

			(*ite).second.clear();
			
		}

		_passMap.clear();


	}


	void SceneManager::Render()
	{
		renderSystem->clear();

		renderSystem->beginFrame();
	
			renderSystem->printFPS();
			_renderAll();


		renderSystem->endFrame();

		renderSystem->present();
	}

	void SceneManager::updateData()
	{
		_terrain->updateData();
	
	}


	bool SceneManager::initialize(	
						LPCTSTR windowTitle,//窗口标题
						ZHRect window,//窗口尺寸和位置
						bool isFullScreen)
	{

		//初始化renderSystem
		if(renderSystem!=NULL)
		{
			SAFE_DELETE(renderSystem);
		}

		renderSystem	=	Singleton<RenderSystem>::getSingletonPtr();
		if(renderSystem!=NULL)
		{
			renderSystem->initialize(windowTitle,window,isFullScreen,this);
		

		}
		else
		{
			return false;
		}

		//初始化Camera
		D3DXMATRIX pMatrix;
		float t_fovy		=D3DX_PI/4.0f;
		ZHRect t_rect;
		t_rect	=	Window::getWindowSize();
		float t_aspect		=t_rect.width/t_rect.height;
		float t_near		=1.0f;
		float t_far			=200000.0f;

		D3DXMatrixPerspectiveFovLH(&pMatrix,t_fovy,t_aspect,t_near,t_far);
		ZH::Matrix4 p_m;
		memcpy(&p_m,&pMatrix,sizeof(pMatrix));
		_camera->setProjMatrix(p_m);
		_camera->setViewFrustumParam(t_fovy, t_aspect, t_near, t_far);
		_camera->setPosition(D3DXVECTOR3(0,300,0));



		//初始化资源
		_resource	=	Singleton<Resource>::getSingletonPtr();

		////初始化terrain
		//_terrain	=	Singleton<Terrain>::getSingletonPtr();	

		//if(_terrain==NULL)
		//{
		//	ZH_OUT(TEXT("SceneManager--->initialize--->terrain failed!"));
		//	return false;
		//}

		//_terrain->initialize();

		return true;


	}

	void SceneManager::mainLoop()
	{

		renderSystem->mainLoop();


	}

	PassMap& SceneManager::getSceneMap()
	{

		return _passMap;


	}

	//渲染_passMap中所有可渲染物体
	void SceneManager::_renderAll()
	{	
		
		//设置摄像机
		renderSystem->setViewMatrix(_camera->getViewMatrix());
		renderSystem->setProjectionMatrix(_camera->getProjMatrix());

		//渲染地形
		//_terrain->estabilsh();
		//_terrain->render();
		
		Pass* p_temp;
		Renderable* r_temp;

	
		renderSystem->setVertexShader(NULL);
		//迭代渲染所有物体
		for(PassMapIte ite	=	_passMap.begin();ite!=_passMap.end();ite++)
		{
			p_temp	=	(*ite).first;
			RenderableList& renderList	=	(*ite).second;
			
			//设置渲染环境
			p_temp->establish();


			for(RenderableList::iterator ite2	=	renderList.begin();ite2!=renderList.end();ite2++)
			{
				//设置变换矩阵 world view projection
				r_temp = (*ite2); //遍历每一个renderable
				//如果可见
				if(r_temp->getVisile())
				{
					renderSystem->setWorldMatrix(r_temp->getWorldMatrix());

					//renderSystem->setProjectionMatrix(r_temp->getProjMatrix());
					r_temp->getMesh()->render();
				}

			}


		}

	}




}
