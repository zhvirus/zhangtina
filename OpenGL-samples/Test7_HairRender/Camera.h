#ifndef CAMERA_H
#define CAMERA_H

#include "Math/Float3.h"
#include "Math/Matrix4x4_f.h"


class Camera
{
public:
    Camera();
    Camera(ZH::Math::float3 pos, ZH::Math::float3 look, ZH::Math::float3 up);
    Camera(const Camera&);
    virtual ~Camera();

    ZH::Math::matrix4x4_f projMatrix();
    ZH::Math::matrix4x4_f viewMatrix();

    inline ZH::Math::float3 pos()const { return m_pos; }
    inline void pos(const ZH::Math::float3& p) { m_pos = p; m_viewMatDirtyFlag = true; }

    inline ZH::Math::float3 lookDir()const { return m_lookDir; }
    inline void lookDir(const ZH::Math::float3& d) { m_lookDir = d; m_viewMatDirtyFlag = true; }

    inline ZH::Math::float3 upDir()const { return m_upDir; }
    inline void upDir(const ZH::Math::float3& d) { m_upDir = d; m_viewMatDirtyFlag = true; }

protected:
    void updateViewMat();
    virtual void updateProjMat() = 0;

    char m_name[100];

    // view related
    ZH::Math::float3 m_pos;
    ZH::Math::float3 m_lookDir;
    ZH::Math::float3 m_upDir;

    // cached view/proj matrices
    ZH::Math::matrix4x4_f m_viewMat;
    ZH::Math::matrix4x4_f m_projMat;
    bool m_viewMatDirtyFlag;
    bool m_projMatDirtyFlag;
};

class CameraPersp : public Camera
{
public:
    CameraPersp();
    CameraPersp(ZH::Math::float3 pos, ZH::Math::float3 look, ZH::Math::float3 up, float fovy, float aspect, float nz, float fz);
    CameraPersp(const CameraPersp&);

    inline float fovy()const { return m_fovy; }
    inline void fovy(float f) { m_fovy = f; m_projMatDirtyFlag = true; }

    inline float aspect()const { return m_aspect; }
    inline void aspect(float a) { m_aspect = a; m_projMatDirtyFlag = true; }

    inline float nearZ()const { return m_nearZ; }
    inline void nearZ(float z) { m_nearZ = z; m_projMatDirtyFlag = true; }

    inline float farZ()const { return m_farZ; }
    inline void farZ(float z) { m_farZ = z; m_projMatDirtyFlag = true; }

    static const char m_sDefaultName[100];
protected:
    virtual void updateProjMat();

    // projection related
    float m_fovy;
    float m_aspect; // width/height
    float m_nearZ;
    float m_farZ;
};




#endif