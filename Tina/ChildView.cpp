
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include <assert.h>
#include "Tina.h"
#include "ChildView.h"

#include "Widget/Window.h"
#include "Graphics/DeviceDX11.h"
#include "Graphics/Caches.h"
#include "Math/MathCommon.h"

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

}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
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

    if( !m_pDevice->isRunning() ){
        // Try to start device if it is not started yet
        startDevice();
        createDefaultRenderFragment();
    }else{
        // Deal with resizing
    }
}


int CChildView::startDevice(void)
{
    // Get device if not get
    if ( !m_pDevice ){
        // Get device
        m_pDevice = ZH::Graphics::DeviceDX11::instance();
    }

    // Start device if not started
    if ( !m_pDevice->isRunning() ){

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
    }



    return 0;
}


int CChildView::createDefaultRenderFragment(void)
{
    // Default camera
    ZH::Math::float3 pos(0.0f,0.0f, -10.0f);
    ZH::Math::float3 look(0.0f,0.0f,1.0f);
    ZH::Math::float3 up(0.0f,1.0f,0.0f);
    float fovy = ZH::Math::PI/3.0f;
    RECT rct;
    this->GetClientRect(&rct);
    float aspect = static_cast<float>((rct.right - rct.left)/(rct.bottom-rct.top));
    ZH::Graphics::CameraPersp tmpCam( pos, look, up, fovy, aspect, 0.1f, 5000.0f );
    tmpCam.name("DefaultCamera");

    ZH::Graphics::CameraPersp* defaultCamera = ZH::Graphics::ResourceCaches::Instance().CameraPersps().acquire( tmpCam );

    // Default world
    //ZH::Graphics::

    //ZH::Graphics::RenderFragment 

    //ZH::Graphics::Cache<ZH::Graphics::RenderFragment>& rf_caches = ZH::Graphics::ResourceCaches::Instance().RenderFragments();




    return 0;
}
