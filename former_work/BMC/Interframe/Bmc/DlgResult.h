#pragma once
#include "common.h"

// CDlgResult dialog

class CDlgResult : public CDialog
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgResult();

// Dialog Data
	enum { IDD = IDD_RESULT_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	int m_nWidth;
	int m_nHeight;
	float m_fBriScale;
	unsigned char *m_pImage;
	BITMAPINFOHEADER m_BmpInfo;

	int m_nWinWidth;
	int m_nWinHeight;

public:
	void Set_image(int Width, int Height, CCol4 *pCol);
	void Set_brightness_scale (const float &fScale);
	void Save_image_2_file();

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnInitDialog();

};
