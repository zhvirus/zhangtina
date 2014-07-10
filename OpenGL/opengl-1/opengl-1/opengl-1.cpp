// opengl-1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#define KEY_ESCAPE 27


GLsizei width = 800;
GLsizei height = 600;
GLuint vertexArray[2];


void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glBindVertexArray(vertexArray[0]);
    glDrawArrays( GL_TRIANGLES, 0, 6);

    glutWireCube(5);

    glutSwapBuffers();
    //glutPostRedisplay();
}


void initialize () 
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_MODELVIEW);

    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

    // Prepare data
    glGenVertexArrays(1, vertexArray);
    glBindVertexArray(vertexArray[0]);

    GLfloat vertices[] ={
        0.5f, 0.1f, -0.3f,
        -0.5f, 0.1f, -0.3f,
        0.0f, 0.5f, -0.3f,
        0.5f, -0.1f, -0.3f,
        -0.5f, -0.1f, -0.3f,
        0.0f, -0.5f, -0.3f,
    };


    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer( GL_ARRAY_BUFFER, buffer);

    size_t size = sizeof(vertices);
    glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    GLuint VS  = glCreateShader( GL_VERTEX_SHADER );
    GLuint PS  = glCreateShader( GL_FRAGMENT_SHADER );

    GLuint program = glCreateProgram();
    {

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

        const GLchar* psSource =
            "#version 430 core\n"
            "out vec4 fColor;\n"
            "void main(){\n"
            "    fColor = vec4(1.0, 0.0, 0.0 ,1.0);\n"
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
    glutInitContextVersion(4,2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(width,height);
    glutCreateWindow("Test");
    glewExperimental = GL_TRUE;
    glewInit();
    if ( GLEW_VERSION_4_2){
        std::cout<<"Support 4.2!\n";
    }
    glutDisplayFunc(display);
    glutIdleFunc( display );
    glutKeyboardFunc( keyboard );
    initialize();
    glutMainLoop();
    return 0;
}
