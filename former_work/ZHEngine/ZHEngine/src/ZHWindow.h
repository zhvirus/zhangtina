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
	
	//window����Ϣ������
	static LRESULT WINAPI _ZHMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	//��������
	static HRESULT WINAPI _ZHCreateWindow(LPCTSTR windowTitle,const ZHRect& rect,bool isFullScreen);
	//��ѭ��
	static LRESULT WINAPI _ZHMainLoop();
	//��ȡ���ھ��
	static HWND getWindowHandle();
	//��ȡ���ڴ�С
	static ZHRect getWindowSize();
	//��ȡ���ڱ���
	static LPCTSTR getWindowTitle();
	//��ȡ��ǰ���λ��
	static void getMousePos(ZHint& mouseX,ZHint& mouseY);
	
	//���浱ǰrenderSystem��һ��ָ�룬����window�ж�renderSystem�ĵ���
	static void setRenderSystem(RenderSystem* _render);
	
	//���浱ǰsceneManager��һ��ָ��
	static void setSceneManager(SceneManager* _sceneManager);

	//�˳������������
	static void CleanUp();

public:
	//�ص������������
	static ZHCALLBACKS m_callbacks;
private:

	//���ھ��
	static HWND hWnd;

	//����λ�ʹ�С
	static ZHRect windowSizePos;
	
	//���ڱ���
	static LPCTSTR windowTitle;

	//RenderSystem
	static RenderSystem* render;

	//SceneManager
	static SceneManager* sceneManager;
	
	//��ǰ���λ��
	static ZHint curMouseX;
	static ZHint curMouseY;






};

}