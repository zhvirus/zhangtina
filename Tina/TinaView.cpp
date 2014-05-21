// TinaView.cpp : implementation file
//

#include "stdafx.h"
#include "Tina.h"
#include "TinaView.h"
#include <iostream>

#include "Global.h"
#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"

// CTinaView

IMPLEMENT_DYNCREATE(CTinaView, CView)

CTinaView::CTinaView()
{

}

CTinaView::~CTinaView()
{
}

BEGIN_MESSAGE_MAP(CTinaView, CView)
END_MESSAGE_MAP()


// CTinaView drawing

void CTinaView::OnDraw(CDC* /*pDC*/)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

void CTinaView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Get window size
    RECT rct;
    this->GetClientRect(&rct);

    if ( (rct.right - rct.left > 0) && (rct.bottom - rct.top > 0)){
        // Initialize device
        ZH::Widgets::WindowsInfo winInfo;
        winInfo.fStartPosX = static_cast<unsigned int>(rct.left);
        winInfo.fStartPosY = static_cast<unsigned int>(rct.top);
        winInfo.fWidth     = static_cast<unsigned int>(rct.right - rct.left);
        winInfo.fHeight    = static_cast<unsigned int>(rct.bottom - rct.top);
        winInfo.fWndHandle = this->GetSafeHwnd();

        if( Global::instance().startDevice( winInfo ) ){
            Global::print("Device created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
        }else{
            Global::error("Device create failed with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
        }
    }

}


// CTinaView diagnostics

#ifdef _DEBUG
void CTinaView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTinaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTinaView message handlers
