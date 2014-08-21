#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/Shader.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/Debug.h"

namespace ZH{
    namespace Graphics{

        static bool toGLShaderType( const SHADER_TYPE& t, GLenum& gl_t )
        {
            switch (t){
            case E_VERTEX_SHADER:
                gl_t = GL_VERTEX_SHADER;
                break;
            case E_HULL_SHADER:
                gl_t = GL_TESS_CONTROL_SHADER;
                break;
            case E_DOMAIN_SHADER:
                gl_t = GL_TESS_EVALUATION_SHADER;
                break;
            case E_GEOMETRY_SHADER:
                gl_t = GL_GEOMETRY_SHADER;
                break;
            case E_FRAGMENT_SHADER:
                gl_t = GL_FRAGMENT_SHADER;
                break;
            case E_COMPUTE_SHADER:
                gl_t = GL_COMPUTE_SHADER;
                break;
            default:
                assert(false);
                ZH::Util::ENG_ERR("Unsupported shader type %d!", t);
                return false;
            };

            return true;
        }


        //----------------------------------------------------------------
        //
        // Shader
        //
        //----------------------------------------------------------------

        Shader::Shader( Context* ctx, SHADER_TYPE type, const char* const pSrc ):
            Resource( ctx ),
            m_type(type),
            m_pCode(NULL),
            m_compiled(false)
        {
            assert(pSrc);

            // Save the code
            m_pCode = new std::string(pSrc);

            GLenum gl_shader_type;
            if ( !toGLShaderType(type, gl_shader_type) ){
                ZH::Util::ENG_ERR("Invalid shader type!\n");
                return;
            }

            m_name = glCreateShader(gl_shader_type);
            assert(m_name>0);

            glShaderSource(m_name, 1, (const GLchar **)(&pSrc), NULL);

            Debug::dump_gl_errors();
        }

        Shader::~Shader()
        {
            if( m_name > 0 ){
                glDeleteShader(m_name);
            }

            delete m_pCode;
            m_pCode = NULL;
        }

        bool Shader::compile()
        {
            // Already compiled
            if ( m_compiled ){
                return true;
            }

            // Shader not created successfully
            if ( m_name <= 0 ){
                return false;
            }

            // Do compile
            glCompileShader(m_name);
            GLint compiled=0;
            glGetShaderiv(m_name, GL_COMPILE_STATUS, &compiled );
            if ( !compiled ) {
                GLsizei len;
                glGetShaderiv( m_name, GL_INFO_LOG_LENGTH, &len );

                GLchar* log = new GLchar[len+1];
                glGetShaderInfoLog( m_name, len, &len, log );

                ZH::Util::ENG_ERR("Shader (%d) compilation failed: %s\n", m_name, log);

                delete [] log;
                return false;
            }

            // success
            m_compiled = true;
            return true;
        }

        bool Shader::valid() const
        {
            return (m_name>0) && m_compiled;
        }


        //----------------------------------------------------------------
        //
        // Program
        //
        //----------------------------------------------------------------

        Program::Program( Context* ctx ):
            Resource(ctx),
            m_linked(false),
            m_pShaders(NULL)
        {
            m_name = glCreateProgram();
            assert(m_name>0);

            m_pShaders = new std::map<SHADER_TYPE, Shader*>;
            m_pShaders->insert( std::make_pair(E_VERTEX_SHADER, (Shader*)NULL) );
            m_pShaders->insert( std::make_pair(E_HULL_SHADER, (Shader*)NULL) );
            m_pShaders->insert( std::make_pair(E_DOMAIN_SHADER, (Shader*)NULL) );
            m_pShaders->insert( std::make_pair(E_GEOMETRY_SHADER, (Shader*)NULL) );
            m_pShaders->insert( std::make_pair(E_FRAGMENT_SHADER, (Shader*)NULL) );
            m_pShaders->insert( std::make_pair(E_COMPUTE_SHADER, (Shader*)NULL) );
        }

        Program::~Program()
        {
            delete m_pShaders;
            m_pShaders = NULL;

            if ( m_name>0 ){
                glDeleteProgram(m_name);
            }
        }

        bool Program::setShaderCommon( Shader* pShader, SHADER_TYPE exp_shader_type )
        {
            // Check shader type match
            if ( pShader && (pShader->type() != exp_shader_type) ){
                return false;
            }

            // Get current shader
            Shader* pCurShader = (*m_pShaders)[exp_shader_type];

            // Detach previous shader of this type
            if ( pCurShader && (pCurShader->name() > 0) ){
                glDetachShader( m_name, pCurShader->name() );
            }

            // Set new shader
            (*m_pShaders)[exp_shader_type] = pShader;

            // If valid, attach it to program
            if ( pShader && (pShader->name() > 0) ){
                glAttachShader( m_name, pShader->name() );
            }

            // Dirty the link flag
            m_linked = false;

            return true;
        }

        // Vertex shader
        bool Program::setVertexShader( Shader* pShader )
        {
            return setShaderCommon( pShader, E_VERTEX_SHADER );
        }

        // Hull shader
        bool Program::setHullShader( Shader* pShader )
        {
            return setShaderCommon( pShader, E_HULL_SHADER );
        }

        // Domain shader
        bool Program::setDomainShader( Shader* pShader )
        {
            return setShaderCommon( pShader, E_DOMAIN_SHADER );
        }

        // Geometry shader
        bool Program::setGeometryShader( Shader* pShader )
        {
            return setShaderCommon( pShader, E_GEOMETRY_SHADER );
        }

        // Fragment shader
        bool Program::setFragmentShader( Shader* pShader )
        {
            return setShaderCommon( pShader, E_FRAGMENT_SHADER );
        }

        // Compute shader
        bool Program::setComputeShader( Shader* pShader )
        {
            return setShaderCommon( pShader, E_COMPUTE_SHADER );
        }

        // Link
        bool Program::link()
        {
            if ( !m_name ){
                return false;
            }

            if ( m_linked ){
                return true;
            }

            if ( !m_pShaders ){
                assert(m_pShaders);
                return false;
            }

            // At least one VS and one PS
            {
                Shader* pShader = (*m_pShaders)[E_VERTEX_SHADER];
                if ( !pShader || !pShader->compile() ){
                    return false;
                }
            }
            {
                Shader* pShader = (*m_pShaders)[E_FRAGMENT_SHADER];
                if ( !pShader || !pShader->compile() ){
                    return false;
                }
            }

            // Any given shader should pass compile
            {
                Shader* pShader = (*m_pShaders)[E_HULL_SHADER];
                if ( pShader && !pShader->compile() ){
                    return false;
                }
            }
            {
                Shader* pShader = (*m_pShaders)[E_DOMAIN_SHADER];
                if ( pShader && !pShader->compile() ){
                    return false;
                }
            }
            {
                Shader* pShader = (*m_pShaders)[E_GEOMETRY_SHADER];
                if ( pShader && !pShader->compile() ){
                    return false;
                }
            }
            {
                Shader* pShader = (*m_pShaders)[E_COMPUTE_SHADER];
                if ( pShader && !pShader->compile() ){
                    return false;
                }
            }

            // Start real link
            glLinkProgram( m_name );
            GLint linked;
            glGetProgramiv( m_name, GL_LINK_STATUS, &linked );
            if ( !linked ) {

                GLsizei len;
                glGetProgramiv( m_name, GL_INFO_LOG_LENGTH, &len );

                GLchar* log = new GLchar[len+1];
                glGetProgramInfoLog( m_name, len, &len, log );
                ZH::Util::ENG_ERR("Program %d linking failed: %s", m_name, log);
                delete [] log;

                return false;
            }

            m_linked = true;
            return true;
        }


    }
}

