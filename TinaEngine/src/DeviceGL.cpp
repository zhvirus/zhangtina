#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/DeviceGL.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Util/Print.h"
#include "Widget/Window.h"

#include <windows.h>
#include "gl/glew.h"

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
            if (  (m_status == DEVICE_STATUS_RUNNING) && m_hglrc && m_hdc ){
                return true;
            }

            if ( !winInfo ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - winInfo is NULL!\n");
                return false;
            }

            // Init glew
            glewInit();

            // Get windows device context
            HDC hdc = GetDC( winInfo->fWndHandle );

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
            int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
            if ( 0 == iPixelFormat ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - ChoosePixelFormat failed!\n");
                return false;
            }

            // make that match the device context's current pixel format  
            if ( !SetPixelFormat(hdc, iPixelFormat, &pfd) ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - SetPixelFormat failed!\n");
                return false;
            }

            // Create render context
            HGLRC hglrc = wglCreateContext( hdc );
            if ( NULL == hglrc ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - wglCreateContext failed!\n");
                return false;
            }

            // Make it current
            if ( !wglMakeCurrent(hdc, hglrc) ){
                ZH::Util::ENG_ERR("DeviceGL::start() failed - wglMakeCurrent failed!\n");
                return false;
            }

            m_hglrc = (void*)hglrc;
            m_hdc   = (void*)hdc;
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














