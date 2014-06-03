
#pragma once

// Forwards
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

    // Operations
public:

    // Overrides
public:
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

    // Implementation
public:
    virtual ~CTinaXView();

protected:


    // Generated message map functions
protected:
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnOptionClearColor();
    DECLARE_MESSAGE_MAP()
};


