// RayTracing.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRayTracingApp:
// �йش����ʵ�֣������ RayTracing.cpp
//

class CRayTracingApp : public CWinApp
{
public:
	CRayTracingApp();

	~CRayTracingApp();
	
	
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRayTracingApp theApp;