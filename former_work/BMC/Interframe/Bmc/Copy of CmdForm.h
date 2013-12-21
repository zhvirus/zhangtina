#pragma once


// CCmdForm dialog

class CCmdForm : public CFormView
{
	DECLARE_DYNAMIC(CCmdForm)

public:
	CCmdForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdForm();

// Dialog Data
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
