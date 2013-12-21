#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "Frame.h"
#include "PixelInfo.h"
#include "Image.h"
#include "Singleton.h"

namespace ZH
{


Frame::Frame(void):
m_geoInfo(NULL),
m_colInfo(NULL),
m_colCIEInfo(NULL),
m_img(NULL)
{
    m_geoInfo   = new PixelInfo(256,256);
    m_img  =  Singleton<ImageUtility>::getSingletonPtr();
}

Frame::~Frame(void)
{

    SAFE_DELETE(m_geoInfo);
    SAFE_DELETE_ARRAY(m_colInfo);
    SAFE_DELETE_ARRAY(m_colCIEInfo);
}

bool Frame::initializeFromFile(const char* colFileName, const char* geoFileName)
{
    if(colFileName==NULL||geoFileName==NULL)
        return false;
    if(!m_geoInfo)
        return false;
    if(m_geoInfo->loadPixelInfoFromFile(const_cast<char*>(geoFileName))==false)
        return false;

    if(m_geoInfo->m_width<=0||m_geoInfo->m_height<=0)
        return false;

    //=================================== read col data    
    std::ifstream iiFile(colFileName,std::ios::binary);
    if(iiFile.fail())
        return false;

    //prepare memory
    int t_widthHeight[2];
    //read width and height
    iiFile.read((char*)(t_widthHeight),2*sizeof(int));
    if((t_widthHeight[0]!=(m_geoInfo->m_width))||(t_widthHeight[1]!=(m_geoInfo->m_height)))
        return false;
    
    if(m_width*m_height<t_widthHeight[0]*t_widthHeight[1]||m_colInfo==NULL)
    {
        SAFE_DELETE_ARRAY(m_colInfo);
        SAFE_DELETE_ARRAY(m_colCIEInfo);

        m_colInfo	=	new CColor4f[t_widthHeight[0] * t_widthHeight[1] ];
        m_colCIEInfo = new CColor4f[t_widthHeight[0] * t_widthHeight[1] ]; 
        if(m_colInfo==NULL||m_colCIEInfo==NULL)
            return false;

        memset(m_colInfo, 0, sizeof(CColor4f) * t_widthHeight[0] * t_widthHeight[1]);
        memset(m_colCIEInfo, 0, sizeof(CColor4f) * t_widthHeight[0] * t_widthHeight[1]);

    }
    else
    {
        memset(m_colInfo, 0, sizeof(CColor4f) * m_width * m_height);
        memset(m_colCIEInfo, 0, sizeof(CColor4f) * m_width * m_height);

    }

    m_width	=	t_widthHeight[0];
    m_height	=	t_widthHeight[1];

    //read data
    iiFile.read((char*)(m_colInfo), sizeof(CColor4f) * m_width * m_height);
    m_img->RGB_to_CIELAB(m_colInfo,m_colCIEInfo,m_width,m_height);
    return true;

}

void Frame::saveImageToFile(char* imageFileName,float scale)
{
    if(imageFileName==NULL)
        return;

    if(!m_img)
        return;

    m_img->saveToFileFromColor4f(imageFileName,m_colInfo,m_width,m_height,scale);


}


//class FrameForResample
FrameForResample::FrameForResample():
m_nFrameIndex(0),
m_fCameraFloat(0.0f)

{

}

FrameForResample::~FrameForResample()
{
    
    for(m_ite = m_vPixels.begin();m_ite!=m_vPixels.end();m_ite++)
    {
        SAFE_DELETE(*m_ite);
    }
    m_vPixels.clear();


}


}
