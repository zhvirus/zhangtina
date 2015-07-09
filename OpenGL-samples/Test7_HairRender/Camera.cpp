#include "stdafx.h"
#include "Camera.h"
#include "Math/MathCommon.h"


//------------------------------------------------
//
//        Camera
//
//------------------------------------------------


Camera::Camera() :
m_pos(0.0f, 0.0f, 3.0f),
m_lookDir(0.0f, 0.0f, -1.0f),
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
    ZH::Math::float3 xaxis = zaxis.cross(m_upDir).normalize();
    ZH::Math::float3 yaxis = xaxis.cross(zaxis);

    float viewMat[] = {
        xaxis.x, xaxis.y, xaxis.z, -xaxis.dot(m_pos),
        yaxis.x, yaxis.y, yaxis.z, -yaxis.dot(m_pos),
        -zaxis.x, -zaxis.y, -zaxis.z, -zaxis.dot(m_pos),
        0.0f, 0.0f, 0.0f, 1.0f
    };
    m_viewMat = viewMat;

    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(m_pos.x, m_pos.y, m_pos.z, 0, 0, 0, 0, 1, 0);
        float model[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, model);
        glLoadIdentity();
        //m_viewMat = model;
    }

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
m_farZ(5000.0f)
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

    const float half_h = m_nearZ * tan(m_fovy / 2);
    const float half_w = half_h * m_aspect;

    float projMat[] = {
        m_nearZ / half_w, 0.0f, 0.0f, 0.0f,
        0.0f, m_nearZ/half_h, 0.0f, 0.0f,
        0.0f, 0.0f, (m_farZ + m_nearZ) / (m_nearZ - m_farZ), 2 * m_nearZ*m_farZ / (m_farZ - m_nearZ),
        0.0f, 0.0f, -1.0f, 0.0f
    };
    m_projMat = projMat;

    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_fovy, m_aspect, m_nearZ, m_farZ);
        float proj[16];
        glGetFloatv(GL_PROJECTION_MATRIX, proj);
        glLoadIdentity();
        //m_projMat = proj;
    }


    m_projMatDirtyFlag = false;
}




