
// TinaXView.cpp : implementation of the CTinaXView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TinaX.h"
#endif

#include "TinaXDoc.h"
#include "TinaXView.h"

#include "Global.h"
#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/ResourceManager.h"
#include "Math/MathCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTinaXView

IMPLEMENT_DYNCREATE(CTinaXView, CView)

BEGIN_MESSAGE_MAP(CTinaXView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTinaXView construction/destruction

CTinaXView::CTinaXView()
{
	// TODO: add construction code here

}

CTinaXView::~CTinaXView()
{
}

BOOL CTinaXView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTinaXView drawing

void CTinaXView::OnDraw(CDC* /*pDC*/)
{
	CTinaXDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    ZH::Graphics::RenderFragment* rf = Global::instance().defaultRenderFragment();
    if ( rf ){
        rf->render();
    }
}

void CTinaXView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTinaXView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CTinaXView::OnInitialUpdate()
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

        // Start device
        if( Global::instance().startDevice( winInfo ) ){
            Global::print("Device created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
        }else{
            Global::error("Device create failed with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
        }

        // Create default render fragment
        if( Global::instance().createDefaultRenderFragment( winInfo ) ){
            Global::print("Default render fragment created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
        }else{
            Global::error("Default render fragment created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
        }
    }

}


// CTinaXView diagnostics

#ifdef _DEBUG
void CTinaXView::AssertValid() const
{
	CView::AssertValid();
}

void CTinaXView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTinaXDoc* CTinaXView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTinaXDoc)));
	return (CTinaXDoc*)m_pDocument;
}
#endif //_DEBUG


// CTinaXView message handlers
