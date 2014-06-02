#include "stdafx.h"
#include "TinaX_UI_ColorChooser.h"
#include "Math/float4.h"


TinaX_UI_ColorChooser::TinaX_UI_ColorChooser( HWND hWin ):
    m_wnd( hWin )
{
}


TinaX_UI_ColorChooser::~TinaX_UI_ColorChooser(void)
{
}

bool TinaX_UI_ColorChooser::pickColor( ZH::Math::float4& retColor ){

    CHOOSECOLOR cc;
    COLORREF crCustColors[16];
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = m_wnd;
    cc.rgbResult = 0xFFFFFF;
    cc.Flags = CC_FULLOPEN|CC_RGBINIT;
    cc.lCustData = 0;
    cc.lpfnHook = NULL;
    cc.hInstance = NULL;
    cc.lpCustColors = crCustColors;
    cc.lpTemplateName = NULL;
    if (ChooseColor(&cc))
    {
        int nR = GetRValue(cc.rgbResult);
        int nG = GetGValue(cc.rgbResult);
        int nB = GetBValue(cc.rgbResult);

        retColor.r = nR/255.0f;
        retColor.g = nG/255.0f;
        retColor.b = nB/255.0f;

        return true;
    }

    return false;
}

