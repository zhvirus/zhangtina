
// ChildView.h : interface of the CChildView class
//


#pragma once

// Forwards
namespace ZH{
    namespace Graphics{
        class Device;
    }
}

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
    //virtual void OnDraw(CDC *);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
private:
    ZH::Graphics::Device* m_pDevice;
public:
    int startDevice(void);
    int createDefaultRenderFragment(void);
};

