#ifndef CAMERA_H
#define CAMERA_H
#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Math/Float3.h"
#include "Math/Matrix4x4_f.h"

namespace ZH{


    namespace Graphics{

        class ZH_DLL Camera : public Resource
        {
        public:
            enum CAMERA_TYPE{
                E_CAMERA_PERSP,
                E_CAMERA_ORTHO
            };
        public:
            Camera( CAMERA_TYPE t );
            Camera( CAMERA_TYPE t, Math::float3 pos, Math::float3 look, Math::float3 right, float near, float far, float w, float h);
            virtual ~Camera();
            Math::matrix4x4_f viewMatrix()const;
            Math::matrix4x4_f projMatrix()const;
            bool isOrho()const { return m_type == E_CAMERA_ORTHO; }
            bool isPersp()const { return m_type == E_CAMERA_PERSP; }
            

        private:
            CAMERA_TYPE m_type;

            // view related
            Math::float3 m_pos;
            Math::float3 m_lookDir;
            Math::float3 m_rightDir;

            // projection related
            float m_nearZ;
            float m_farZ;
            float m_width;
            float m_height;


            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };



    }
}




#endif