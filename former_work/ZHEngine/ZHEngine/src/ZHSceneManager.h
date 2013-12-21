#pragma once

#include "ZHRenderDecl.h"
#include "ZHRenderSystem.h"

namespace ZH
{

//****************************************
//class name:		SceneManager
//author:				Hui Zhang
//latest modified:	2007.6.22
//description:			
//****************************************
typedef std::vector<Renderable*> RenderableList;
typedef std::map<Pass*,RenderableList> PassMap;
typedef std::map<Pass*,RenderableList>::iterator PassMapIte;

class ZH_DLL SceneManager
{
    friend class Window;
	

public:
	explicit SceneManager();
	~SceneManager();



public:
	bool initialize(	LPCTSTR windowTitle=TEXT("Zhanghui Engine"),//窗口标题
						ZHRect window=ZHRect(40,40,500,400),//窗口尺寸和位置
						bool isFullScreen=false);
	void mainLoop();

	PassMap& getSceneMap();
	inline Camera* getCamera(){return _camera;}
	void updateData();
private:
	void _destroyPassMap();
	void Render();//add in class window for free call
	void _renderAll();//render all scene
private:
	RenderSystem* renderSystem;
	Terrain* _terrain;
	
	//pass 和mesh列表的绑定map
	PassMap _passMap;
	Resource* _resource;
	
	//摄像机
	Camera* _camera;


};


}