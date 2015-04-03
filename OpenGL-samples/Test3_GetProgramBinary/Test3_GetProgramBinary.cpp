// Test3_GetProgramBinary.cpp : Defines the entry point for the console application.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "../common_src/gl_common.h"
#include "GL/glew.h"
#include <GL/freeglut.h>
#define KEY_ESCAPE 27

#define DEBUG_GL_ERRORS TEST_COM::peek_gl_errors(__LINE__);

int width = 800;
int height = 600;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glutSwapBuffers();
}


void initialize(const char* program_name)
{
    glViewport(0, 0, width, height);


    GLuint program = glCreateProgram();
    {
        GLuint VS = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vsSource =
            "#version 440 core\n"
            "layout( location=0 ) in vec4 vPosition;\n"
            "layout( location=1 ) in vec2 vTexCoord;\n"
            "out vec2 vs_tex_coord;\n"
            "void main(){\n"
            "    gl_Position = vPosition;\n"
            "    vs_tex_coord = vTexCoord;\n"
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
            "uniform sampler2D tex1;\n"
            "in vec2 vs_tex_coord;\n"
            "layout (location=0) out vec4 fColor;\n"
            "void main(){\n"
            "    vec4 texdata = texture(tex1, vs_tex_coord);\n"
            "    fColor = texdata.bgra;\n"
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

    int binaryLength = 0;
    glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
    void* binary = (void*)malloc(binaryLength);
    GLenum fmt;
    glGetProgramBinary(program, binaryLength, NULL, &fmt, binary);

    //
    //  Cache the program binary for future runs
    //
    FILE* outfile = fopen(program_name, "wb");
    fwrite(binary, binaryLength, 1, outfile);
    fclose(outfile);



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


int main(int argc, char* argv[])
{
    // initialize and run program
    glutInit(&argc, argv);


    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    int majorVersion = 4;
    int minorVersion = 5;

    glutInitContextVersion(majorVersion, minorVersion);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(width, height);


    glutCreateWindow("Test1_simple_triangle");
    glewExperimental = GL_TRUE;
    glewInit();

    const char* a = (const char*)glGetString(GL_VERSION);
    const char* b = (const char*)glGetString(GL_VENDOR);
    std::cout << "GL_VERSION: " << a << std::endl;
    std::cout << "GL_VENDOR:  " << b << std::endl;

    DEBUG_GL_ERRORS;

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    DEBUG_GL_ERRORS;
    initialize("X:/1.bin");
    initialize("X:/2.bin");
    initialize("X:/3.bin");
    DEBUG_GL_ERRORS;
    glutMainLoop();
    return 0;
}


