#ifndef WINDOWS_H
#define WINDOWS_H

#include "Common/ZHSTD.h"
#include "Widget/WindowCallbacks.h"
#include <windows.h>


namespace ZH{
    namespace Widgets{
        class ZH_DLL WindowsInfo
        {
        public:
            WindowsInfo();
            ~WindowsInfo(){}
            HWND         fWndHandle;
            unsigned int fStartPosX;
            unsigned int fStartPosY;
            unsigned int fWidth;
            unsigned int fHeight;
            TCHAR        fWndTitle[100];
        };

        class ZH_DLL Window
        {
        public:
            static Window* CreateZHWindow(
                TCHAR* title,
                unsigned int x,
                unsigned int y,
                unsigned int w,
                unsigned int h);

            static void DestryZHWindow();

            static Window* GetWindow(){ return fWindowPtr; }

        public:
            void showWindow();
            void enterMsgLoop();
            WindowsInfo* getWndInfo(){ return &fWndInfo; }
            void registerWindowResizeFunc( WindowCallbacks::LPRESHAPE funcPtr ){
                fCallbacks.fReShapeFuncPtr = funcPtr;
            }
            void registerKeyboardFunc( WindowCallbacks::LPKEYBOARD funcPtr ){
                fCallbacks.fKeyboardFuncPtr = funcPtr;
            }
            void registerSpecialKeyboardFunc( WindowCallbacks::LPSPECIALKEYBOARD funcPtr ){
                fCallbacks.fSpecialKeyFuncPtr = funcPtr;
            }
            void registerMousePressReleaseFunc( WindowCallbacks::LPMOUSE funcPtr ){
                fCallbacks.fMouseFuncPtr = funcPtr;
            }
            void registerMouseMoveWithButtonPressedFunc( WindowCallbacks::LPMOTION funcPtr ){
                fCallbacks.fMotionFuncPtr = funcPtr;
            }
            void registerMouseMoveFunc( WindowCallbacks::LPPASSIVEMOTION funcPtr ){
                fCallbacks.fPassiveMotionFuncPtr = funcPtr;
            }
            void registerRenderFunc( WindowCallbacks::LPRENDER funcPtr ){
                fCallbacks.fRenderFuncPtr = funcPtr;
            }
        private:
            Window(){}
            virtual ~Window(){}
            static Window* fWindowPtr;
            static LRESULT CALLBACK _ZHMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
            bool createWnd(
                TCHAR* title,
                unsigned int x,
                unsigned int y,
                unsigned int w,
                unsigned int h);

            // Windows information
            WindowsInfo fWndInfo;

            // Callbacks
            WindowCallbacks fCallbacks;

            // Current mouse position
            int fCurMouseX;
            int fCurMouseY;
        };
    }

}

#endif
