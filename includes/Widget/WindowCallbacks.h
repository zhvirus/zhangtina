#ifndef WINDOWS_CALLBACK_H
#define WINDOWS_CALLBACK_H

#include "Common/ZHSTD.h"
#include <windows.h>

namespace ZH{
    namespace Widgets{

        class ZH_DLL WindowCallbacks
        {
        public:
            typedef void    (*LPRESHAPE)        (int,int);                  // Window resize
            typedef void    (*LPKEYBOARD)       (unsigned char,int, int);   // Keyboard Input
            typedef void    (*LPSPECIALKEYBOARD)(int,int ,int);             // 12 special key input
            typedef void    (*LPMOUSE)          (int,int,int,int);          // LMB, RMB or MMB, pressed/Released, 6 events
            typedef void    (*LPMOTION)         (int,int);                  // Mouse moved with at least one mouse button is pressed
            typedef void    (*LPPASSIVEMOTION)  (int,int);                  // Mouse moved without any mouse button being pressed
            typedef void    (*LPRENDER)();

            WindowCallbacks();
            ~WindowCallbacks();

            void reset(){
                fReShapeFuncPtr         = NULL;
                fKeyboardFuncPtr        = NULL;
                fSpecialKeyFuncPtr      = NULL;
                fMouseFuncPtr           = NULL;
                fMotionFuncPtr          = NULL;
                fPassiveMotionFuncPtr   = NULL;
                fRenderFuncPtr          = NULL;
            }

        public:

            LPRESHAPE           fReShapeFuncPtr;
            LPKEYBOARD          fKeyboardFuncPtr;
            LPSPECIALKEYBOARD   fSpecialKeyFuncPtr;
            LPMOUSE             fMouseFuncPtr;
            LPMOTION            fMotionFuncPtr;
            LPPASSIVEMOTION     fPassiveMotionFuncPtr;
            LPRENDER            fRenderFuncPtr;
        };
    }
}

#endif
