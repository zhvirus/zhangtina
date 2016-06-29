#ifndef WINDOWS_H
#define WINDOWS_H

#include "Common/ZHSTD.h"

#include <map>
#include <windows.h>

#include "Widget/WindowCallbacks.h"
#include "Widget/FPSHelper.h"


namespace ZH{
    namespace Widgets{


        class ZH_WIDGET_DLL WindowsInfo
        {
        public:
            WindowsInfo() = default;
            ~WindowsInfo() = default;
            HWND         m_winHandle{nullptr};
            unsigned int m_startPosX{0};
            unsigned int m_startPosY{0};
            unsigned int m_width{0};
            unsigned int m_height{0};
            TCHAR        m_title[100];
        };


        class ZH_WIDGET_DLL Window
        {
        public:
            Window(WindowsInfo& winInfo, bool debug);

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

            // Windows information
            WindowsInfo m_winInfo;

            // Callbacks
            WindowCallbacks m_callbacks;

            // Current mouse position
            int m_curMouseX{0};
            int m_curMouseY{0};

            // FPS helper
            FPSHelper m_fps;

            // if print some debug info
            bool m_debug;

            friend class WindowFactory;
        };

        /*
            This class is the main entry to create/query/destroy windows.
        */
        class ZH_WIDGET_DLL WindowFactory
        {
        public:
            static WindowFactory& instance();

            std::shared_ptr<Window> createWindow(
                TCHAR* title,
                unsigned int x,
                unsigned int y,
                unsigned int w,
                unsigned int h,
                bool debug = false);

            void destroyWindow(std::shared_ptr<Window>& w);

            std::shared_ptr<Window> getWindowByHandle(HWND hWnd);

        private:
            static LRESULT CALLBACK _ZHMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
            void* _blindData{nullptr};
        private:
            WindowFactory();
            ~WindowFactory();
            void operator = (WindowFactory&){}
        };



    }

}

#endif
