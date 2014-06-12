#ifndef INTERNAL_COMMON_GRAPHICS_H
#define INTERNAL_COMMON_GRAPHICS_H

// DX head file
#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>
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

// assert, error return NULL
#define ASSERT_RET_NULL(v) \
    assert((v)); \
    if ( NULL == (v) ) { \
        return NULL; \
    }

// assert, error return false
#define ASSERT_RET_FALSE(v) \
    assert((v)); \
    if ( NULL == (v) ) { \
        return false; \
    }

// assert, error return
#define ASSERT_RET(v) \
    assert((v)); \
    if ( NULL == (v) ) { \
        return; \
    }

#endif
