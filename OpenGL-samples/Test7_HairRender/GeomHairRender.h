#ifndef GEOM_HAIR_RENDER
#define GEOM_HAIR_RENDER

#include "BaseHairRender.h"

class GeomHairRender : public BaseHairRender
{
public:
    GeomHairRender();
    ~GeomHairRender();

    virtual void initialize();
    virtual void render(CameraPersp& cam);

private:
    bool build_geometry();
    bool build_shaders();
private:
    GLuint m_program;
    GLuint m_vertexArray;

    GLint  m_psSolidColorLoc;
    GLuint m_matUboIndex;
    GLint  m_matUboSize;
    void*  m_matUboBuffer;
    GLuint m_matUbo;
};

#endif