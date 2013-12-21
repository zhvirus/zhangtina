#pragma once
#include "ZHDecl.h"
#include "ZHSTD.h"

namespace ZH

{


class ZH_DLL ZHLock
{
public:
    ZHLock();
    ~ZHLock();
    void Lock();
    void UnLock();
private:
    CRITICAL_SECTION m_cs;

};


}