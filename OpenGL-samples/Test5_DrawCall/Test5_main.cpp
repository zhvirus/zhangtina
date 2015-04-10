
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../common_src/gl_common.h"
#include "GL/glew.h"
#include <GL/freeglut.h>
#define KEY_ESCAPE 27

int majorVersion = 4;
int minorVersion = 4;
GLsizei width    = 800;
GLsizei height   = 600;

GLuint vertexArray[2];
GLint location_color1 = -1;

#define DEBUG_GL_ERRORS TEST_COM::peek_gl_errors(__LINE__);

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUniform4f(location_color1, 0.5, 0.5, 1, 1);

    glBindVertexArray(vertexArray[0]);
    //glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //char offset = 3 * sizeof(GLushort);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)offset);

    glPointSize(5);
    glUniform4f(location_color1, 0.0, 1.0, 1, 1);
    glDrawArrays(GL_POINTS, 0, 6);

    glutSwapBuffers();


}


void initialize()
{
    glViewport(0, 0, width, height);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.3f, 0.3f, 0.0f, 1.0f);

    // Prepare data
    glGenVertexArrays(1, vertexArray);
    glBindVertexArray(vertexArray[0]);

    GLfloat vertices[] = {
        0.5f, 0.1f, -0.3f,
        0.0f, 0.1f, -0.3f,
        0.5f, 0.5f, -0.3f,
        0.0f, -0.1f, -0.3f,
        -0.5f, -0.1f, -0.3f,
        -0.5f, -0.5f, -0.3f,
    };

    GLushort index_array[]=
    {
        0,1,2,3,4,5
    };

    // GL_ARRAY_BUFFER
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    size_t size = sizeof(vertices);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // GL_ELEMENT_ARRAY_BUFFER
    GLuint element_arr_buffer;
    glGenBuffers(1, &element_arr_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_arr_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);



    GLuint program = glCreateProgram();
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
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);

    glVertexAttribFormat(0x00000000, 3, GL_FLOAT, 0, 0x00000000);

    glVertexAttribBinding(0x00000000, 0x00000000);

    const GLuint arrBuffer = vertexArray[0];
    const GLintptr offset = 0;
    const GLsizei stride = 12;
    glBindVertexBuffers(0, 1, &arrBuffer, &offset, &stride);
    //glBindVertexBuffer(0, arrBuffer, offset, stride);
    
    location_color1 = glGetUniformLocation(program, "user_color1");
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
