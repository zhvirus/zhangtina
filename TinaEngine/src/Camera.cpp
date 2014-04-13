#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Camera.h"
#include "Internal/Common/internal_common.h"
#include "Math/MathCommon.h"

namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Camera, Camera )
        CLASS_TYPE_NAME_DEFINITION( CameraOrtho, CameraOrtho )
        CLASS_TYPE_NAME_DEFINITION( CameraPersp, CameraPersp )


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

        Camera::Camera( const Camera& v ):
            Resource(v),
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

        CameraOrtho::CameraOrtho( Math::float3 pos, Math::float3 look, Math::float3 up, float w, float h, float nz, float fz, const std::string& name ):
            Camera(pos,look,up),
            m_width( w ),
            m_height( h ),
            m_nearZ( nz ),
            m_farZ( fz )
        {
            Name::constructName( CameraOrtho::m_cClassName, name );
        }

        CameraOrtho::CameraOrtho( const CameraOrtho& v ):
            Camera(v),
            m_width( v.m_width ),
            m_height( v.m_height ),
            m_nearZ( v.m_nearZ ),
            m_farZ( v.m_farZ )
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

        bool CameraOrtho::operator==(const Camera& v)
        {
            const CameraOrtho& rh = dynamic_cast<const CameraOrtho&>(v);
            return
                ( *m_name == *rh.m_name ) &&
                ( m_pos == rh.m_pos) &&
                ( m_lookDir == rh.m_lookDir) &&
                ( m_upDir == rh.m_upDir) &&
                (ZH::Math::VALUE_EQUAL<float>(m_width, rh.m_width))&&
                (ZH::Math::VALUE_EQUAL<float>(m_height, rh.m_height))&&
                (ZH::Math::VALUE_EQUAL<float>(m_nearZ, rh.m_nearZ))&&
                (ZH::Math::VALUE_EQUAL<float>(m_farZ, rh.m_farZ))
                ;
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

        CameraPersp::CameraPersp( Math::float3 pos, Math::float3 look, Math::float3 up, float fovy, float aspect, float nz, float fz, const std::string& name ):
            Camera(pos,look,up),
            m_fovy( fovy ),
            m_aspect( aspect ),
            m_nearZ( nz ),
            m_farZ( fz )
        {
            Name::constructName( CameraPersp::m_cClassName, name );
        }

        CameraPersp::CameraPersp( const CameraPersp& v):
            Camera(v),
            m_fovy(v.m_fovy),
            m_aspect(v.m_aspect),
            m_nearZ(v.m_nearZ),
            m_farZ(v.m_farZ)
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

        bool CameraPersp::operator==(const Camera& v)
        {
            const CameraPersp& rh = dynamic_cast<const CameraPersp&>(v);

            return
                ( *m_name == *rh.m_name ) &&
                ( m_pos == rh.m_pos) &&
                ( m_lookDir == rh.m_lookDir) &&
                ( m_upDir == rh.m_upDir) &&
                (ZH::Math::VALUE_EQUAL<float>(m_fovy, rh.m_fovy))&&
                (ZH::Math::VALUE_EQUAL<float>(m_aspect, rh.m_aspect))&&
                (ZH::Math::VALUE_EQUAL<float>(m_nearZ, rh.m_nearZ))&&
                (ZH::Math::VALUE_EQUAL<float>(m_farZ, rh.m_farZ))
                ;
        }


    }

}