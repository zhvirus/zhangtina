// BmcView.h : interface of the CBmcView class
//


#pragma once

#include "common.h"

class CBmcDoc;
class CBmcView : public CView
{
protected: // create from serialization only
	CBmcView();
	DECLARE_DYNCREATE(CBmcView)

protected:
	HGLRC  m_hRC;
	HDC	   m_hDC;
	CPoint m_iOldPos;
	int    m_nWinWidth, m_nWinHeight;

// Attributes
public:
	CBmcDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBmcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void DrawScene();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // debug version in BmcView.cpp
inline CBmcDoc* CBmcView::GetDocument() const
   { return reinterpret_cast<CBmcDoc*>(m_pDocument); }
#endif

