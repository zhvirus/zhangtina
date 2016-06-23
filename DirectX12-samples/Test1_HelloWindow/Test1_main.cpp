// DirectX12-samples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Widget\Window.h"

void resize(int w, int h)
{
}

void keyboard(unsigned char c, int x, int y)
{
}

void s_keyboard(int b, int x, int y)
{
}

void mouse(int b, int p, int x, int y)
{
}

void motion(int b, int x, int y)
{
}

void p_motion(int x, int y)
{
}

void render()
{

}

int main()
{
    std::shared_ptr<ZH::Widgets::Window> pWin =
        ZH::Widgets::WindowFactory::instance().createWindow(
        "jack", 100, 100, 800, 600, true);

    pWin->RegisterWindowResizeFunc(resize);
    pWin->RegisterKeyboardFunc(keyboard);
    pWin->RegisterSpecialKeyboardFunc(s_keyboard);
    pWin->RegisterMousePressReleaseFunc(mouse);
    pWin->RegisterMouseMoveWithButtonPressedFunc(motion);
    pWin->RegisterMouseMoveFunc(p_motion);
    pWin->RegisterRenderFunc(render);

    pWin->ShowWindow();
    pWin->EnterMsgLoop();


    ZH::Widgets::WindowFactory::instance().destroyWindow(pWin);

    return 0;
}

