
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include <assert.h>
#include "Tina.h"
#include "ChildView.h"

#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/Caches.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView():
    m_pDevice( NULL )
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}



void CChildView::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    //if(gDXContext){
    //    gDXContext->OMSetRenderTargets( 1, &gDXRenderTargetView, NULL );

    //    // Just clear the backbuffer
    //    float ClearColor[4] = { 0.3f, 0.3f, 0.0f, 1.0f }; //red,green,blue,alpha
    //    gDXContext->ClearRenderTargetView( gDXRenderTargetView, ClearColor );
    //}

    //if ( gDXSwapChain )
    //    gDXSwapChain->Present( 0, 0 );
    ZH::Graphics::Cache<ZH::Graphics::Texture2D>& caches = ZH::Graphics::ResourceCaches::Instance().Textures();
    caches.findByName("jack");
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Get device
    m_pDevice = ZH::Graphics::DeviceDX11::instance();

    return 0;
}


void CChildView::OnDestroy()
{
    if( m_pDevice ){
        m_pDevice->shutdown();
        m_pDevice = NULL;
    }
    CWnd::OnDestroy();
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // Get window size
    RECT rct;
    this->GetClientRect(&rct);

    // Start the device if it is not started
    if ( (!m_pDevice||!m_pDevice->isRunning()) &&
        (rct.right - rct.left > 0) &&
        (rct.bottom - rct.top > 0))
    {
        // Initialize device
        ZH::Widgets::WindowsInfo winInfo;
        winInfo.fStartPosX = static_cast<unsigned int>(rct.left);
        winInfo.fStartPosY = static_cast<unsigned int>(rct.top);
        winInfo.fWidth     = static_cast<unsigned int>(rct.right - rct.left);
        winInfo.fHeight    = static_cast<unsigned int>(rct.bottom - rct.top);
        winInfo.fWndHandle = this->GetSafeHwnd();

        // Get device when needed
        if ( !m_pDevice ){
            m_pDevice = ZH::Graphics::DeviceDX11::instance();
        }

        // Start device
        m_pDevice->start( &winInfo );

        //// Create a render target view
        //ID3D11Texture2D* pBackBuffer = NULL;
        //HRESULT hr = gDXSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
        //if( FAILED( hr ) )
        //    return;

        //hr = gDXDevice->CreateRenderTargetView( pBackBuffer, NULL, &gDXRenderTargetView );
        //pBackBuffer->Release();
        //if( FAILED( hr ) )
        //    return;
    }

    if( !m_pDevice || !m_pDevice->isRunning() ){
        return;
    }

    // Deal with resizing

}
