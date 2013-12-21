// BmcView.cpp : implementation of the CBmcView class
//

#include "stdafx.h"
#include "Bmc.h"

#include "BmcDoc.h"
#include "BmcView.h"
#include ".\bmcview.h"
#include "Arcball.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CArcBall *G_pArcBall;

// CBmcView

IMPLEMENT_DYNCREATE(CBmcView, CView)

BEGIN_MESSAGE_MAP(CBmcView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CBmcView construction/destruction

CBmcView::CBmcView()
{
	// TODO: add construction code here

}

CBmcView::~CBmcView()
{
}

BOOL CBmcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBmcView drawing

void CBmcView::OnDraw(CDC* /*pDC*/)
{
	CBmcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	// TODO: add draw code for native data here
	DrawScene();
}


// CBmcView printing

BOOL CBmcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBmcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBmcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBmcView diagnostics

#ifdef _DEBUG
void CBmcView::AssertValid() const
{
	CView::AssertValid();
}

void CBmcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBmcDoc* CBmcView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBmcDoc)));
	return (CBmcDoc*)m_pDocument;
}
#endif //_DEBUG


// CBmcView message handlers

int CBmcView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	DWORD dwFlags = (PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL);
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // Structure size,
		1,                              // Structure version number
		dwFlags,                        // Property flags
		PFD_TYPE_RGBA,                  // RGBA type 
		24,                             // 24-bit color depth 
		0, 0, 0, 0, 0, 0,               // color bits ignored 
		0,                              // no alpha buffer 
		0,                              // shift bit ignored 
		0,                              // no accumulation buffer 
		0, 0, 0, 0,                     // accum bits ignored 
		32,                             // 32-bit z-buffer     
		0,                              // no stencil buffer 
		0,                              // no auxiliary buffer 
		PFD_MAIN_PLANE,                 // main layer 
		0,                              // reserved 
		0, 0, 0                         // layer masks ignored 
	}; 

	m_hDC = this->GetDC()->m_hDC;
	
	int PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	BOOL success = SetPixelFormat(m_hDC, PixelFormat,&pfd);
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

	return 0;
}

void CBmcView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hRC);	
}

///////////////////////////////////////////////////////////////////////////////
//  Mouse operations
///////////////////////////////////////////////////////////////////////////////

void CBmcView::OnLButtonDown(UINT nFlags, CPoint point)
{
	G_pArcBall->MouseDown(point, m_nWinWidth, m_nWinHeight);
	GetDocument()->Set_mouse_status (MOUSE_LEFT_BUTTON);
	
	CView::OnLButtonDown(nFlags, point);
}

void CBmcView::OnLButtonUp(UINT nFlags, CPoint point)
{
	G_pArcBall->MouseUp(point, m_nWinWidth, m_nWinHeight);
	GetDocument()->Set_mouse_status (NO_BUTTON);
	DrawScene();
	
	CView::OnLButtonUp(nFlags, point);
}

void CBmcView::OnMButtonDown(UINT nFlags, CPoint point)
{
	GetDocument()->Set_mouse_status (MOUSE_MID_BUTTON);
	m_iOldPos = point;

	CView::OnMButtonDown(nFlags, point);
}

void CBmcView::OnMButtonUp(UINT nFlags, CPoint point)
{
	GetDocument()->Set_mouse_status (NO_BUTTON);

	CView::OnMButtonUp(nFlags, point);
}

void CBmcView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_iOldPos = point;
	GetDocument()->Set_mouse_status (MOUSE_RIGHT_BUTTON);
	
	CView::OnRButtonDown(nFlags, point);
}

void CBmcView::OnRButtonUp(UINT nFlags, CPoint point)
{
	GetDocument()->Set_mouse_status (NO_BUTTON);

	CView::OnRButtonUp(nFlags, point);
}

void CBmcView::OnMouseMove(UINT nFlags, CPoint point)
{
	int   nMouseStatus = GetDocument()->Get_mouse_status ();
	UINT  nDownkey     = GetDocument()->Get_key_status ();

	if (nMouseStatus == MOUSE_LEFT_BUTTON)
	{
		G_pArcBall->MouseMove (point, m_nWinWidth, m_nWinHeight);
	}
	else if (nMouseStatus == MOUSE_RIGHT_BUTTON)
	{
		float scale = 2*(point.y - m_iOldPos.y)/(float)m_nWinWidth;
		
		if (nDownkey == VK_CONTROL)
		{
			float fDistance = GetDocument()->Get_distance();
			fDistance = fDistance + scale;
			if (fDistance < .02f) fDistance = .02f;
			if (fDistance > 4.f)  fDistance = 4.f;
			GetDocument()->Set_distance(fDistance);
		}
		else
		{
			float fDistScale = GetDocument()->Get_distance_scale();
			fDistScale = fDistScale + scale;
			if (fDistScale < .02f) fDistScale = .02f;
			if (fDistScale > 4.f)  fDistScale = 4.f;
			GetDocument()->Set_distance_scale(fDistScale);
		}

		m_iOldPos = point;
	}
	else if (nMouseStatus == MOUSE_MID_BUTTON)
	{
		CVec3 vEye   = GetDocument()->Get_eye_pos();
		CVec3 vCent  = GetDocument()->Get_cent_pos();
		CVec3 vTrans = GetDocument()->Get_translate();

		CVec3 dv = CVec3 ( (point.x - m_iOldPos.x)/(float)m_nWinHeight,
					(m_iOldPos.y - point.y)/(float)m_nWinHeight,
					0.f);
		float	*IM, *MM;
		CVec3 vp, vc, vu, vt;
		CVec3 vX, vY;
		
		IM = G_pArcBall->GetInvertedBallMatrix();
		MM = G_pArcBall->GetBallMatrix();
		
		vp.x = IM[0] * vEye.x + IM[4] * vEye.y + IM[8] * vEye.z;
		vp.y = IM[1] * vEye.x + IM[5] * vEye.y + IM[9] * vEye.z;
		vp.z = IM[2] * vEye.x + IM[6] * vEye.y + IM[10]* vEye.z;

		vc.x = IM[0] * vCent.x + IM[4] * vCent.y + IM[8] * vCent.z;
		vc.y = IM[1] * vCent.x + IM[5] * vCent.y + IM[9] * vCent.z;
		vc.z = IM[2] * vCent.x + IM[6] * vCent.y + IM[10]* vCent.z;

		vu.x = IM[4];
		vu.y = IM[5];
		vu.z = IM[6];

		vX = vu.Cross(vp - vc);
		vY = vp - vc;
		vY = vY.Cross(vX);
		vX.Normalize();
		vY.Normalize();

		dv = dv * ( (vp - vc).Length() * 0.5f );
		vt = (vX * dv.x) + (vY * dv.y);

		vTrans.x += MM[0]*vt.x + MM[4]*vt.y + MM[8]*vt.z;
		vTrans.y += MM[1]*vt.x + MM[5]*vt.y + MM[9]*vt.z;
		vTrans.z += MM[2]*vt.x + MM[6]*vt.y + MM[10]*vt.z;
		GetDocument()->Set_translate(vTrans);

		m_iOldPos = point;
	}
	DrawScene();

	CView::OnMouseMove(nFlags, point);
}

void CBmcView::DrawScene(void)
{
	RECT  rect;
	GetClientRect(&rect);
	m_nWinWidth  = rect.right-rect.left;
	m_nWinHeight = rect.bottom-rect.top;
	//while (m_nWinWidth%4) m_nWinWidth --;
	GetDocument()->DrawScene(m_nWinWidth, m_nWinHeight);

	SwapBuffers(m_hDC);
}

void CBmcView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GetDocument()->Set_key_status (nChar);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBmcView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GetDocument()->Set_key_status(NO_BUTTON);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
