// CmdForm.cpp : implementation file
//

#include "stdafx.h"
#include "Bmc.h"
#include "CmdForm.h"


// CCmdForm dialog

IMPLEMENT_DYNAMIC(CCmdForm, CFormView)
CCmdForm::CCmdForm(CWnd* pParent /*=NULL*/)
	: CFormView(CCmdForm::IDD)
{
}

CCmdForm::~CCmdForm()
{
}

void CCmdForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCmdForm, CFormView)
END_MESSAGE_MAP()


// CCmdForm message handlers
