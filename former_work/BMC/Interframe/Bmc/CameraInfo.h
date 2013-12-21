#pragma once
#include "ArcBall.h"
#include "Common.h"

const int CAMERA_BUFFER_SIZE = 27;

class CameraInfo
{
public:
    CameraInfo(void);
public:
    ~CameraInfo(void);

public:
    CArcBall    m_archBall;
    float  m_rotateMatrix[16];
    float  m_fDistScale;
    float  m_fDistance;
    CVec3   m_vTrans;
    CVec3   m_vCent;
    CVec3   m_vEye;
public:
    float*    getBuffer();
    void      setBuffer(float* _buf, int _len);
    int getBufferSizeInFloat() {return CAMERA_BUFFER_SIZE;}
};
