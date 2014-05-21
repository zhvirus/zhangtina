#pragma once


// CTinaView view

class CTinaView : public CView
{
	DECLARE_DYNCREATE(CTinaView)

public:
	CTinaView();           // protected constructor used by dynamic creation
	virtual ~CTinaView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual void OnInitialUpdate();
    virtual void PostNcDestroy(){}
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


