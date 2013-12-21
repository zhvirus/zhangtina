#pragma once
#include "ZHDecl.h"
#include "Common.h"
#include <fstream>

namespace ZH
{


class ZH_DLL ZHLog
{
public:
    ZHLog();
public:
    ~ZHLog(void);
    bool initialize(char* baseFileName);
    void assertError(char* errMessage);
    void assertMsg(char* message);
    char* getCurTime();
    inline bool ifIntialized()const{return m_bInitialized;}

private:
    char m_fileName[MAX_PATH];
    char tempMessage[500];
    std::ofstream aaFile;
    char curTime[50];
    bool m_bInitialized;
};

}