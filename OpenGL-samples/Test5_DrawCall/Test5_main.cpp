
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../common_src/gl_common.h"
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <assert.h>
#include <Math/Float3.h>

#define KEY_ESCAPE 27

int majorVersion = 4;
int minorVersion = 4;
GLsizei width    = 1200;
GLsizei height   = 800;

GLuint triangleVAO;
GLuint sampleTriangleVAO;
GLuint boxVAO;
GLuint sampleBoxVAO;

GLuint program = 0;
GLint location_color1 = -1;

using namespace ZH::Math;

const unsigned int sampleCount = 100;
unsigned int realSampCount = 0;

#define DEBUG_GL_ERRORS TEST_COM::peek_gl_errors(__LINE__);


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    // Triangle
    {
        glUniform4f(location_color1, 0.5, 0.5, 1, 1);
        glBindVertexArray(triangleVAO);
        glDrawArrays(GL_LINE_LOOP, 0, 3);

        glBindVertexArray(sampleTriangleVAO);
        glUniform4f(location_color1, 0.0, 1.0, 0.0, 1);
        glPointSize(2);
        glDrawArrays(GL_POINTS, 0, realSampCount);
    }

    // box
    {
        //glUniform4f(location_color1, 0.5, 0.5, 1, 1);
        //glBindVertexArray(boxVAO);
        //glDrawArrays(GL_LINE_LOOP, 0, 4);

        //glBindVertexArray(sampleBoxVAO);
        //glUniform4f(location_color1, 0.0, 0.0, 0.0, 1);
        //glPointSize(2);
        //glDrawArrays(GL_POINTS, 0, sampleCount);
    }


    glutSwapBuffers();


}

static void sampleTriangle(float*& samples, unsigned int sampCount, float* vertices, unsigned int& sampCountRet )
{
    //  sanity check
    samples = NULL;
    assert(vertices);
    if (!vertices) return;
    if (sampCount < 1) return;

    // Find an acute angle
    unsigned int pIdx[] = { 0, 0, 0 };
    for (unsigned int i = 0; i < 3; ++i){
        pIdx[0] = i;
        pIdx[1] = (i + 1) % 3;
        pIdx[2] = (i + 2) % 3;
        float3 p0(vertices[3 * pIdx[0]], vertices[3 * pIdx[0] + 1], vertices[3 * pIdx[0] + 2]);
        float3 p1(vertices[3 * pIdx[1]], vertices[3 * pIdx[1] + 1], vertices[3 * pIdx[1] + 2]);
        float3 p2(vertices[3 * pIdx[2]], vertices[3 * pIdx[2] + 1], vertices[3 * pIdx[2] + 2]);
        if ((p1-p0).dot(p2-p0) > 0){
            break;
        }
    }

    const float3 p0 = { vertices[3 * pIdx[0]], vertices[3 * pIdx[0] + 1], vertices[3 * pIdx[0] + 2] };
    const float3 p1 = { vertices[3 * pIdx[1]], vertices[3 * pIdx[1] + 1], vertices[3 * pIdx[1] + 2] };
    const float3 p2 = { vertices[3 * pIdx[2]], vertices[3 * pIdx[2] + 1], vertices[3 * pIdx[2] + 2] };

    const float len01 = (p0 - p1).length();
    const float len02 = (p2 - p0).length();

    const float len01_02_ratio = len01 / len02;
    unsigned int sampeAlong02 =
        (unsigned int)((1.0f - len01_02_ratio +
        std::sqrt((1-len01_02_ratio)*(1-len01_02_ratio)+8*len01_02_ratio*sampleCount)
        )/2.0f/len01_02_ratio);
    sampeAlong02 = (sampeAlong02 == 0) ? 1:sampeAlong02;
    unsigned int sampeAlong01 = (unsigned int)((sampeAlong02 + 1)*len01_02_ratio - 1);
    sampeAlong01 = (sampeAlong01 == 0) ? 1 : sampeAlong01;
    samples = new float[sampeAlong01*sampeAlong02 * 3];

    unsigned int sampleBias = 0;
    float3 p01, p02;
    const float sampleInterleaveRatio01 = 1.0f / (sampeAlong01 + 1);
    float3 dVec01 = (p1 - p0)*sampleInterleaveRatio01;
    const float sampleInterleaveRatio02 = 1.0f / (sampeAlong02 + 1);
    float3 dVec02 = (p2 - p0)*sampleInterleaveRatio02;
    float3 startPoint = (p0 + dVec01 + p0 + dVec02) / 2.0f;

    float3 t;
    for (auto j = 0; j < sampeAlong02; ++j){
        if (j > 0){
            startPoint += dVec02;
        }
        for (auto i = 0; i < sampeAlong01; ++i){
            t = startPoint + dVec01 * i;

            // (t-p1)x(p2-t) should be same direction as
            // (p0-p1)x(p2-p0)
            float3 n1 = (t - p1).cross(p2 - t);
            float3 n2 = (p0 - p1).cross(p2 - p0);
            if (n1.dot(n2) < 0.0f)
                continue;

            samples[sampleBias * 3] = t[0];
            samples[sampleBias * 3 + 1] = t[1];
            samples[sampleBias * 3 + 2] = t[2];
            sampleBias++;

        }
    }

    sampCountRet = sampleBias;


}

void prepareTriangle()
{
    static GLfloat vertices[] = {
        -0.6f, -0.8f, -0.3f,
        0.6f, -0.8f, -0.3f,
        -0.6f, 0.1f, -0.3f
    };

    // Triangle
    {
        // Prepare data
        glGenVertexArrays(1, &triangleVAO);
        glBindVertexArray(triangleVAO);

        // GL_ARRAY_BUFFER
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        size_t size = sizeof(vertices);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

        glUseProgram(program);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0x00000000, 3, GL_FLOAT, 0, 0x00000000);
        glBindVertexArray(0);
    }

    // samples
    {
        // generate seed for rand
        srand((unsigned)time(0));

        float* ppData = NULL;

        sampleTriangle(ppData, sampleCount, vertices, realSampCount);

        glGenVertexArrays(1, &sampleTriangleVAO);
        glBindVertexArray(sampleTriangleVAO);

        // GL_ARRAY_BUFFER
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        size_t size = realSampCount * 3 * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, size, ppData, GL_STATIC_DRAW);

        glUseProgram(program);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0x00000000, 3, GL_FLOAT, 0, 0x00000000);
        glBindVertexArray(0);

        delete[] ppData;
        glBindVertexArray(0);
    }
}

void prepareBox()
{
    static GLfloat vertices[] = {
        0.1f, 0.1f, -0.3f,
        0.8f, 0.1f, -0.3f,
        0.8f, 0.8f, -0.3f,
        0.1f, 0.8f, -0.3f
    };

    // box
    {
        // Prepare data
        glGenVertexArrays(1, &boxVAO);
        glBindVertexArray(boxVAO);

        // GL_ARRAY_BUFFER
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        size_t size = sizeof(vertices);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

        glUseProgram(program);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0x00000000, 3, GL_FLOAT, 0, 0x00000000);
        glBindVertexArray(0);
    }


    // samples
    {
        // generate seed for rand
        srand((unsigned)time(0));

        float* ppData = new float[sampleCount * 3];

        for (auto i = 0; i < sampleCount; ++i){
            const float r0 = static_cast<float>(rand() / (float)RAND_MAX);
            const float r1 = static_cast<float>(rand() / (float)RAND_MAX);

            ppData[i * 3] = vertices[0] + (vertices[3] - vertices[0])* r0;
            ppData[i * 3 + 1] = vertices[1] + (vertices[7] - vertices[1])* r1;
            ppData[i * 3 + 2] = vertices[2];
        }

        glGenVertexArrays(1, &sampleBoxVAO);
        glBindVertexArray(sampleBoxVAO);

        // GL_ARRAY_BUFFER
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        size_t size = sampleCount * 3 * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, size, ppData, GL_STATIC_DRAW);

        glUseProgram(program);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0x00000000, 3, GL_FLOAT, 0, 0x00000000);
        glBindVertexArray(0);

        delete[] ppData;
        glBindVertexArray(0);
    }


}

void initialize()
{
    glViewport(0, 0, width, height);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.3f, 0.3f, 0.0f, 1.0f);

    program = glCreateProgram();
    {
        GLuint VS = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vsSource =
            "#version 440 core\n"
            "layout( location=0 ) in vec4 vPosition;\n"
            "void main(){\n"
            "    gl_Position = vPosition;\n"
            "}\n"
            ;
        glShaderSource(VS, 1, &vsSource, NULL);
        glCompileShader(VS);
        GLint compiled = 0;
        glGetShaderiv(VS, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {

            GLsizei len;
            glGetShaderiv(VS, GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetShaderInfoLog(VS, len, &len, log);
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete[] log;
            return;
        }

        glAttachShader(program, VS);
    }
    {
        GLuint PS = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* psSource =
            "#version 440 core\n"
            "uniform vec4 user_color1;\n"
            "out vec4 fColor;\n"
            "void main(){\n"
            "    fColor = user_color1;\n"
            "}\n"
            ;
        glShaderSource(PS, 1, &psSource, NULL);
        glCompileShader(PS);
        GLint compiled = 0;
        glGetShaderiv(PS, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {

            GLsizei len;
            glGetShaderiv(PS, GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetShaderInfoLog(PS, len, &len, log);
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete[] log;
            return;
        }

        glAttachShader(program, PS);
    }
    {
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {

            GLsizei len;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetProgramInfoLog(program, len, &len, log);
            std::cerr << "Shader linking failed: " << log << std::endl;
            delete[] log;

            return;
        }

    }

    glUseProgram(program);
    location_color1 = glGetUniformLocation(program, "user_color1");

    prepareTriangle();
    prepareBox();

}


void keyboard(unsigned char key, int mousePositionX, int mousePositionY)
{
    switch (key)
    {
    case KEY_ESCAPE:
        exit(0);
        break;

    default:
        break;
    }
}

int main(int argc, char **argv)
{
    // initialize and run program
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitContextVersion(majorVersion, minorVersion);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitWindowSize(width, height);
    glutCreateWindow("Test6_ZBC");
    glewExperimental = GL_TRUE;
    glewInit();

    const char* a = (const char*)glGetString(GL_VERSION);
    const char* b = (const char*)glGetString(GL_VENDOR);

    std::cout << "GL_VERSION: " << a << std::endl;
    std::cout << "GL_VENDOR:  " << b << std::endl;

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    initialize();
    glutMainLoop();
    return 0;
}
