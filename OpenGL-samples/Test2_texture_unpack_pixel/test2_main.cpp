// Test2_texture_unpack_pixel.cpp : Defines the entry point for the console application.
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


GLsizei width = 800;
GLsizei height = 600;
GLuint vertexArray[2];
GLint location = 0;
GLint location2 = 0;

GLuint texture = 0;


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindVertexArray(vertexArray[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glutSwapBuffers();
}


void initialize()
{
    DEBUG_GL_ERRORS;
    glViewport(0, 0, width, height);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    DEBUG_GL_ERRORS;

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

    glUseProgram(program);


    // Prepare data
    GLfloat vertices[] = {
        -0.8f, -0.8f, -0.3f, 1.0f,
         0.8f, -0.8f, -0.3f, 1.0f,
         0.8f, 0.8f, -0.3f, 1.0f,
        -0.8f, 0.8f, -0.3f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };


    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    DEBUG_GL_ERRORS;

    size_t size = sizeof(vertices);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    DEBUG_GL_ERRORS;

    glGenVertexArrays(1, vertexArray);
    glBindVertexArray(vertexArray[0]);

    DEBUG_GL_ERRORS;

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    DEBUG_GL_ERRORS;

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(16 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // prepare texture
    void* pData = NULL;
    unsigned int w=0, h=0, s=0;
    TEST_COM::image::instance().read_image(L"../res/512_512.jpg",
        pData, w, h, s);

    DEBUG_GL_ERRORS;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    DEBUG_GL_ERRORS;
    glTexStorage2D(GL_TEXTURE_2D, 10, GL_COMPRESSED_RGBA_ASTC_12x12_KHR, w, h);
    //glTexStorage2D(GL_TEXTURE_2D, 10, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, w, h);  
    DEBUG_GL_ERRORS;

    // Gen GL_PIXEL_UNPACK_BUFFER buffer
    GLuint bb;
    glGenBuffers(1, &bb); // => objects = { 1 }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bb);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, w*h*s, pData, GL_STATIC_DRAW);

    DEBUG_GL_ERRORS;
    //void* data = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_ONLY);
    //DEBUG_GL_ERRORS;
    //glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    //glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

    DEBUG_GL_ERRORS;

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)0);



    //glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
    DEBUG_GL_ERRORS;

    GLint tex1_loc = glGetUniformLocation(program, "tex1");
    glUniform1i(tex1_loc, 0);


    delete pData;

    //

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
    int minorVersion = 4;

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
    initialize();
    glutMainLoop();
    return 0;
}

