#include "stdafx.h"

#include "GeomHairRender.h"

bool GeomHairRender::build_geometry()
{
    // Prepare data
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, -0.5f,
        0.0f, 0.0f, 0.5f
    };

    // GL_ARRAY_BUFFER
    GLuint vb, ib;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    size_t size = sizeof(vertices);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    GLuint index_array[] =
    {
1,2,4
    };

    // GL_ELEMENT_ARRAY_BUFFER
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

bool GeomHairRender::build_shaders()
{
    m_program = glCreateProgram();
    {
        GLuint VS = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vsSource =
            "#version 440 core\n"
            "layout( shared, row_major ) uniform;\n"
            "uniform Matrices {\n"
            "   mat4 modelView;\n"
            "   mat4 proj;\n"
            "};\n"
            "layout( location=0 ) in vec4 vPosition;\n"
            "void main(){\n"
            "    gl_Position = proj*modelView*vPosition;\n"
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
            return false;
        }

        glAttachShader(m_program, VS);
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
            return false;
        }

        glAttachShader(m_program, PS);
    }
    {
        glLinkProgram(m_program);
        GLint linked;
        glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
        if (!linked) {

            GLsizei len;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetProgramInfoLog(m_program, len, &len, log);
            std::cerr << "Shader linking failed: " << log << std::endl;
            delete[] log;
            return false;
        }
    }


    // Get uniform locations
    glUseProgram(m_program);

    m_psSolidColorLoc = glGetUniformLocation(m_program, "user_color1");

    GLuint m_matUboIndex = glGetUniformBlockIndex(m_program, "Matrices");
    glGetActiveUniformBlockiv(m_program, m_matUboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_matUboSize);
    m_matUboBuffer = malloc(m_matUboSize);
    glGenBuffers(1, &m_matUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_matUbo);
    glBufferData(GL_UNIFORM_BUFFER, m_matUboSize, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_matUboIndex, m_matUbo);

    return true;
}

GeomHairRender::GeomHairRender():
m_matUboBuffer(NULL)
{

}

GeomHairRender::~GeomHairRender()
{

}


void GeomHairRender::initialize()
{
    if (!build_geometry()){
        std::cout << "Build geometry failed!\n";
        return;
    }

    if (!build_shaders()){
        std::cout << "Build shader failed!\n";
        return;
    }

}

void GeomHairRender::render(CameraPersp& cam)
{
    // ps color
    glUseProgram(m_program);
    glUniform4f(m_psSolidColorLoc, 0.0, 1.0, 0.0, 1);

    // vs matrix

    ZH::Math::matrix4x4_f viewMat = cam.viewMatrix();
    ZH::Math::matrix4x4_f projMat = cam.projMatrix();

    //viewMat = ZH::Math::matrix4x4_f::IDENTITY;
    //projMat = ZH::Math::matrix4x4_f::IDENTITY;

    memcpy(m_matUboBuffer, (void*)viewMat.v, sizeof(float) * 16);
    memcpy((void*)((float*)m_matUboBuffer + 16), (void*)(projMat.v), sizeof(float) * 16);

    glBindBuffer(GL_UNIFORM_BUFFER, m_matUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, m_matUboSize, m_matUboBuffer);

    glBindVertexArray(m_vertexArray);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
}

