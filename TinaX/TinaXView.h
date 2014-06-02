
// TinaXView.h : interface of the CTinaXView class
//

#pragma once

namespace ZH{
    namespace Graphics{
        class RenderFragment;
    }
}


class CTinaXView : public CView
{
protected: // create from serialization only
	CTinaXView();
	DECLARE_DYNCREATE(CTinaXView)

// Attributes
public:
	CTinaXDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTinaXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void applyPrefToRender( ZH::Graphics::RenderFragment* );

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnOptionClearColor();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TinaXView.cpp
inline CTinaXDoc* CTinaXView::GetDocument() const
   { return reinterpret_cast<CTinaXDoc*>(m_pDocument); }
#endif

