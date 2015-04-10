
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../common_src/gl_common.h"
#include "GL/glew.h"
#include <GL/freeglut.h>



#define KEY_ESCAPE 27

int majorVersion = 4;
int minorVersion = 2;
GLsizei width  = 800;
GLsizei height = 600;
GLsizei tex_w = 800;
GLsizei tex_h = 600;

// simple
GLuint vertexArray[2];
GLint location_color1 = -1;
GLuint buffer = 0;
GLuint element_arr_buffer = 0;

// tex
GLuint tex_vb = 0;
GLuint tex_arr = 0;
GLint tex1_loc = 0;

GLuint textures[2];
GLuint fbos[2];

GLuint programs[2];

#define DEBUG_GL_ERRORS TEST_COM::peek_gl_errors(__LINE__);

int i = 0;

static float clear_col1[] = {1.0f, 0.0f, 0.0f, 1.0f};
static float clear_col2[] = {0.0f, 1.0f, 0.0f, 1.0f};
static float clear_col3[] = {0.0f, 0.0f, 1.0f, 1.0f};
static float tri_col[]    = {0.0f, 0.7f, 0.6f, 1.0f};
static float point_col[]  = {0.2f, 0.2f, 0.2f, 1.0f };

void render_to_textures()
{
    // binding
    glBindVertexArray(vertexArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_arr_buffer);
    glPointSize(5);

    // prog
    glUseProgram(programs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // fbo 0
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
    glClearColor(clear_col1[0], clear_col1[1], clear_col1[2], clear_col1[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform4f(location_color1, tri_col[0], tri_col[1], tri_col[2], tri_col[3]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform4f(location_color1, point_col[0], point_col[1], point_col[2], point_col[3]);
    glDrawArrays(GL_POINTS, 0, 6);

    // fbo 1
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
    glClearColor(clear_col2[0], clear_col2[1], clear_col2[2], clear_col2[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform4f(location_color1, tri_col[0], tri_col[1], tri_col[2], tri_col[3]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform4f(location_color1, point_col[0], point_col[1], point_col[2], point_col[3]);
    glDrawArrays(GL_POINTS, 0, 6);

    // clear fbo 1
    glClearColor(clear_col3[0], clear_col3[1], clear_col3[2], clear_col3[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void render_textures_to_screen()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programs[1]);

    glBindVertexArray(tex_arr);
    glBindBuffer(GL_ARRAY_BUFFER, tex_vb);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(16 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glUniform1i(tex1_loc, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void display()
{
    if (i < 3){
        render_to_textures();
        //render_textures_to_screen();
        glutSwapBuffers();
    }
    else{
        PostQuitMessage(0);
    }

    i++;
}


void prepare_simple_buffers()
{
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

    GLushort index_array[] =
    {
        0, 1, 2, 3, 4, 5
    };

    // GL_ARRAY_BUFFER
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    size_t size = sizeof(vertices);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // GL_ELEMENT_ARRAY_BUFFER
    glGenBuffers(1, &element_arr_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_arr_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);

    // prog
    glUseProgram(programs[0]);
    location_color1 = glGetUniformLocation(programs[0], "user_color1");
}



void prepare_tex_buffers()
{

    // Prepare data
    GLfloat vertices[] = {
        -0.8f, -0.8f, -0.3f, 1.0f,
        0.8f, -0.8f, -0.3f, 1.0f,
        0.8f, 0.8f, -0.3f, 1.0f,
        -0.8f, 0.8f, -0.3f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenVertexArrays(1, &tex_arr);
    glBindVertexArray(tex_arr);

    glGenBuffers(1, &tex_vb);
    glBindBuffer(GL_ARRAY_BUFFER, tex_vb);
    size_t size = sizeof(vertices);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // prog
    glUseProgram(programs[1]);
    tex1_loc = glGetUniformLocation(programs[1], "tex1");

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(16 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void create_simple_program()
{

    programs[0] = glCreateProgram();
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

        glAttachShader(programs[0], VS);
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

        glAttachShader(programs[0], PS);
    }
    {
        glLinkProgram(programs[0]);
        GLint linked;
        glGetProgramiv(programs[0], GL_LINK_STATUS, &linked);
        if (!linked) {

            GLsizei len;
            glGetProgramiv(programs[0], GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetProgramInfoLog(programs[0], len, &len, log);
            std::cerr << "Shader linking failed: " << log << std::endl;
            delete[] log;

            return;
        }

    }

}

void create_tex_program()
{

    programs[1] = glCreateProgram();
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

        glAttachShader(programs[1], VS);
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
            "    fColor = texdata.rgba;\n"
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

        glAttachShader(programs[1], PS);
    }
    {
        glLinkProgram(programs[1]);
        GLint linked;
        glGetProgramiv(programs[1], GL_LINK_STATUS, &linked);
        if (!linked) {

            GLsizei len;
            glGetProgramiv(programs[1], GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetProgramInfoLog(programs[1], len, &len, log);
            std::cerr << "Shader linking failed: " << log << std::endl;
            delete[] log;

            return;
        }

    }
}


void initialize()
{
    glViewport(0, 0, width, height);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);



    create_simple_program();
    create_tex_program();

    prepare_simple_buffers();
    prepare_tex_buffers();

    // Generate textures
    glGenTextures(2, &(textures[0]));
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(2, &(fbos[0]));
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    glutCreateWindow("Test5_simple_draw");
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
