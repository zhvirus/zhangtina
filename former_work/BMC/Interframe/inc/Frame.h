#pragma once

#include "ZHDecl.h"
#include "Common.h"

namespace ZH
{

class ZH_DLL Frame
{
public:
    Frame(void);
public:
    ~Frame(void);
public:
    inline int getFrameWidth()const{return m_width;}
    inline int getFrameHeight()const{return m_height;}
    bool initializeFromFile(const char* colFileName, const char* geoFileName);
    void saveImageToFile(char* imageFileName,float scale =  0.5f);
public:
    PixelInfo *m_geoInfo;
    CCol4*      m_colInfo;
    CCol4*      m_colCIEInfo;
    ImageUtility* m_img;
public:
    int m_width;
    int m_height;
};

//re-sample class
class ZH_DLL reSamplePixel
{
public:
    int m_nFrameNumber;
    int m_nRow;
    int m_nCol;
    int m_nIncreasedSampleNum;
    int m_nNewSampleNum;
    CCol4 m_color;
};


class ZH_DLL FrameForResample
{
    public:
        FrameForResample();
        ~FrameForResample();
    public:
        int m_nFrameIndex;
        float m_fCameraFloat;
        std::vector<reSamplePixel*> m_vPixels;
        std::vector<reSamplePixel*>::iterator m_ite;
};


}