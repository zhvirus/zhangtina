#ifndef _ZH_WIGET_DLL_
#define _ZH_WIGET_DLL_
#endif

#include "Widget/Window.h"
#include "Widget/WidgetCommonDefine.h"
#include "Internal/Common/Internal_common.h"

#include <time.h>

#define WINMAP_CAST_PTR(P) ((std::map<HWND, std::shared_ptr<Window>>*)(P))

namespace ZH{
    namespace Widgets{


        //======================================
        // Static area for Window
        //======================================

        //======================================
        // Non Static area for Window
        //======================================

        Window::Window(WindowsInfo& winInfo, bool debug):
            m_winInfo(winInfo),
            m_debug(debug)
        {}

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
                    
                    if (m_debug) {
                        clock_t curClock = clock();
                        unsigned int curMilliSec = (unsigned int)(curClock);
                        m_fps.updateByCurTime((unsigned int)curMilliSec);
                        m_fps.printFPS((unsigned int)curMilliSec);
                    }
                }
            }
        }


        //======================================
        // WindowFactory
        //======================================

#define KEY(a,b) case a: keypress = b; break;

        LRESULT CALLBACK WindowFactory::_ZHMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            std::shared_ptr<Window> pWin = WindowFactory::instance().getWindowByHandle(hWnd);

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
                if (pWin && pWin->m_debug)
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




        WindowFactory& WindowFactory::instance()
        {
            static WindowFactory s;
            return s;
        }

        WindowFactory::WindowFactory()
        {
            _blindData = (void*)(new std::map<HWND, std::shared_ptr<Window>>());
        }

        WindowFactory::~WindowFactory()
        {
            if (_blindData) {
                delete WINMAP_CAST_PTR(_blindData);
                _blindData = nullptr;
            }
        }

        std::shared_ptr<Window> WindowFactory::createWindow(
            TCHAR* title,
            unsigned int x,
            unsigned int y,
            unsigned int w,
            unsigned int h,
            bool debug/* = false*/)
        {

            //Register window class
            {
                static bool registered = false;
                if (!registered)
                {
                    WNDCLASSEX wcex;
                    wcex.cbSize = sizeof(WNDCLASSEX);
                    wcex.style = CS_HREDRAW | CS_VREDRAW; // Window style
                    wcex.lpfnWndProc = (WNDPROC)_ZHMsgProc;     // Window message process function ptr
                    wcex.cbClsExtra = 0;                       // 
                    wcex.cbWndExtra = 0;                       // 
                    wcex.hInstance = nullptr;                    //
                    wcex.hIcon = nullptr;//LoadIcon(hInstance, (LPCTSTR)IDI_WIN32TEST);//A handle to the class icon
                    wcex.hCursor = nullptr;// LoadCursor(nullptr, IDC_ARROW);   //A handle to the class cursor
                    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);   //A handle to the class background brush
                    wcex.lpszMenuName = nullptr;                    //
                    wcex.lpszClassName = TEXT("zhanghui");        //
                    wcex.hIconSm = nullptr;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL); 

                    if (!registered && !RegisterClassEx(&wcex))
                    {
                        return false;
                    }
                    else {
                        registered = true;
                    }
                }
            }

            // Adjust window with desired client area size
            RECT windowRect = { 0, 0, (LONG)w, (LONG)h };
            {
                AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
            }

            //Create window
            HWND hWnd = nullptr;
            {
                hWnd = CreateWindow(
                    TEXT("zhanghui"),           //
                    title,                      //
                    WS_OVERLAPPEDWINDOW,        //
                    x,    //
                    y,    //
                    windowRect.right - windowRect.left,    //
                    windowRect.bottom - windowRect.top,    //
                    nullptr, // We have no parent window.
                    nullptr, // We aren't using menus.
                    nullptr, //
                    nullptr  //
                    );

                if (!hWnd)
                {
                    return false;
                }
            }

            // Construct a Window class to hold it
            std::shared_ptr<Window> wndPtr = nullptr;
            {
                WindowsInfo tmpWndInfo;
                {
                    if (title) {
                        strcpy_s(tmpWndInfo.m_title, title);
                    }
                    else {
                        tmpWndInfo.m_title[0] = 0;
                    }
                    tmpWndInfo.m_winHandle = hWnd;
                    tmpWndInfo.m_startPosX = x;
                    tmpWndInfo.m_startPosY = y;
                    tmpWndInfo.m_width = w;
                    tmpWndInfo.m_height = h;
                }

                wndPtr = std::make_shared<Window>(tmpWndInfo, debug);
            }

            // Add Window to map
            WINMAP_CAST_PTR(_blindData)->insert(std::make_pair(hWnd, wndPtr));

            ZH_OUT("A new window created successfully.");

            return wndPtr;
        }

        void WindowFactory::destroyWindow(std::shared_ptr<Window>& w)
        {
            if (w) {
                if (w->m_winInfo.m_winHandle) {
                    DestroyWindow(w->m_winInfo.m_winHandle);
                    w->m_winInfo.m_winHandle = nullptr;
                }

                for (auto& it : *WINMAP_CAST_PTR(_blindData)) {
                    if (it.second == w) {
                        WINMAP_CAST_PTR(_blindData)->erase(it.first);
                        break;
                    }
                }
            }
        }

        std::shared_ptr<Window> WindowFactory::getWindowByHandle(HWND hWnd)
        {
            if (!hWnd)
                return nullptr;

            auto it = WINMAP_CAST_PTR(_blindData)->find(hWnd);
            if (it == WINMAP_CAST_PTR(_blindData)->end()) {
                return nullptr;
            }
            return it->second;
        }





    }

}
