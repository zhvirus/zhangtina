#pragma once

namespace ZH{
    namespace Math{
        class float4;
    }
}

class TinaX_UI_ColorChooser
{
public:
    TinaX_UI_ColorChooser(HWND);
    ~TinaX_UI_ColorChooser(void);

    bool pickColor( ZH::Math::float4& );
private:
    HWND m_wnd;
};

