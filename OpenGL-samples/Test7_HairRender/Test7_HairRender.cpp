// Test7_HairRender.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GeomHairRender.h"

static CameraPersp gCamera;
static BaseHairRender* pRender = new GeomHairRender();

#define KEY_ESCAPE 27

int majorVersion = 4;
int minorVersion = 4;
GLsizei width = 800;
GLsizei height = 600;
float clearColor[] = {0.3f, 0.3f, 0.3f, 1.0f};

GLuint vertexArray[2];
GLint location_color1 = -1;


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width, height);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    pRender->render(gCamera);

    glutSwapBuffers();


}


void initialize()
{
    glViewport(0, 0, width, height);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

    pRender->initialize();
}

void reshape(int w, int h)
{
    gCamera.aspect((float)w / (float)h);
    glViewport(0, 0, w, h);

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

int _tmain(int argc, char* argv[])
{
    // initialize and run program
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitContextVersion(majorVersion, minorVersion);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE/*GLUT_CORE_PROFILE*/);

    glutInitWindowSize(width, height);
    glutCreateWindow("Test7_HairRender");
    glewExperimental = GL_TRUE;
    glewInit();

    const char* a = (const char*)glGetString(GL_VERSION);
    const char* b = (const char*)glGetString(GL_VENDOR);

    std::cout << "GL_VERSION: " << a << std::endl;
    std::cout << "GL_VENDOR:  " << b << std::endl;

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    initialize();
    glutMainLoop();
    return 0;
}
