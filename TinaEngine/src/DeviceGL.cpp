#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/DeviceGL.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Util/Print.h"
#include "Widget/Window.h"

#include <windows.h>
#include "gl/glew.h"
#include "gl/wglew.h"

#pragma warning(disable:4127)



namespace ZH{
    namespace Graphics{

        DeviceGL::DeviceGL():
            Device(),
            m_hglrc(NULL),
            m_hdc(NULL)
        {
        }

        DeviceGL::~DeviceGL()
        {
            shutdown();
        }

        bool DeviceGL::start( ZH::Widgets::WindowsInfo* winInfo )
        {
            // Do nothing is already initialized
            if (  (m_status == DEVICE_STATUS_RUNNING) && m_hglrc && m_hdc ){
                return true;
            }

            // Windos info should be given
            if ( !winInfo ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - winInfo is NULL!\n");
                return false;
            }

            // Create a dummy window for initializing glew
            ZH::Widgets::Window* pDummyWnd =
                ZH::Widgets::Window::CreateZHWindow("Dummy",0,0,10,10);
            if ( !pDummyWnd ){
                ZH::Util::ENG_ERR("Create dummy window failed!\n");
                return false;
            }
            ZH::Widgets::WindowsInfo* pDummyWinInfo =
                pDummyWnd->getWndInfo();
            HDC hDummyDC = GetDC( pDummyWinInfo->fWndHandle );
            if ( !hDummyDC ){
                ZH::Util::ENG_ERR("Get dummy window DC failed!\n");
                return false;
            }

            // Pixel format
            PIXELFORMATDESCRIPTOR pfd = { 
                sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
                1,                                // version number
                PFD_DRAW_TO_WINDOW |              // support window
                PFD_SUPPORT_OPENGL |              // support OpenGL
                PFD_DOUBLEBUFFER,                 // double buffered
                PFD_TYPE_RGBA,                    // RGBA type
                24,                               // 24-bit color depth
                0, 0, 0, 0, 0, 0,                 // color bits ignored
                0,                                // no alpha buffer
                0,                                // shift bit ignored
                0,                                // no accumulation buffer
                0, 0, 0, 0,                       // accum bits ignored
                32,                               // 32-bit z-buffer
                0,                                // no stencil buffer
                0,                                // no auxiliary buffer
                PFD_MAIN_PLANE,                   // main layer
                0,                                // reserved
                0, 0, 0                           // layer masks ignored
            };

            // get the device context's best, available pixel format match
            int iPixelFormat = ChoosePixelFormat(hDummyDC, &pfd);
            if ( 0 == iPixelFormat ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - Dummy window ChoosePixelFormat failed!\n");
                return false;
            }
            // Set format to dummy dc, once per window
            if ( !SetPixelFormat(hDummyDC, iPixelFormat, &pfd) ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - Dummy window SetPixelFormat failed!\n");
                return false;
            }
            // Create render context
            HGLRC dummy_hglrc = wglCreateContext( hDummyDC );
            if ( NULL == dummy_hglrc ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - dummy window wglCreateContext failed!\n");
                return false;
            }

            // Make it current
            if ( !wglMakeCurrent(hDummyDC, dummy_hglrc) ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - dummy window wglMakeCurrent failed!\n");
                return false;
            }

            // Init glew
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if ( GLEW_OK != err ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - glewInit!\n");
                ZH::Util::ENG_ERR("Error: %s\n", glewGetErrorString(err));
                return false;
            }else{
                ZH::Util::ENG_DBG("Glew initialized successfully!\n");
            }

            // Clean up dummy resources
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext( dummy_hglrc );
            ZH::Widgets::Window::DestryZHWindow();


            // Get windows device context
            HDC hDC = GetDC( winInfo->fWndHandle );


            int nPixelFormat=-1;
            int nPixCount = 0;
            float fPixAttribs[]={0,0};

            int iPixAttribs[] = {
                WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
                WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
                WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB, // must be HW accelerated
                WGL_COLOR_BITS_ARB,     32, // 8 bits of each R, G and B
                WGL_DEPTH_BITS_ARB,     24, // 24 bits of depth precision for window
                WGL_DOUBLE_BUFFER_ARB,GL_TRUE, // Double buffered context
                WGL_PIXEL_TYPE_ARB,      WGL_TYPE_RGBA_ARB, // pf should be RGBA type
                WGL_STENCIL_BITS_ARB, 8,//Stencil buffer, bit 8
                WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // MSAA on
                WGL_SAMPLES_ARB,        4, // 4x MSAA
                0
            };

            // Choose pixel format
            // http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
            BOOL result =
            wglChoosePixelFormatARB(
                hDC,                // Device context.
                iPixAttribs,        // A list of attribute {type, value} pairs
                fPixAttribs,        // Floating point attribute values
                1,                  // Maximum number of pixel formats to be returned.
                &nPixelFormat,      // Returned format
                (UINT*)&nPixCount   // Returns the number of matching formats
                );
            if ( !result || (nPixelFormat==-1) ){
                // Try again without MSAA
                iPixAttribs[17] = GL_FALSE;
                iPixAttribs[19] = 1;

                result =
                wglChoosePixelFormatARB(hDC, iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
                if ( !result || (nPixelFormat==-1) ){
                    ZH::Util::ENG_ERR("DeviceGL::start() failed - wglChoosePixelFormatARB() failed!\n");
                    return false;
                }
                ZH::Util::ENG_DBG("wglChoosePixelFormatARB() + No MSAA succeeded!\n");
            }else{
                ZH::Util::ENG_DBG("wglChoosePixelFormatARB() + 4x MSAA succeeded!\n");
            }

            // Set pixel format
            if (!SetPixelFormat( hDC, nPixelFormat, &pfd ))
            {
                ZH::Util::ENG_ERR("DeviceGL::start() failed - SetPixelFormat() failed!\n");
                return false;
            }

            // OGL version
            GLint major_ver = 4;
            GLint minor_ver = 4;

            // Create render context
            GLint attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB,  major_ver,
                WGL_CONTEXT_MINOR_VERSION_ARB,  minor_ver,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0 };
            HGLRC hglrc  = wglCreateContextAttribsARB(hDC, 0, attribs);
            while ( NULL == hglrc ){
                if ( major_ver < 3){
                    break;
                }
                if ( minor_ver > 0 ){
                    minor_ver--;
                }else{
                    major_ver--;
                    minor_ver = 3;
                }
                attribs[1] = major_ver;
                attribs[3] = minor_ver;
                hglrc  = wglCreateContextAttribsARB(hDC, 0, attribs);
            }
            if ( NULL == hglrc ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - wglCreateContextAttribsARB() failed!\n");
                return false;
            }else{
                ZH::Util::ENG_DBG("DeviceGL::start() OpenGL context version %d.%d created successfully!\n", major_ver, minor_ver);
            }

            // Make it current
            if ( !wglMakeCurrent(hDC, hglrc) ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - wglMakeCurrent failed!\n");
                return false;
            }

            // Some information
            const GLubyte* str = glGetString(GL_VERSION);
            ZH::Util::ENG_INF("GL_VERSION: %s\n", str);
            str = glGetString(GL_VENDOR);
            ZH::Util::ENG_INF("GL_VENDOR: %s\n", str);
            str = glGetString(GL_RENDERER);
            ZH::Util::ENG_INF("GL_RENDERER: %s\n", str);
            str = glGetString(GL_SHADING_LANGUAGE_VERSION);
            ZH::Util::ENG_INF("GL_SHADING_LANGUAGE_VERSION: %s\n", str);

            // Save context
            m_hglrc = (void*)hglrc;
            m_hdc   = (void*)hDC;
            m_status = DEVICE_STATUS_RUNNING;

            ZH::Util::ENG_DBG("OpenGL Device started successfully\n");

            return true;
        }

        bool DeviceGL::shutdown()
        {
            // TODO
            return true;
        }

        bool DeviceGL::setRenderTarget( RenderTarget* )
        {
            // TODO
            return true;
        }

        bool DeviceGL::clearRenderTargetView( RenderTarget*, const ZH::Math::float4& )
        {
            // TODO
            return true;
        }

        bool DeviceGL::present()
        {
            // TODO
            return true;
        }

        bool DeviceGL::createInputLayout( InputLayout&, const ShaderCodes* )
        {
            // TODO
            return true;
        }

        RenderTarget* DeviceGL::createRenderTarget( const char* const, Texture2D* )
        {
            // TODO
            return NULL;
        }

        VertexBuffer* DeviceGL::createVertexBuffer( SEMANTIC_TYPE, const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&)
        {
            // TODO
            return NULL;
        }

        IndexBuffer*  DeviceGL::createIndexBuffer ( const char* const, const BUFFER_DESC&, const SUBRESOURCE_DATA&)
        {
            // TODO
            return NULL;
        }

        bool DeviceGL::getBackBuffer( Texture2D*& )
        {
            // TODO
            return true;
        }
    }
}














