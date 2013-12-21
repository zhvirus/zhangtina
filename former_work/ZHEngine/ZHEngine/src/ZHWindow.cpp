#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHWindow.h"
#include "ZHRenderSystem.h"
#include "ZHCallBacks.h"
#include "ZHSceneManager.h"

namespace ZH
{


	//��̬��Ա����
	HWND Window::hWnd = NULL;
	ZHRect Window::windowSizePos = ZHRect(0,0,0,0);
	LPCTSTR Window::windowTitle	=	TEXT("default");
	RenderSystem* Window::render = NULL;
	SceneManager* Window::sceneManager	=	NULL;
	ZHCALLBACKS Window::m_callbacks;
	ZHint Window::curMouseX	=	0;
	ZHint Window::curMouseY	=	0;

	//���졢����
	Window::Window(){}
	Window::~Window(){}

	HWND Window::getWindowHandle(){return hWnd;}
	ZHRect Window::getWindowSize(){return windowSizePos;}
	LPCTSTR Window::getWindowTitle(){return windowTitle;}
	void Window::getMousePos(ZHint& mouseX,ZHint& mouseY)
	{
		mouseX	=	curMouseX;
		mouseY	=	curMouseY;
	}

	void Window::setRenderSystem(RenderSystem* _render){render = _render;}
	void Window::setSceneManager(SceneManager* _sceneManager){sceneManager=_sceneManager;}
	void Window::CleanUp()
	{
		if(render)
		{
			render->Cleanup();
			render = NULL;
		}
	}



//-----------------------------------------------------------------------------
// Name: ZHMsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI Window::_ZHMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	
	
	case WM_KEYDOWN:
			{
				
				int keypress	=	-1;
				#define KEY(a,b) case a: keypress = b; break;
					switch( wParam )
					{
						KEY( VK_F1,     ZH_KEY_F1        );
						KEY( VK_F2,     ZH_KEY_F2        );
						KEY( VK_F3,     ZH_KEY_F3        );
						KEY( VK_F4,     ZH_KEY_F4        );
						KEY( VK_F5,     ZH_KEY_F5        );
						KEY( VK_F6,     ZH_KEY_F6        );
						KEY( VK_F7,     ZH_KEY_F7        );
						KEY( VK_F8,     ZH_KEY_F8        );
						KEY( VK_F9,     ZH_KEY_F9        );
						KEY( VK_F10,    ZH_KEY_F10       );
						KEY( VK_F11,    ZH_KEY_F11       );
						KEY( VK_F12,    ZH_KEY_F12       );
						KEY( VK_PRIOR,  ZH_KEY_PAGE_UP   );
						KEY( VK_NEXT,   ZH_KEY_PAGE_DOWN );
						KEY( VK_HOME,   ZH_KEY_HOME      );
						KEY( VK_END,    ZH_KEY_END       );
						KEY( VK_LEFT,   ZH_KEY_LEFT      );
						KEY( VK_UP,     ZH_KEY_UP        );
						KEY( VK_RIGHT,  ZH_KEY_RIGHT     );
						KEY( VK_DOWN,   ZH_KEY_DOWN      );
						KEY( VK_INSERT, ZH_KEY_INSERT    );
					}

				if(keypress!=-1)	
				{	
					//������ص�
					if(m_callbacks.specialkeyboard)
						(*m_callbacks.specialkeyboard)(keypress,curMouseX,curMouseY);
				}
				break;
			}
		

	case WM_CHAR:
			{
				//�����¼��ص�
				if(m_callbacks.keyboard)
				(*m_callbacks.keyboard)(static_cast<ZHchar>(wParam),curMouseX,curMouseY);
	
				break;	
			}


	case WM_SIZE:
			{
	
				windowSizePos.width	=	LOWORD(lParam);
				windowSizePos.height	=	HIWORD(lParam);
				//���ڴ�С�ı�ص�			
				if(m_callbacks.reshape)
				(*m_callbacks.reshape)(LOWORD(lParam),HIWORD(lParam));
			

				break;
			}

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		{
			ZHbool pressed	= true;
			ZHint	button;

			curMouseX	=	LOWORD(lParam);
			curMouseY	=	HIWORD(lParam);
			
	  switch( msg )
        {
        case WM_LBUTTONDOWN:
            pressed = true;
            button = ZH_LEFT_BUTTON;
            break;
        case WM_MBUTTONDOWN:
            pressed = true;
            button = ZH_MIDDLE_BUTTON;
            break;
        case WM_RBUTTONDOWN:
            pressed = true;
            button = ZH_RIGHT_BUTTON;
            break;
        case WM_LBUTTONUP:
            pressed = false;
            button = ZH_LEFT_BUTTON;
            break;
        case WM_MBUTTONUP:
            pressed = false;
            button = ZH_MIDDLE_BUTTON;
            break;
        case WM_RBUTTONUP:
            pressed = false;
            button = ZH_RIGHT_BUTTON;
            break;
        default:
            pressed = false;
            button = -1;
            break;
        }
		

		if(button!=-1)
		{
			int state	=	(pressed==true)?ZH_DOWN:ZH_UP;
			if(m_callbacks.mouse)
			(*m_callbacks.mouse)(button,state,LOWORD(lParam),HIWORD(lParam));

		}

		break;
		}

		
	case WM_MOUSEMOVE:
		{
			
			//���λ�ñ���
			curMouseX	=	LOWORD(lParam);
			curMouseY	=	HIWORD(lParam);


		 if( ( wParam & MK_LBUTTON ) ||
            ( wParam & MK_MBUTTON ) ||
            ( wParam & MK_RBUTTON ) )
			{
				//������������ʱ�������ƶ�����ص��������
				if(m_callbacks.motion)
				(*m_callbacks.motion)(LOWORD(lParam),HIWORD(lParam));
			}
		else
			{	
				//û���κ���������ʱ�������ƶ�������������
				if(m_callbacks.passiveMotion)
				(*m_callbacks.passiveMotion)(LOWORD(lParam),HIWORD(lParam));
			}
			break;
			
		}





        case WM_DESTROY:
		{


          PostQuitMessage( 0 );
          return 0;
		}

        case WM_PAINT:
			{
            
				return 0;
			}
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




HRESULT WINAPI Window::_ZHCreateWindow(LPCTSTR windowTitle,const ZHRect& rect,bool isFullScreen)
{	

	//���洰�ڴ�С������
	windowSizePos		=	rect;
	Window::windowTitle =	windowTitle;

	//register window class
	WNDCLASSEX wcex;



	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;            //���ڷ��
	wcex.lpfnWndProc	= (WNDPROC)_ZHMsgProc;                      //������Ϣ������̺���ָ��
	wcex.cbClsExtra		= 0;                                                     //WNDCLASS������ɵĶ����ֽڣ���ʼ��Ϊ0         
	wcex.cbWndExtra		= 0;                                                 //����ʵ���������Ķ����ֽ�,��ʼ��Ϊ0
	wcex.hInstance		= NULL;                                       //���ڹ��̵�ʵ�����
	wcex.hIcon			= NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_WIN32TEST);//����ͼ����
	wcex.hCursor		= NULL;// LoadCursor(NULL, IDC_ARROW);                         //�����
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);   //����ˢ���
	wcex.lpszMenuName	= NULL;                                                           //ָ�򴰿ڲ˵����Ƶ��ַ���ָ��
	wcex.lpszClassName	= TEXT("zhanghui");                                          //һ��ָ�������ƶԵ��ַ���ָ��
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL); 

	
	if(!RegisterClassEx(&wcex))
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return E_FAIL;
	}
	
	//create window
	hWnd = CreateWindow(TEXT("zhanghui"),    //��ע�ᴰ�������� 
	                                               windowTitle,    //ָ�򴰿����Ƶ�ָ��
					   WS_OVERLAPPEDWINDOW,    //���ڷ��
					   static_cast<int>(rect.x),    //����λ��x����
					   static_cast<int>(rect.y),    //����λ��y����
					   static_cast<int>(rect.width),     //���ڿ��
					   static_cast<int>(rect.height),     //���ڸ߶�
														NULL,     //�����ھ��
														NULL,     //���ڲ˵����
														NULL,     //Ӧ�ó���ʵ�����
														NULL      //Ӧ�ó���������ָ��
														       );

   if (!hWnd)
   {
	   ::MessageBox(0, "CreateWindow() - FAILED", 0, 0);
	   ZH_OUT(TEXT("Window Created  failed!\n"));
      return E_FAIL;
   }
   
  ZH_OUT(TEXT("Window Created  successfully!\n"));

   ::ShowWindow(hWnd, SW_SHOW);
   ::UpdateWindow(hWnd);


return S_OK;

}

LRESULT WINAPI Window::_ZHMainLoop()
{
  // Enter the message loop
        MSG msg; 
		memset(&msg,0,sizeof(msg));
		ZH_OUT(TEXT("Enter main loop.\n"));

        while(true)
        {
			if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
			{

				if(msg.message==WM_QUIT)
				{
				//��Դ���ͷš�����
				CleanUp();
				break;
				}
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			

			}

			// Custom override renderer
			if ( m_callbacks.render ){
				m_callbacks.render();
			} else{
				//����FPS
				render->updateFPS();
				//��Ⱦ
				sceneManager->Render();

			}
		}

	return S_OK;
    }




}