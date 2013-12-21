#pragma once 


//����_ZH_DLL������������������
//���������֮����ÿ����Ҫ������cpp�ļ�����_ZH_DLL
//�����ͻ�ʹ�õ�ʱ�� ֻҪ������_ZH_DLL��������
#ifdef _ZH_DLL
#define ZH_DLL _declspec(dllexport)
#else
#define ZH_DLL _declspec(dllimport)
#endif

#include <olectl.h>	
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include <math.h>
#include <time.h>
#include <limits>
#include <assert.h>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <hash_map>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <algorithm>
#include <functional>



//disable warnings
#pragma warning (disable:4251)
#pragma warning (disable:4996)
#pragma warning (disable:4819)

// include ��ѧ��
#include "..\\..\\Math\\src\\ZHMath.h"
#pragma comment(lib,"..\\Math\\Debug\\ZHMath.lib")



// DirectX Related
// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

// Direct3D11 includes
#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11.h>

// XInput includes
#include <xinput.h>

// HRESULT translation for Direct3D and other APIs 
#include <dxerr.h>

//time
#pragma comment(lib,"winmm.lib")


//GDI+
#include	<gdiplus.h>

#pragma comment(lib,"gdiplus.lib")

//�߳�
#include <process.h>