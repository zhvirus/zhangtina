#ifndef BASE_HAIR_RENDER
#define BASE_HAIR_RENDER

class CameraPersp;

class BaseHairRender
{
public:
    BaseHairRender();
    ~BaseHairRender();

    virtual void initialize() = 0;
    virtual void render(CameraPersp& cam) = 0;
protected:
    float* m_vertices;
    unsigned int* m_indices;
    unsigned int m_vertexCount;
    unsigned int m_indexCount;
};



#endif

