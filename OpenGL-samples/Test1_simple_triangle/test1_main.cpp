// opengl-1.cpp : Defines the entry point for the console application.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GL/glew.h"
#include <GL/freeglut.h>
#define KEY_ESCAPE 27


GLsizei width = 800;
GLsizei height = 600;
GLuint vertexArray[2];
GLint location_color1 = -1;

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;

static void updateRGB()
{
    static unsigned int R = 0;
    static unsigned int G = 0;
    static unsigned int B = 0;

    if (R < 255){
        R++;
    }
    else if (G < 255){
        G++;
    }
    else if (B < 255){
        B++;
    }
    else{
        R = 0;
        G = 0;
        B = 0;
    }

    r = (float)R / 255.0f;
    g = (float)G / 255.0f;
    b = (float)B / 255.0f;
}


void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glLoadIdentity();
    updateRGB();

    glUniform4f(location_color1, r, g, b, 1);

    glBindVertexArray(vertexArray[0]);
    glDrawArrays( GL_TRIANGLES, 0, 6);

    GLenum err;
    while (true)
    {
        err = glGetError();
        if (err == GL_NONE)
        {
            break;
        }
    }

    char* pData = new char[width*height * 4];
    memset((void*)pData, 0, width*height * 4);

    glReadPixels(0, 0, width, height,
        GL_BGRA,
        GL_UNSIGNED_BYTE, pData);

    while (true)
    {
        err = glGetError();
        if (err == GL_NONE)
        {
            break;
        }
    }

    glutSwapBuffers();


}


void initialize () 
{
    glViewport(0, 0, width, height);

    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    glClearColor(0.3f, 0.3f, 0.0f, 1.0f);

    // Prepare data
    glGenVertexArrays(1, vertexArray);
    glBindVertexArray(vertexArray[0]);

    GLfloat vertices[] ={
        0.5f, 0.1f, -0.3f,
        0.0f, 0.1f, -0.3f,
        0.5f, 0.5f, -0.3f,
        0.0f, -0.1f, -0.3f,
        -0.5f, -0.1f, -0.3f,
        -0.5f, -0.5f, -0.3f,
    };


    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer( GL_ARRAY_BUFFER, buffer);

    size_t size = sizeof(vertices);
    glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);


    GLuint program = glCreateProgram();
    {
        GLuint VS  = glCreateShader( GL_VERTEX_SHADER );
        const GLchar* vsSource =
            "#version 430 core\n"
            "layout( location=0 ) in vec4 vPosition;\n"
            "void main(){\n"
            "    gl_Position = vPosition;\n"
            "}\n"
            ;
        glShaderSource(VS, 1, &vsSource, NULL);
        glCompileShader(VS);
        GLint compiled=0;
        glGetShaderiv( VS, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {

            GLsizei len;
            glGetShaderiv( VS, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( VS, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
            return;
        }

        glAttachShader( program, VS );
    }
    {
        GLuint PS  = glCreateShader( GL_FRAGMENT_SHADER );
        const GLchar* psSource =
            "#version 430 core\n"
            "uniform vec4 user_color1;\n"
            "layout (std140) uniform MoreColor1\n"
            "{\n"
            "   vec4  user_color2;\n"
            "};\n"
            "layout (std140) uniform MoreColor2\n"
            "{\n"
            "   vec4  user_color3;\n"
            "};\n"
            "out vec4 fColor;\n"
            "void main(){\n"
            "    fColor = 0.33f * (user_color1+user_color2+user_color3);\n"
            "}\n"
            ;
        glShaderSource(PS, 1, &psSource, NULL);
        glCompileShader(PS);
        GLint compiled=0;
        glGetShaderiv( PS, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {

            GLsizei len;
            glGetShaderiv( PS, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( PS, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
            return;
        }

        glAttachShader( program, PS );
    }
    {
        glLinkProgram( program );
        GLint linked;
        glGetProgramiv( program, GL_LINK_STATUS, &linked );
        if ( !linked ) {

            GLsizei len;
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetProgramInfoLog( program, len, &len, log );
            std::cerr << "Shader linking failed: " << log << std::endl;
            delete [] log;

            return;
        }

    }

    glUseProgram( program );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray( 0 );

    location_color1 = glGetUniformLocation(program, "user_color1");

    GLuint index1 = glGetUniformBlockIndex(program, "MoreColor1");
    GLuint index2 = glGetUniformBlockIndex(program, "MoreColor2");

    char name1[100];
    glGetActiveUniformBlockName(program, index1, 100, NULL, name1);
    char name2[100];
    glGetActiveUniformBlockName(program, index2, 100, NULL, name2);

    glUniformBlockBinding(program, 0, 1);
    glUniformBlockBinding(program, 1, 2);

    GLint oldReadFramebuffer = 0;
    GLint oldDrawFramebuffer = 0;
    GLint oldReadBuffer = GL_NONE;
    GLint oldDrawBuffer = GL_NONE;

    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &oldReadFramebuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oldDrawFramebuffer);
    glGetIntegerv(GL_READ_BUFFER, &oldReadBuffer);
    glGetIntegerv(GL_DRAW_BUFFER, &oldDrawBuffer);
    GLint value;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &value);
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &value);
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &value);
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &value);
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &value);
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &value);
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &value);

    GLenum err;
    while (true)
    {
        err = glGetError();
        if (err == GL_NONE)
        {
            break;
        }
    }

    char* pData = new char[width*height * 4];
    memset((void*)pData, 0, width*height * 4);

    glReadPixels(0, 0, width, height,
        GL_BGRA,
        GL_UNSIGNED_BYTE, pData);

    while (true)
    {
        err = glGetError();
        if (err == GL_NONE)
        {
            break;
        }
    }
}


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )
{ 
  switch ( key ) 
  {
    case KEY_ESCAPE:
      exit ( 0 );
      break;

    default:
      break;
  }
}

int main(int argc, char **argv) 
{
    // initialize and run program
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

    int majorVersion = 4;
    int minorVersion = 4;
    glutInitContextVersion(majorVersion, minorVersion);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitWindowSize(width,height);
    glutCreateWindow("Test1_simple_triangle");
    glewExperimental = GL_TRUE;
    glewInit();

    const char* a = (const char*)glGetString(GL_VERSION );
    const char* b = (const char*)glGetString(GL_VENDOR);

    std::cout << "GL_VERSION: " << a << std::endl;
    std::cout << "GL_VENDOR:  " << b << std::endl;


    glutDisplayFunc(display);
    glutIdleFunc( display );
    glutKeyboardFunc( keyboard );
    initialize();
    glutMainLoop();
    return 0;
}
