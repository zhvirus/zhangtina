#include <iostream>

#include "Widget\Window.h"
#include "Graphics\Device.h"


ZH::Graphics::Device* pDevice = NULL;
ZH::Widgets::Window* pWin = NULL;

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
    pDevice->clear();
    pDevice->present(*pWin->GetWinInfo());
}


void main()
{
    pWin = ZH::Widgets::Window::Create(
        "jack", 100, 100, 800, 600, true);

    pDevice = &ZH::Graphics::Device::instance();

    if (pDevice->start(*pWin->GetWinInfo()))
    {
        std::cout << "Enging start ok!\n";
    }

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