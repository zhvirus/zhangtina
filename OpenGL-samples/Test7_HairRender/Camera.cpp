#include "stdafx.h"
#include "Camera.h"
#include "Math/MathCommon.h"


//------------------------------------------------
//
//        Camera
//
//------------------------------------------------


Camera::Camera() :
m_pos(10.0f, 10.0f, 10.0f),
m_lookDir(-1.0f, -1.0f, -1.0f),
m_upDir(0.0f, 1.0f, 0.0f),
m_viewMatDirtyFlag(true),
m_projMatDirtyFlag(true)
{
    m_viewMat.identityIt();
    m_projMat.identityIt();
}

Camera::Camera(ZH::Math::float3 pos, ZH::Math::float3 look, ZH::Math::float3 up) :
m_pos(pos),
m_lookDir(look),
m_upDir(up),
m_viewMatDirtyFlag(true),
m_projMatDirtyFlag(true)
{
    m_viewMat.identityIt();
    m_projMat.identityIt();
}

Camera::Camera(const Camera& v) :
m_pos(v.m_pos),
m_lookDir(v.m_lookDir),
m_upDir(v.m_upDir),
m_viewMatDirtyFlag(v.m_viewMatDirtyFlag),
m_projMatDirtyFlag(v.m_projMatDirtyFlag),
m_viewMat(v.m_viewMat),
m_projMat(v.m_projMat)
{
}

Camera::~Camera()
{
}

void Camera::updateViewMat()
{
    if (!m_viewMatDirtyFlag)
        return;

    // OpenGL RH
    ZH::Math::float3 zaxis = m_lookDir.normalize();
    ZH::Math::float3 xaxis = m_upDir.cross(zaxis).normalize();
    ZH::Math::float3 yaxis = zaxis.cross(xaxis);

    float viewMat[] = {
        xaxis.x, yaxis.x, -zaxis.x, 0.0f,
        xaxis.y, yaxis.y, -zaxis.y, 0.0f,
        xaxis.z, yaxis.z, -zaxis.z, 0.0f,
        -xaxis.dot(m_pos), -yaxis.dot(m_pos), -zaxis.dot(m_pos), 1.0f
    };
    m_viewMat = viewMat;

    m_viewMatDirtyFlag = false;
}

ZH::Math::matrix4x4_f Camera::viewMatrix()
{
    updateViewMat();
    return m_viewMat;
}

ZH::Math::matrix4x4_f Camera::projMatrix()
{
    updateProjMat();
    return m_projMat;
}

//------------------------------------------------
//
//        CameraPersp
//
//------------------------------------------------

const char CameraPersp::m_sDefaultName[] = "DefaultCameraPersp";

CameraPersp::CameraPersp() :
m_fovy(ZH::Math::PI / 3.0f),
m_aspect(1.0f),
m_nearZ(0.1f),
m_farZ(50000.0f)
{
}

CameraPersp::CameraPersp(ZH::Math::float3 pos, ZH::Math::float3 look, ZH::Math::float3 up, float fovy, float aspect, float nz, float fz) :
Camera(pos, look, up),
m_fovy(fovy),
m_aspect(aspect),
m_nearZ(nz),
m_farZ(fz)
{
}

CameraPersp::CameraPersp(const CameraPersp& v) :
m_fovy(v.m_fovy),
m_aspect(v.m_aspect),
m_nearZ(v.m_nearZ),
m_farZ(v.m_farZ)
{
}

void CameraPersp::updateProjMat()
{
    if (!m_projMatDirtyFlag)
        return;

    // Learned from 'D3DXMatrixPerspectiveFovLH'
    const float yScale = 1.0f / tan(m_fovy / 2);
    const float xScale = yScale / m_aspect;

    float projMat[] = {
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, m_farZ / (m_farZ - m_nearZ), 1.0f,
        0.0f, 0.0f, -m_nearZ*m_farZ / (m_farZ - m_nearZ), 0.0f
    };

    m_projMat = projMat;

    m_projMatDirtyFlag = false;
}




