#ifndef _ZH_WIGET_DLL_
#define _ZH_WIGET_DLL_
#endif
#include "Widget/Window.h"
#include "Widget/WidgetCommonDefine.h"
#include "Internal/Common/Internal_common.h"
#include <map>

namespace ZH{
    namespace Widgets{

        WindowsInfo::WindowsInfo():
            m_winHandle( NULL ),
            m_startPosX( 0 ),
            m_startPosY( 0 ),
            m_width( 0 ),
            m_height( 0 )
        {}

        //======================================
        // Static area for Window
        //======================================

        // Window map
        static std::map<HWND, Window*> sWindowMap;

        static Window* GetWindowFromMap(HWND hwnd)
        {
            std::map<HWND, Window*>::iterator it = sWindowMap.find(hwnd);
            if (it == sWindowMap.end())
            {
                return NULL;
            }
            return it->second;
        }

        static void AddWindowToMap(HWND hwnd, Window* window)
        {
            sWindowMap.insert(std::make_pair(hwnd, window));
        }

        static void RemoveWindowFromMap(Window* window)
        {
            std::map<HWND, Window*>::iterator it = sWindowMap.begin();
            for (; it != sWindowMap.end(); ++it)
            {
                if (it->second == window)
                {
                    sWindowMap.erase(it);
                    break;
                }
            }
        }

        static void RemoveWindowFromMap(HWND hwnd)
        {
            std::map<HWND, Window*>::iterator it = sWindowMap.find(hwnd);
            if (it != sWindowMap.end())
            {
                sWindowMap.erase(it);
            }
        }


#define KEY(a,b) case a: keypress = b; break;

        LRESULT CALLBACK Window::_ZHMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
        {
            Window* pWin = GetWindowFromMap(hWnd);

            switch (msg)
            {
                case WM_KEYDOWN:
                {
                    int keypress = -1;
                    switch (wParam)
                    {
                        KEY(VK_F1, KEY_F1);
                        KEY(VK_F2, KEY_F2);
                        KEY(VK_F3, KEY_F3);
                        KEY(VK_F4, KEY_F4);
                        KEY(VK_F5, KEY_F5);
                        KEY(VK_F6, KEY_F6);
                        KEY(VK_F7, KEY_F7);
                        KEY(VK_F8, KEY_F8);
                        KEY(VK_F9, KEY_F9);
                        KEY(VK_F10, KEY_F10);
                        KEY(VK_F11, KEY_F11);
                        KEY(VK_F12, KEY_F12);
                        KEY(VK_PRIOR, KEY_PAGE_UP);
                        KEY(VK_NEXT, KEY_PAGE_DOWN);
                        KEY(VK_HOME, KEY_HOME);
                        KEY(VK_END, KEY_END);
                        KEY(VK_LEFT, KEY_LEFT);
                        KEY(VK_UP, KEY_UP);
                        KEY(VK_RIGHT, KEY_RIGHT);
                        KEY(VK_DOWN, KEY_DOWN);
                        KEY(VK_INSERT, KEY_INSERT);
                        KEY(VK_ESCAPE, KEY_ESCAPE);
                    }

                    if (keypress == KEY_ESCAPE)
                    {
                        PostQuitMessage(0);
                    }

                    if (keypress != -1)
                    {
                        if (pWin && pWin->m_callbacks.fSpecialKeyFuncPtr)
                        {
                            if (pWin->m_debug)
                            {
                                printf("SpecialKeyBoardFunc(%d, x=%d, y=%d)\n",
                                    keypress, pWin->m_curMouseX, pWin->m_curMouseY);
                            }

                            (*(pWin->m_callbacks.fSpecialKeyFuncPtr))(keypress, pWin->m_curMouseX, pWin->m_curMouseY);
                        }
                    }
                    break;
                }

                case WM_CHAR:
                {
                    if (pWin && pWin->m_callbacks.fKeyboardFuncPtr)
                    {
                        if (pWin->m_debug)
                        {
                            printf("KeyBoardFunc(%c, x=%d, y=%d)\n", static_cast<unsigned char>(wParam),
                                pWin->m_curMouseX, pWin->m_curMouseY);
                        }

                        (*(pWin->m_callbacks.fKeyboardFuncPtr))(static_cast<unsigned char>(wParam), pWin->m_curMouseX, pWin->m_curMouseY);
                    }
                    break;
                }

                case WM_SIZE:
                {
                    if (pWin)
                    {
                        pWin->m_winInfo.m_width = LOWORD(lParam);
                        pWin->m_winInfo.m_height = HIWORD(lParam);
                        if (pWin->m_callbacks.fReShapeFuncPtr)
                        {
                            if (pWin->m_debug)
                            {
                                printf("ReShapeFunc(w=%d, h=%d)\n",
                                    pWin->m_winInfo.m_width, pWin->m_winInfo.m_height);
                            }
                            (*(pWin->m_callbacks.fReShapeFuncPtr))(pWin->m_winInfo.m_width, pWin->m_winInfo.m_height);
                        }
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

                    if (pWin)
                    {
                        pWin->m_curMouseX = LOWORD(lParam);
                        pWin->m_curMouseY = HIWORD(lParam);
                    }

                    switch (msg)
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

                    if (button != -1)
                    {
                        if (pWin && pWin->m_callbacks.fMouseFuncPtr)
                        {
                            if (pWin->m_debug)
                            {
                                printf("MouseFunc(button=%d, pressed=%d, x=%d, y=%d)\n",
                                    button, pressed ? 1 : 0, pWin->m_curMouseX, pWin->m_curMouseY);
                            }
                            (*(pWin->m_callbacks.fMouseFuncPtr))(button, pressed ? 1 : 0, pWin->m_curMouseX, pWin->m_curMouseY);
                        }

                    }

                    break;
                }

                case WM_MOUSEMOVE:
                {
                    if (pWin)
                    {
                        pWin->m_curMouseX = LOWORD(lParam);
                        pWin->m_curMouseY = HIWORD(lParam);
                    }

                    if ((wParam & MK_LBUTTON) ||
                        (wParam & MK_MBUTTON) ||
                        (wParam & MK_RBUTTON))
                    {
                        if (pWin && pWin->m_callbacks.fMotionFuncPtr)
                        {
                            if (pWin->m_debug)
                            {
                                printf("MotionFunc(button=%d, x=%d, y=%d)\n",
                                    (int)wParam, pWin->m_curMouseX, pWin->m_curMouseY);
                            }
                            (*(pWin->m_callbacks.fMotionFuncPtr))((int)wParam, pWin->m_curMouseX, pWin->m_curMouseY);
                        }
                    }
                    else
                    {
                        if (pWin && pWin->m_callbacks.fPassiveMotionFuncPtr)
                        {
                            if (pWin->m_debug)
                            {
                                printf("PassiveMotionFunc(x=%d, y=%d)\n",
                                    pWin->m_curMouseX, pWin->m_curMouseY);
                            }
                            (*(pWin->m_callbacks.fPassiveMotionFuncPtr))(pWin->m_curMouseX, pWin->m_curMouseY);
                        }
                    }
                    break;
                }

                case WM_DESTROY:
                {
                    if (pWin->m_debug)
                    {
                        printf("WM_DESTROY\n");
                    }
                    PostQuitMessage(0);
                    return 0;
                }

                case WM_PAINT:
                {
                    return 0;
                }
            }

            return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        Window* Window::Create(
            TCHAR* title,
            unsigned int x,
            unsigned int y,
            unsigned int w,
            unsigned int h,
            bool debug/* = false*/)
        {
            Window* pWin = new Window(debug);
            if (!pWin->CreateWinImp(title, x, y, w, h))
            {
                delete pWin;
                return NULL;
            }

            ZH_OUT("A new window created successfully.");
            return pWin;
        }

        void Window::Destroy(Window*& pWin)
        {
            if (!pWin)
            {
                return;
            }

            RemoveWindowFromMap(pWin);

            delete pWin;
            pWin = NULL;
        }


        //======================================
        // Non Static area for Window
        //======================================

        Window::Window(bool debug):
            m_debug(debug),
            m_curMouseX(0),
            m_curMouseY(0)
        {}

        Window::~Window()
        {
            DestroyWindow(m_winInfo.m_winHandle);
        }

        bool Window::CreateWinImp(
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
            wcex.cbClsExtra     = 0;                       // 
            wcex.cbWndExtra     = 0;                       // 
            wcex.hInstance      = NULL;                    //
            wcex.hIcon          = NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_WIN32TEST);//A handle to the class icon
            wcex.hCursor        = NULL;// LoadCursor(NULL, IDC_ARROW);   //A handle to the class cursor
            wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);   //A handle to the class background brush
            wcex.lpszMenuName   = NULL;                    //
            wcex.lpszClassName  = TEXT("zhanghui");        //
            wcex.hIconSm        = NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL); 

            if(!RegisterClassEx(&wcex))
            {
                return false;
            }

            //Create window
            HWND hWnd = CreateWindow(
                TEXT("zhanghui"),           //
                title,                      //
                WS_OVERLAPPEDWINDOW,        //
                x,    //
                y,    //
                w,    //
                h,    //
                NULL, //
                NULL, //
                NULL, //
                NULL  //
                );

            if (!hWnd)
            {
                return false;
            }

            // Record window information
            if ( title ){
                strcpy_s( m_winInfo.m_title, title );
            }else{
                m_winInfo.m_title[0] = 0;
            }
            m_winInfo.m_winHandle = hWnd;
            m_winInfo.m_startPosX = x;
            m_winInfo.m_startPosY = y;
            m_winInfo.m_width     = w;
            m_winInfo.m_height    = h;

            AddWindowToMap(hWnd, this);

            return true;
        }

        void Window::ShowWindow()
        {
            if ( !m_winInfo.m_winHandle )
                return;

            ::ShowWindow( m_winInfo.m_winHandle, SW_SHOW);
            ::UpdateWindow( m_winInfo.m_winHandle );

            ZH_OUT("Show up window.");
        }

        void Window::EnterMsgLoop(){
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
                if ( m_callbacks.fRenderFuncPtr ){
                    m_callbacks.fRenderFuncPtr();
                }
            }
        }

    }

}
