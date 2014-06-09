#ifndef INTERNAL_COMMON_GRAPHICS_H
#define INTERNAL_COMMON_GRAPHICS_H

// DX head file
#include <d3d11.h>
#include <dxgi.h>
#include <iostream>
#include <string>
#include <assert.h>

#include "Util/Print.h"


// Safe delete
template<typename T>
inline void SAFE_DELETE(T& p) {delete p;p=0;}

// Safe delete array
template<typename T>
inline void SAFE_DELETE_ARRAY(T& p){delete []p;p=0;}

// Safe release
template<typename T>
inline void SAFE_RELEASE(T& p){if(p){p->Release();}}

#endif
