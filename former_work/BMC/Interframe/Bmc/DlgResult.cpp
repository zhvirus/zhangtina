// DlgResult.cpp : implementation file
//

#include "stdafx.h"
#include "Bmc.h"
#include "DlgResult.h"
#include ".\dlgresult.h"
#include "Image.h"
#include "Singleton.h"

// CDlgResult dialog

IMPLEMENT_DYNAMIC(CDlgResult, CDialog)
CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResult::IDD, pParent)
{
	m_nWidth = m_nHeight = 0;
	m_nWinWidth = m_nWinHeight = 0;

	m_pImage = NULL;
	m_fBriScale = 1.f;
	
	m_BmpInfo.biSize			= sizeof(BITMAPINFOHEADER);
	m_BmpInfo.biPlanes			= 1; 
	m_BmpInfo.biBitCount		= 24;
	m_BmpInfo.biCompression		= 0; 
	m_BmpInfo.biSizeImage		= 0; 
	m_BmpInfo.biXPelsPerMeter	= 0; 
	m_BmpInfo.biYPelsPerMeter	= 0; 
	m_BmpInfo.biClrUsed			= 0; 
	m_BmpInfo.biClrImportant	= 0;
}

CDlgResult::~CDlgResult()
{
	if (m_pImage != NULL)  delete [] m_pImage;
}

void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgResult, CDialog)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CDlgResult message handlers

void CDlgResult::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetBkMode(TRANSPARENT);
	if (m_nHeight <= 0)
	{
		dc.TextOut(10, 0, " No result image. ");
		return;
	}

	::StretchDIBits(dc.GetSafeHdc(),
		0, 0, m_nWinWidth, m_nWinHeight, // Destination
		0, 0, m_nWidth, m_nHeight, // Source
		m_pImage, (BITMAPINFO*)&m_BmpInfo,
		DIB_RGB_COLORS, SRCCOPY);

	// dc.SetTextColor(RGB(255-m_pImage[0], 255-m_pImage[1], 255-m_pImage[2]));
	// dc.TextOut(m_nWidth - 180, m_nHeight - 25, "Microsoft Research, China");
	// Do not call CDialog::OnPaint() for painting messages
}


void CDlgResult::Set_image(int Width, int Height, CCol4 *pCol)
{
	//std::cout<<"CDlgResult::Set_image"<<std::endl;
	int nLineWidth, x, y, index;

	if ( (Width <= 0) || (Height <= 0)||pCol==NULL )
		return;

	if (m_pImage != NULL)  delete [] m_pImage;

	m_nWidth     = Width;
	m_nHeight    = Height;

	if(m_nWinWidth==0)
		m_nWinWidth  = Width;
	if(m_nWinHeight==0)
		m_nWinHeight = Height;
	
	m_BmpInfo.biWidth	= m_nWidth;
	m_BmpInfo.biHeight	= m_nHeight;

	// resize the window
	RECT rectW, rectC;
	GetWindowRect (&rectW);
	GetClientRect (&rectC);
	MoveWindow (rectW.left, rectW.top,
		rectW.right  - rectW.left + (m_nWinWidth  - (rectC.right - rectC.left)),
		rectW.bottom - rectW.top  + (m_nWinHeight - (rectC.bottom - rectC.top)));


	nLineWidth = m_nWidth * 3;
	while (nLineWidth%4)	nLineWidth++;

	m_pImage = new BYTE [nLineWidth * Height];

	float fScale = 255 * m_fBriScale;
	for (y = Height - 1; y >= 0 ; y --)
	{
		index = y * nLineWidth;
		for (x = 0; x < Width; x ++)
		{
			float r = pCol->r() * fScale;
			float g = pCol->g() * fScale;
			float b = pCol->b() * fScale;
			
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			m_pImage [index]   = (BYTE) b;
			m_pImage [index+1] = (BYTE) g;
			m_pImage [index+2] = (BYTE) r;

			index += 3;
			pCol ++;
		}
	}
}


void CDlgResult::Set_brightness_scale (const float &fScale)
{
	m_fBriScale = fScale;
}


void CDlgResult::Save_image_2_file()
{
	// If the image has not been synthesized
	if ( (m_pImage == NULL) || (m_nWidth <= 0) || (m_nHeight <= 0) )
		return;

	// Image file name
	char	fn[256];
	CFileDialog myfiledlg(FALSE, "bmp", NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
		"Windows BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||", NULL);
	if ( myfiledlg.DoModal() == IDOK)
	{
		sprintf( fn, "%s", myfiledlg.GetPathName() );
		if( strlen( fn ) == 0 )
		{
			MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else	return;

	// Save the image
	BITMAPFILEHEADER head;
	FILE	*fp;
	int	nLineWidth;

	fp=fopen(fn,"wb");
	if(fp==NULL)
	{
		MessageBox("Can't Create New File", "Error", MB_OK|MB_ICONERROR);
		return;
	}
	
	head.bfType	= 0x4d42;
	head.bfSize	= sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+m_nWidth*m_nHeight*3; 
	head.bfReserved1= 0;
	head.bfReserved2= 0;
	head.bfOffBits	= sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	nLineWidth = m_nWidth*3;
	while (nLineWidth%4) nLineWidth++;

	fwrite(&head,sizeof(BITMAPFILEHEADER),1,fp);
	fwrite(&m_BmpInfo,sizeof(BITMAPINFOHEADER),1,fp);
	fwrite(m_pImage, sizeof(BYTE), nLineWidth*m_nHeight, fp);

	fclose(fp);
}


BOOL CDlgResult::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta < 0)
	{
		m_nWinWidth += m_nWidth;
		m_nWinHeight+= m_nHeight;
	}
	else
	{
		if (m_nWinWidth > m_nWidth)
		{
			m_nWinWidth -= m_nWidth;
			m_nWinHeight-= m_nHeight;
		}
	}

	RECT rectW, rectC;
	GetWindowRect (&rectW);
	GetClientRect (&rectC);
	MoveWindow (rectW.left, rectW.top,
		rectW.right  - rectW.left + (m_nWinWidth  - (rectC.right - rectC.left)),
		rectW.bottom - rectW.top  + (m_nWinHeight - (rectC.bottom - rectC.top)));
	Invalidate();

	char sTitle[100];
	sprintf (sTitle, "Result (%d:x%d) %d:1",
		m_nWidth, m_nHeight, m_nWinWidth/m_nWidth);
	SetWindowText (sTitle);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgResult::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 83)	Save_image_2_file();
	if(nChar	 ==80)
	{
		Singleton<ImageUtility>::getSingletonPtr()->saveToFileFromChar("c:\\char1.bmp",m_pImage,m_nWidth,m_nHeight);
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CDlgResult::OnInitDialog()
{
	SetTimer(2,1000,0);
	return CDialog::OnInitDialog();

}

void CDlgResult::OnTimer(UINT nIDEvent)
{

	Invalidate(false);
	SetTimer(2,1000,0);
}