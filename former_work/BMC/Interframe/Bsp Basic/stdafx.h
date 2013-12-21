// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components

// TODO: reference additional headers your program requires here
#pragma warning(disable : 4996)

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <olectl.h>

//GDI+
#include	<gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include <string>
#include "ZHUT.h"
using namespace ZH;