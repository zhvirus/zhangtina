#pragma once

#include "ZHRenderDecl.h"


namespace ZH
{


//****************************************
//class name:		Window
//author:				Hui Zhang
//latest modified:	2007.2.3
//description:			window&message management
//****************************************

class ZH_DLL Window
{
public:
	Window();
	~Window();
	
	//window的消息处理函数
	static LRESULT WINAPI _ZHMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	//创建窗口
	static HRESULT WINAPI _ZHCreateWindow(LPCTSTR windowTitle,const ZHRect& rect,bool isFullScreen);
	//主循环
	static LRESULT WINAPI _ZHMainLoop();
	//获取窗口句柄
	static HWND getWindowHandle();
	//获取窗口大小
	static ZHRect getWindowSize();
	//获取窗口标题
	static LPCTSTR getWindowTitle();
	//获取当前鼠标位置
	static void getMousePos(ZHint& mouseX,ZHint& mouseY);
	
	//保存当前renderSystem的一个指针，方便window中对renderSystem的调用
	static void setRenderSystem(RenderSystem* _render);
	
	//保存当前sceneManager的一个指针
	static void setSceneManager(SceneManager* _sceneManager);

	//退出程序的清理工作
	static void CleanUp();

public:
	//回调函数管理对象
	static ZHCALLBACKS m_callbacks;
private:

	//窗口句柄
	static HWND hWnd;

	//窗口位和大小
	static ZHRect windowSizePos;
	
	//窗口标题
	static LPCTSTR windowTitle;

	//RenderSystem
	static RenderSystem* render;

	//SceneManager
	static SceneManager* sceneManager;
	
	//当前鼠标位置
	static ZHint curMouseX;
	static ZHint curMouseY;






};

}