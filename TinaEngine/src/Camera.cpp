#ifndef _ZH_DLL_
#define _ZH_DLL_
#endif
#include "Graphics/Camera.h"
#include "Internal/Common/internal_common.h"


namespace ZH{
    namespace Graphics{
        CLASS_TYPE_NAME_DEFINITION( Camera, "Camera" )

        Camera::Camera( CAMERA_TYPE t ):
            m_type(t),
            m_pos(0.0f,0.0f,-10.0f),
            m_lookDir(0.0f,0.0f,1.0f),
            m_rightDir(1.0f,0.0f,0.0f),
            m_nearZ(0.5f),
            m_farZ(50000.0f),
            m_width(640),
            m_height(480)
        {
        }

        Camera::Camera( CAMERA_TYPE t, Math::float3 pos, Math::float3 look, Math::float3 right, float near, float far, float w, float h):
            m_type(t),
            m_pos(pos),
            m_lookDir(look),
            m_rightDir(right),
            m_nearZ(near),
            m_farZ(far),
            m_width(w),
            m_height(h)
        {
        }

        Camera::~Camera()
        {
        }

        Math::matrix4x4_f Camera::viewMatrix()const
        {
            // Learned from 'D3DXMatrixOrthoLH'

            return Math::matrix4x4_f(
                2/m_width,       0.0f,   0.0f,                      0.0f,
                0.0f,      2/m_height,   0.0f,                      0.0f,
                0.0f,            0.0f,   1/(m_farZ-m_nearZ),        0.0f,
                0.0f,            0.0f,   -m_nearZ/(m_farZ-m_nearZ), 1.0f
                );
        }


        Math::matrix4x4_f Camera::projMatrix()const
        {
            // Learned from 'D3DXMatrixPerspectiveLH'

            return Math::matrix4x4_f(
                2*m_nearZ/m_width,       0.0f,   0.0f,                    0.0f,
                0.0f,      2*m_nearZ/m_height,   0.0f,                    0.0f,
                0.0f,            0.0f,   m_farZ/(m_farZ-m_nearZ),         1.0f,
                0.0f,            0.0f,  -m_nearZ*m_farZ/(m_farZ-m_nearZ), 0.0f
                );
        }




    }

}