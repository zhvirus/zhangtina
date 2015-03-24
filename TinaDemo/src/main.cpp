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


void main()
{
    ZH::Widgets::Window* pWin = ZH::Widgets::Window::Create(
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


    ZH::Widgets::Window::Destroy(pWin);
}