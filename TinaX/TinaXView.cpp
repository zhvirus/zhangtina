
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
#include "Util/Print.h"
#include "TinaX_UI_ColorChooser.h"



// CTinaXView

IMPLEMENT_DYNCREATE(CTinaXView, CView)

    BEGIN_MESSAGE_MAP(CTinaXView, CView)
        ON_WM_CONTEXTMENU()
        ON_WM_RBUTTONUP()
        ON_COMMAND(ID_MENUITEM_OPTION_CLEAR_COLOR, &CTinaXView::OnOptionClearColor)
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

    void CTinaXView::OnDraw(CDC* /*pDC*/)
    {
        // Apply setting from preference to the renderer
        Global::instance().applyPrefToRender();

        // Get render fragment
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
                ZH::Util::INF("Device created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
            }else{
                ZH::Util::ERR("Device create failed with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
            }

            // Create default render fragment
            if( Global::instance().createDefaultRenderFragment( winInfo ) ){
                ZH::Util::INF("Default render fragment created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
            }else{
                ZH::Util::ERR("Default render fragment created with win_size(%d,%d)\n",winInfo.fWidth, winInfo.fHeight);
            }
        }
    }

    void CTinaXView::OnOptionClearColor()
    {
        TinaX_UI_ColorChooser cc(m_hWnd);
        ZH::Math::float4 clearColor;
        if ( cc.pickColor( clearColor ) ){
            Global::instance().pref().clearColor( clearColor );
            OnDraw(0);
        }
    }


    // CTinaXView message handlers
