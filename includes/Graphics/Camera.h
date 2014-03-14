#ifndef CAMERA_H
#define CAMERA_H
#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Math/Float3.h"
#include "Math/Matrix4x4_f.h"

namespace ZH{


    namespace Graphics{

        class ZH_GRAPHICS_DLL Camera : public Resource
        {
        public:
            Camera();
            Camera( Math::float3 pos, Math::float3 look, Math::float3 up );
            virtual ~Camera();

            virtual bool operator==(const Camera&) = 0;

            Math::matrix4x4_f projMatrix();
            Math::matrix4x4_f viewMatrix();

            inline Math::float3 pos()const { return m_pos; }
            inline void pos( const Math::float3& p ) { m_pos = p; m_viewMatDirtyFlag=true; }

            inline Math::float3 lookDir()const { return m_lookDir; }
            inline void lookDir( const Math::float3& d ) { m_lookDir = d; m_viewMatDirtyFlag=true; }

            inline Math::float3 upDir()const { return m_upDir; }
            inline void upDir( const Math::float3& d ) { m_upDir = d; m_viewMatDirtyFlag=true; }

        protected:
            void updateViewMat();
            virtual void updateProjMat() = 0;

            // view related
            Math::float3 m_pos;
            Math::float3 m_lookDir;
            Math::float3 m_upDir;

            // cached view/proj matrices
            Math::matrix4x4_f m_viewMat;
            Math::matrix4x4_f m_projMat;
            bool m_viewMatDirtyFlag;
            bool m_projMatDirtyFlag;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        class ZH_GRAPHICS_DLL CameraOrtho : public Camera
        {
        public:
            CameraOrtho();
            CameraOrtho( Math::float3 pos, Math::float3 look, Math::float3 up, float w, float h, float nz, float fz);

            virtual bool operator==(const Camera&);

            inline float width()const { return m_width; }
            inline void width( float w ) { m_width = w; m_projMatDirtyFlag=true; }

            inline float height()const { return m_height; }
            inline void height( float h ) { m_height = h; m_projMatDirtyFlag=true; }

            inline float nearZ()const { return m_nearZ; }
            inline void nearZ( float z ) { m_nearZ = z; m_projMatDirtyFlag=true; }

            inline float farZ()const { return m_farZ; }
            inline void farZ( float z ) { m_farZ = z; m_projMatDirtyFlag=true; }

        protected:
            virtual void updateProjMat();
        private:
            // projection related
            float m_width;
            float m_height;
            float m_nearZ;
            float m_farZ;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        class ZH_GRAPHICS_DLL CameraPersp : public Camera
        {
        public:
            CameraPersp();
            CameraPersp( Math::float3 pos, Math::float3 look, Math::float3 up, float fovy, float aspect, float nz, float fz);

            virtual bool operator==(const Camera&);

            inline float fovy()const { return m_fovy; }
            inline void fovy( float f ) { m_fovy = f; m_projMatDirtyFlag=true; }

            inline float aspect()const { return m_aspect; }
            inline void aspect( float a ) { m_aspect = a; m_projMatDirtyFlag=true; }

            inline float nearZ()const { return m_nearZ; }
            inline void nearZ( float z ) { m_nearZ = z; m_projMatDirtyFlag=true; }

            inline float farZ()const { return m_farZ; }
            inline void farZ( float z ) { m_farZ = z; m_projMatDirtyFlag=true; }

        protected:
            virtual void updateProjMat();
        private:
            // projection related
            float m_fovy;
            float m_aspect; // width/height
            float m_nearZ;
            float m_farZ;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

    }
}




#endif