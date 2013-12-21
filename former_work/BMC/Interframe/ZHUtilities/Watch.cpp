#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "Watch.h"

Watch::Watch(void)
{
}

Watch::~Watch(void)
{
}

int Watch::startWatch()
{
    m_nStartTime   =	static_cast<int>(timeGetTime());
    return m_nStartTime;
}

float Watch::endWatch()
{
    return static_cast<float>((static_cast<int>(timeGetTime()) - m_nStartTime)/1000.0f);
}

int Watch::m_nStartTime   =  0;
int Watch::m_nEndTime   =  0;