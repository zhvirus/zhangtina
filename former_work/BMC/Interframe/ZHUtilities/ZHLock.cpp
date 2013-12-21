#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "ZHLock.h"

namespace ZH
{

    ZHLock::ZHLock()
    {
        InitializeCriticalSection(&m_cs);
    }

    ZHLock::~ZHLock()
    {
        DeleteCriticalSection(&m_cs);

    }

    void ZHLock::Lock()
    {
        EnterCriticalSection(&m_cs);
    }

    void ZHLock::UnLock()
    {
        LeaveCriticalSection(&m_cs);

    }



}