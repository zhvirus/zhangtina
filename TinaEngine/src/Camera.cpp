#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Camera.h"
#include "Internal/Common/internal_common.h"
#include "Math/MathCommon.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Camera, "Camera" )
        CLASS_TYPE_NAME_DEFINITION( CameraOrtho, "CameraOrtho" )
        CLASS_TYPE_NAME_DEFINITION( CameraPersp, "CameraPersp" )


//------------------------------------------------
//
//        Camera
//
//------------------------------------------------


        Camera::Camera():
            m_pos(0.0f,0.0f,-10.0f),
            m_lookDir(0.0f,0.0f,1.0f),
            m_upDir(0.0f,1.0f,0.0f),
            m_viewMatDirtyFlag(true),
            m_projMatDirtyFlag(true)
        {
            m_viewMat.identityIt();
            m_projMat.identityIt();
        }

        Camera::Camera( Math::float3 pos, Math::float3 look, Math::float3 up ):
            m_pos(pos),
            m_lookDir(look),
            m_upDir(up),
            m_viewMatDirtyFlag(true),
            m_projMatDirtyFlag(true)
        {
            m_viewMat.identityIt();
            m_projMat.identityIt();
        }

        Camera::~Camera()
        {
        }

        void Camera::updateViewMat()
        {
            if ( !m_viewMatDirtyFlag )
                return;

            Math::float3 zaxis = m_lookDir.normalize();
            Math::float3 xaxis = m_upDir.cross(zaxis).normalize();
            Math::float3 yaxis = zaxis.cross(xaxis);

            float viewMat[]={
                xaxis.x,           yaxis.x,           zaxis.x,          0.0f,
                xaxis.y,           yaxis.y,           zaxis.y,          0.0f,
                xaxis.z,           yaxis.z,           zaxis.z,          0.0f,
               -xaxis.dot(m_pos), -yaxis.dot(m_pos), -zaxis.dot(m_pos), 1.0f
            };
            m_viewMat = viewMat;

            m_viewMatDirtyFlag = false;
        }

        Math::matrix4x4_f Camera::viewMatrix()
        {
            updateViewMat();
            return m_viewMat;
        }

        Math::matrix4x4_f Camera::projMatrix()
        {
            updateProjMat();
            return m_projMat;
        }

//------------------------------------------------
//
//        CameraOrtho
//
//------------------------------------------------

        CameraOrtho::CameraOrtho():
            Camera(),
            m_width( 640 ),
            m_height( 480 ),
            m_nearZ( 0.1f ),
            m_farZ( 50000.0f )
        {
        }

        CameraOrtho::CameraOrtho( Math::float3 pos, Math::float3 look, Math::float3 up, float w, float h, float nz, float fz):
            Camera(pos,look,up),
            m_width( w ),
            m_height( h ),
            m_nearZ( nz ),
            m_farZ( fz )
        {
        }

        void CameraOrtho::updateProjMat()
        {
            if ( !m_projMatDirtyFlag )
                return;

            // Learned from 'D3DXMatrixOrthoLH'

            float projMat[] = {
                2/m_width,       0.0f,   0.0f,                      0.0f,
                0.0f,      2/m_height,   0.0f,                      0.0f,
                0.0f,            0.0f,   1/(m_farZ-m_nearZ),        0.0f,
                0.0f,            0.0f,   -m_nearZ/(m_farZ-m_nearZ), 1.0f
            };

            m_projMat = projMat;

            m_projMatDirtyFlag = false;
        }

//------------------------------------------------
//
//        CameraPersp
//
//------------------------------------------------

        CameraPersp::CameraPersp():
            Camera(),
            m_fovy( Math::PI/3.0f ),
            m_aspect( 1.0f ),
            m_nearZ( 0.1f ),
            m_farZ( 50000.0f )
        {
        }

        CameraPersp::CameraPersp( Math::float3 pos, Math::float3 look, Math::float3 up, float fovy, float aspect, float nz, float fz):
            Camera(pos,look,up),
            m_fovy( fovy ),
            m_aspect( aspect ),
            m_nearZ( nz ),
            m_farZ( fz )
        {
        }

        void CameraPersp::updateProjMat()
        {
            if ( !m_projMatDirtyFlag )
                return;

            // Learned from 'D3DXMatrixPerspectiveFovLH'
            const float yScale = 1.0f/tan(m_fovy/2);
            const float xScale = yScale / m_aspect;

            float projMat[] = {
                xScale,    0.0f,   0.0f,                      0.0f,
                0.0f,      yScale, 0.0f,                      0.0f,
                0.0f,      0.0f,   m_farZ/(m_farZ-m_nearZ),   1.0f,
                0.0f,      0.0f,  -m_nearZ/(m_farZ-m_nearZ),  0.0f
            };

            m_projMat = projMat;

            m_projMatDirtyFlag = false;
        }
    }

}