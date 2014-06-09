#ifndef _ZH_WIGET_DLL_
#define _ZH_WIGET_DLL_
#endif
#include "Widget/Window.h"
#include "Widget/WidgetCommonDefine.h"
#include "Internal/Common/Internal_common.h"

namespace ZH{
    namespace Widgets{

        WindowsInfo::WindowsInfo():
            fWndHandle( NULL ),
            fStartPosX( 0 ),
            fStartPosY( 0 ),
            fWidth( 0 ),
            fHeight( 0 )
        {}


        //======================================
        // Static area for Window
        //======================================
        Window* Window::fWindowPtr = NULL;

        LRESULT CALLBACK Window::_ZHMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
        {
            switch( msg )
            {
            case WM_KEYDOWN:
                {
                    int keypress    =   -1;
#define KEY(a,b) case a: keypress = b; break;
                    switch( wParam )
                    {
                        KEY( VK_F1,     KEY_F1        );
                        KEY( VK_F2,     KEY_F2        );
                        KEY( VK_F3,     KEY_F3        );
                        KEY( VK_F4,     KEY_F4        );
                        KEY( VK_F5,     KEY_F5        );
                        KEY( VK_F6,     KEY_F6        );
                        KEY( VK_F7,     KEY_F7        );
                        KEY( VK_F8,     KEY_F8        );
                        KEY( VK_F9,     KEY_F9        );
                        KEY( VK_F10,    KEY_F10       );
                        KEY( VK_F11,    KEY_F11       );
                        KEY( VK_F12,    KEY_F12       );
                        KEY( VK_PRIOR,  KEY_PAGE_UP   );
                        KEY( VK_NEXT,   KEY_PAGE_DOWN );
                        KEY( VK_HOME,   KEY_HOME      );
                        KEY( VK_END,    KEY_END       );
                        KEY( VK_LEFT,   KEY_LEFT      );
                        KEY( VK_UP,     KEY_UP        );
                        KEY( VK_RIGHT,  KEY_RIGHT     );
                        KEY( VK_DOWN,   KEY_DOWN      );
                        KEY( VK_INSERT, KEY_INSERT    );
                    }

                    if(keypress!=-1)
                    {
                        //特殊键回调
                        if( fWindowPtr && fWindowPtr->fCallbacks.fSpecialKeyFuncPtr)
                            (*(fWindowPtr->fCallbacks.fSpecialKeyFuncPtr))(keypress,fWindowPtr->fCurMouseX,fWindowPtr->fCurMouseY);
                    }
                    break;
                }

            case WM_CHAR:
                {
                    //键盘事件回调
                    if( fWindowPtr && fWindowPtr->fCallbacks.fKeyboardFuncPtr)
                        (*(fWindowPtr->fCallbacks.fKeyboardFuncPtr))(static_cast<unsigned char>(wParam),fWindowPtr->fCurMouseX,fWindowPtr->fCurMouseY);

                    break;
                }

            case WM_SIZE:
                {
                    if( fWindowPtr ){
                        fWindowPtr->fWndInfo.fWidth     =   LOWORD(lParam);
                        fWindowPtr->fWndInfo.fHeight    =   HIWORD(lParam);
                        //窗口大小改变回调
                        if( fWindowPtr->fCallbacks.fReShapeFuncPtr)
                            (*(fWindowPtr->fCallbacks.fReShapeFuncPtr))(fWindowPtr->fWndInfo.fWidth,fWindowPtr->fWndInfo.fHeight);
                    }
                    break;
                }

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
                {
                    bool pressed = true;
                    int button;

                    if( fWindowPtr ){
                        fWindowPtr->fCurMouseX  =   LOWORD(lParam);
                        fWindowPtr->fCurMouseY  =   HIWORD(lParam);
                    }

                    switch( msg )
                    {
                    case WM_LBUTTONDOWN:
                        pressed = true;
                        button = MOUSE_BOTTON_LEFT;
                        break;
                    case WM_MBUTTONDOWN:
                        pressed = true;
                        button = MOUSE_BOTTON_MIDDLE;
                        break;
                    case WM_RBUTTONDOWN:
                        pressed = true;
                        button = MOUSE_BOTTON_RIGHT;
                        break;
                    case WM_LBUTTONUP:
                        pressed = false;
                        button = MOUSE_BOTTON_LEFT;
                        break;
                    case WM_MBUTTONUP:
                        pressed = false;
                        button = MOUSE_BOTTON_MIDDLE;
                        break;
                    case WM_RBUTTONUP:
                        pressed = false;
                        button = MOUSE_BOTTON_RIGHT;
                        break;
                    default:
                        pressed = false;
                        button = -1;
                        break;
                    }

                    if(button!=-1)
                    {
                        int state   =   (pressed==true)?0:1;
                        if( fWindowPtr && fWindowPtr->fCallbacks.fMouseFuncPtr)
                            (*(fWindowPtr->fCallbacks.fMouseFuncPtr))(button,state,fWindowPtr->fCurMouseX,fWindowPtr->fCurMouseY);

                    }

                    break;
                }

            case WM_MOUSEMOVE:
                {
                    //鼠标位置保存
                    if ( fWindowPtr ){
                        fWindowPtr->fCurMouseX = LOWORD(lParam);
                        fWindowPtr->fCurMouseY = HIWORD(lParam);
                    }

                    if( ( wParam & MK_LBUTTON ) ||
                        ( wParam & MK_MBUTTON ) ||
                        ( wParam & MK_RBUTTON ) )
                    {
                        //在有鼠标键按下时候的鼠标移动，会回调这个函数
                        if( fWindowPtr && fWindowPtr->fCallbacks.fMotionFuncPtr)
                            (*(fWindowPtr->fCallbacks.fMotionFuncPtr))( fWindowPtr->fCurMouseX, fWindowPtr->fCurMouseY );
                    }
                    else
                    {
                        //没有任何鼠标键按下时候的鼠标移动会调用这个函数
                        if( fWindowPtr && fWindowPtr->fCallbacks.fPassiveMotionFuncPtr )
                            (*(fWindowPtr->fCallbacks.fPassiveMotionFuncPtr))(fWindowPtr->fCurMouseX,fWindowPtr->fCurMouseY);
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

        Window* Window::CreateZHWindow(
            TCHAR* title,
            unsigned int x,
            unsigned int y,
            unsigned int w,
            unsigned int h)
        {
            if ( !fWindowPtr ){
                fWindowPtr = new Window();
                fWindowPtr->createWnd(title,x,y,w,h);
            }

            ZH_OUT("Windows Created.");
            return fWindowPtr;
        }

        void Window::DestryZHWindow()
        {
            ZH_OUT("Window Destroyed.");
            delete fWindowPtr;
            fWindowPtr = NULL;
        }

        //======================================
        // Non Static area for Window
        //======================================
        bool Window::createWnd(
            TCHAR* title,
            unsigned int x,
            unsigned int y,
            unsigned int w,
            unsigned int h)
        {
            //Register window class
            WNDCLASSEX wcex;
            wcex.cbSize         = sizeof(WNDCLASSEX); 
            wcex.style          = CS_HREDRAW | CS_VREDRAW; // Window style
            wcex.lpfnWndProc    = (WNDPROC)_ZHMsgProc;     // Window message process function ptr
            wcex.cbClsExtra     = 0;                       // WNDCLASS后面分派的额外字节，初始化为0
            wcex.cbWndExtra     = 0;                       // 窗口实例化后分配的额外字节,初始化为0
            wcex.hInstance      = NULL;                    // 窗口过程的实例句柄
            wcex.hIcon          = NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_WIN32TEST);//窗口图标句柄
            wcex.hCursor        = NULL;// LoadCursor(NULL, IDC_ARROW);   //光标句柄
            wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);   //背景刷句柄
            wcex.lpszMenuName   = NULL;                    //指向窗口菜单名称的字符串指针
            wcex.lpszClassName  = TEXT("zhanghui");        //一个指向类名称对的字符串指针
            wcex.hIconSm        = NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL); 

            if(!RegisterClassEx(&wcex))
            {
                return false;
            }

            //Create window
            HWND hWnd = CreateWindow(
                TEXT("zhanghui"),           //已注册窗口类名称 
                title,                      //指向窗口名称的指针
                WS_OVERLAPPEDWINDOW,        //窗口风格
                x,    //窗口位置x坐标
                y,    //窗口位置y坐标
                w,    //窗口宽度
                h,    //窗口高度
                NULL, //父窗口句柄
                NULL, //窗口菜单句柄
                NULL, //应用程序实例句柄
                NULL  //应用程序数据区指针
                );

            if (!hWnd)
            {
                return false;
            }

            // Record window information
            if ( title ){
                strcpy_s( fWndInfo.fWndTitle, title );
            }else{
                fWndInfo.fWndTitle[0] = 0;
            }
            fWndInfo.fWndHandle = hWnd;
            fWndInfo.fStartPosX = x;
            fWndInfo.fStartPosY = y;
            fWndInfo.fWidth     = w;
            fWndInfo.fHeight    = h;

            return true;
        }

        void Window::showWindow()
        {
            if ( !fWndInfo.fWndHandle )
                return;

            ::ShowWindow( fWndInfo.fWndHandle, SW_SHOW);
            ::UpdateWindow( fWndInfo.fWndHandle );

            ZH_OUT("Show up window.");
        }

        void Window::enterMsgLoop(){
            // Enter the message loop
            MSG msg; 
            memset(&msg,0,sizeof(msg));
            ZH_OUT( "Enter main loop.\n" );

#pragma warning(push)
#pragma warning(disable:4127)
            while(true)
#pragma warning(pop)
            {
                if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
                {

                    if(msg.message==WM_QUIT)
                    {
                        break;
                    }
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }

                // Custom override renderer
                if ( fCallbacks.fRenderFuncPtr ){
                    fCallbacks.fRenderFuncPtr();
                } else{
                }
            }
        }

    }

}
