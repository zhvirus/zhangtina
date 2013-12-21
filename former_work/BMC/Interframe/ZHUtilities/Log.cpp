#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "Log.h"
#include <atltime.h>

namespace ZH
{


    ZHLog::ZHLog():
    m_bInitialized(false)
{


}

ZHLog::~ZHLog(void)
{

}

char* ZHLog::getCurTime()
{
    CTime   now   =   CTime::GetCurrentTime();   
    CString   s   =   now.Format("%Y-%m-%d-%H-%M-%S");   
    strcpy(curTime,s.GetString());
    return (char*)curTime;
}


bool ZHLog::initialize(char* baseFileName)
{
    if(!m_bInitialized)
    {

        if(baseFileName==NULL)
        {
            return false;    
        }

        //get current time
        sprintf(m_fileName,"%s%s.txt",baseFileName,getCurTime());
      
        std::ofstream ooFile(m_fileName,std::ios::out);
        if(ooFile.fail())
            return false;
        ooFile.close();
        m_bInitialized   =  true;
    }

    return true;
}

void ZHLog::assertError(char* errMessage )
{
    if(errMessage==NULL)
        return;

    aaFile.open(m_fileName,std::ios::app);
    if(aaFile.fail())
        return;

    sprintf(tempMessage,"ERROR: %s\n%s",getCurTime(),errMessage);
    
    aaFile<<tempMessage<<std::endl<<std::endl;

    aaFile.close();
    
}

void ZHLog::assertMsg(char* message)
{
    if(message==NULL)
        return;

    aaFile.open(m_fileName,std::ios::app);
    if(aaFile.fail())
        return;

    sprintf(tempMessage,"INFOMATION: %s\n%s",getCurTime(),message);

    aaFile<<tempMessage<<std::endl<<std::endl;

    aaFile.close();


}

}
