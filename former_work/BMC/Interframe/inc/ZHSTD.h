#pragma once 

#include <olectl.h>	
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include <math.h>
#include <time.h>
#include <limits>
#include <assert.h>

#pragma comment(lib,"Winmm.lib")
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
#include <assert.h>
#include <string>

//disable warnings
#pragma warning (disable:4251)
#pragma warning (disable:4996)
#pragma warning (disable:4819)
#pragma warning (disable:4996)
//GDI+
#include	<gdiplus.h>

#pragma comment(lib,"gdiplus.lib")

//process
#include "windows.h"
#include "process.h"

//delete 
template<typename T>
inline void SAFE_DELETE(T p)		{if(p){delete p;p=0;}}

//delete array
template<typename T>
inline void SAFE_DELETE_ARRAY(T p){if(p){delete []p;p=0;}}

template<typename T>
inline void SAFE_RELEASE(T p){if(p){p->Release();}}

inline void ZH_OUT(LPCTSTR message){std::cout<<message<<std::endl;}

inline void ZH_ERR(LPCTSTR message)
{
    std::cout<<"Error:   ";
    ZH_OUT(message);
}