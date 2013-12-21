#include "StdAfx.h"
#include "CameraInfo.h"

CameraInfo::CameraInfo(void)
{
}

CameraInfo::~CameraInfo(void)
{
}

float* CameraInfo::getBuffer()
{
    return static_cast<float*>(m_rotateMatrix);
}

void CameraInfo::setBuffer(float* _buf, int _len)
{
    memcpy((void*)(m_rotateMatrix),(void*)(_buf), _len * sizeof(float));

}