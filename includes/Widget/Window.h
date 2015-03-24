#ifndef WINDOWS_H
#define WINDOWS_H

#include "Common/ZHSTD.h"
#include "Widget/WindowCallbacks.h"
#include <windows.h>


namespace ZH{
    namespace Widgets{
        class ZH_WIDGET_DLL WindowsInfo
        {
        public:
            WindowsInfo();
            ~WindowsInfo(){}
            HWND         m_winHandle;
            unsigned int m_startPosX;
            unsigned int m_startPosY;
            unsigned int m_width;
            unsigned int m_height;
            TCHAR        m_title[100];
        };

        class ZH_WIDGET_DLL Window
        {
        public:
            static Window* Create(
                TCHAR* title,
                unsigned int x,
                unsigned int y,
                unsigned int w,
                unsigned int h,
                bool debug=false);

            static void Destroy(Window*& window);

            void ShowWindow();
            void EnterMsgLoop();

            WindowsInfo* GetWinInfo(){ return &m_winInfo; }

            void RegisterWindowResizeFunc( WindowCallbacks::LPRESHAPE funcPtr ){
                m_callbacks.fReShapeFuncPtr = funcPtr;
            }
            void RegisterKeyboardFunc( WindowCallbacks::LPKEYBOARD funcPtr ){
                m_callbacks.fKeyboardFuncPtr = funcPtr;
            }
            void RegisterSpecialKeyboardFunc( WindowCallbacks::LPSPECIALKEYBOARD funcPtr ){
                m_callbacks.fSpecialKeyFuncPtr = funcPtr;
            }
            void RegisterMousePressReleaseFunc( WindowCallbacks::LPMOUSE funcPtr ){
                m_callbacks.fMouseFuncPtr = funcPtr;
            }
            void RegisterMouseMoveWithButtonPressedFunc( WindowCallbacks::LPMOTION funcPtr ){
                m_callbacks.fMotionFuncPtr = funcPtr;
            }
            void RegisterMouseMoveFunc( WindowCallbacks::LPPASSIVEMOTION funcPtr ){
                m_callbacks.fPassiveMotionFuncPtr = funcPtr;
            }
            void RegisterRenderFunc( WindowCallbacks::LPRENDER funcPtr ){
                m_callbacks.fRenderFuncPtr = funcPtr;
            }
        private:
            static LRESULT CALLBACK _ZHMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
            Window(bool debug);
            virtual ~Window();

            bool CreateWinImp(
                TCHAR* title,
                unsigned int x,
                unsigned int y,
                unsigned int w,
                unsigned int h);

            // Windows information
            WindowsInfo m_winInfo;

            // Callbacks
            WindowCallbacks m_callbacks;

            // Current mouse position
            int m_curMouseX;
            int m_curMouseY;

            // if print some debug info
            bool m_debug;
        };
    }

}

#endif
