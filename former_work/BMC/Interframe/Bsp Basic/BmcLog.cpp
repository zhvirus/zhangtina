#include "StdAfx.h"
#include "BmcLog.h"
#include <atltime.h>

BmcLog::BmcLog()
{


}

BmcLog::~BmcLog(void)
{

}

char* BmcLog::getCurTime()
{
    CTime   now   =   CTime::GetCurrentTime();   
    CString   s   =   now.Format("%Y-%m-%d-%H-%M-%S");   
    strcpy(curTime,s.GetString());
    return (char*)curTime;
}


bool BmcLog::initialize(char* baseFileName)
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

    return true;
}

void BmcLog::assertError(char* errMessage )
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

void BmcLog::assertMsg(char* message)
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

