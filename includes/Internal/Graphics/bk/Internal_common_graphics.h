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
#include "Internal/Graphics/AccessInternal.h"


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
#define ASSERT_NOT_NULL_RET_NULL(v) \
    assert((v)); \
    if ( NULL == (v) ) { \
        return NULL; \
    }

// assert, error return false
#define ASSERT_NOT_NULL_RET_FALSE(v) \
    assert((v)); \
    if ( NULL == (v) ) { \
        return false; \
    }

// assert, error return
#define ASSERT_NOT_NULL_RET(v) \
    assert((v)); \
    if ( NULL == (v) ) { \
        return; \
    }

// assert, error return NULL
#define ASSERT_BOOL_RET_NULL(v) \
    assert((v)); \
    if ( !(v) ) { \
        return NULL; \
    }

// assert, error return false
#define ASSERT_BOOL_RET_FALSE(v) \
    assert((v)); \
    if ( !(v) ) { \
        return false; \
    }

// assert, error return
#define ASSERT_BOOL_RET(v) \
    assert((v)); \
    if ( !(v) ) { \
        return; \
    }

// non-empty string check
inline bool non_empty_string( const char* const name )
{
    if ( !name || (strcmp(name,"")==0 ) ){
        return false;
    }

    return true;
}

#endif
