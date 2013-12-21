#pragma once
#include "ZHDecl.h"

class ZH_DLL Watch
{
public:
    Watch(void);
public:
    ~Watch(void);
    static int startWatch();
    static float endWatch();
private:
	static int m_nStartTime;
    static int m_nEndTime;

};
