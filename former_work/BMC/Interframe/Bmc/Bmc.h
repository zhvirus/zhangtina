// Bmc.h : main header file for the Bmc application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CBmcApp:
// See Bmc.cpp for the implementation of this class
//

class CBmcApp : public CWinApp
{
public:
	CBmcApp();
	~CBmcApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBmcApp theApp;