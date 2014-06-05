#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Graphics/ResourceManager.h"
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Internal/Graphics/ResourceFactory.h"
#include "Util/Print.h"
#include "Widget/Window.h"
#include "Math/MathCommon.h"

namespace ZH{
    namespace Graphics{

        ResourceManager::ResourceManager()
        {
        }

        ResourceManager::~ResourceManager()
        {
        }


        //----------------------------------------------------------------
        //
        //  Render target
        //
        //----------------------------------------------------------------

        // Find render target by name
        RenderTarget* ResourceManager::findRenderTargetByName( const char* const name )
        {
            assert( name );

            RenderTarget* rt = NULL;
            if ( name ){
                rt = m_renderTargetCache.findByName(name);
            }

            if ( rt ){
                ZH::Util::ENG_DBG("Render target (\"%s\") found in cache.\n", name);
            }else{
                ZH::Util::ENG_DBG("Render target (\"%s\") *NOT* found in cache.\n", name);
            }

            return rt;
        }

        // Acquire render target
        RenderTarget* ResourceManager::acquireRenderTarget(
            const char* const name,
            Device* pDevice,
            Texture2D* tex2d
            )
        {
            // Find in cache first
            RenderTarget* rt = findRenderTargetByName(name);
            if ( rt ){
                return rt;
            }

            // Create render target through device interface
            if ( pDevice->createRenderTarget( tex2d, rt, name ) ){
                if ( rt && m_renderTargetCache.insert( rt ) ){
                    ZH::Util::ENG_DBG("Render target (\"%s\") created successfully!\n", rt->name() );
                    return rt;
                }
            }

            ZH::Util::ENG_ERR("Render target (\"%s\") created failed!\n", rt->name() );
            return NULL;
        }


        //----------------------------------------------------------------
        //
        //  Back buffer
        //
        //----------------------------------------------------------------

        // Acquire back buffer
        Texture2D* ResourceManager::acquireBackBuffer( Device* pDevice )
        {
            // First find in cache, if found just return it
            Texture2D* bf = m_texture2DCache.findByName(Texture2D::m_sBackBufferName);
            if ( bf ){
                ZH::Util::ENG_DBG("Back buffer (\"%s\") returned in cache.\n", Texture2D::m_sBackBufferName);
                return bf;
            }

            if ( !pDevice ){
                assert( pDevice );
                ZH::Util::ENG_ERR("ResourceManager::acquireRenderTarget(), pDevice is NULL!\n");
                return NULL;
            }

            Texture2D* backBuffer = NULL;
            bool ret = pDevice->getBackBuffer( backBuffer );

            if ( !ret || !backBuffer ){
                ZH::Util::ENG_ERR("Get back buffer failed!\n");
                return NULL;
            }

            m_texture2DCache.insert( backBuffer );

            return backBuffer;
        }

        //----------------------------------------------------------------
        //
        //  Perspective camera
        //
        //----------------------------------------------------------------

        CameraPersp* ResourceManager::findCameraPerspByName( const char* const name )
        {
            assert( name );

            CameraPersp* pCam = NULL;
            if ( name ){
                pCam = m_cameraPerspCache.findByName(name);
            }

            if ( pCam ){
                ZH::Util::ENG_DBG("CameraPersp (\"%s\") found in cache.\n", name);
            }else{
                ZH::Util::ENG_DBG("CameraPersp (\"%s\") *NOT* found in cache.\n", name);
            }

            return pCam;
        }

        CameraPersp* ResourceManager::acquireDefaultCameraPersp()
        {
            // Find in cache first
            CameraPersp* pObj = findCameraPerspByName( CameraPersp::m_sDefaultName );
            if ( pObj ){
                return pObj;
            }

            // Default camera
            ZH::Math::float3 pos(0.0f,0.0f, -10.0f);
            ZH::Math::float3 lookDir(0.0f,0.0f,1.0f);
            ZH::Math::float3 upDir(0.0f,1.0f,0.0f);
            float fovy = ZH::Math::PI/3.0f;
            float aspect = 1.0f;

            return acquireCameraPersp( CameraPersp::m_sDefaultName, pos, lookDir, upDir, fovy, aspect, 0.1f, 5000.0f);
        }

        CameraPersp* ResourceManager::acquireCameraPersp(
                const char* const name,
                const ZH::Math::float3& pos,
                const ZH::Math::float3& lookDir,
                const ZH::Math::float3& upDir,
                float fovy,
                float aspect,
                float nearZ,
                float farZ)
        {
            // Find in cache first
            CameraPersp* pCam = findCameraPerspByName( name );
            if ( pCam ){
                return pCam;
            }

            // Create a new persp camera
            pCam = ResourceFactory::createCameraPersp(
                name, pos, lookDir, upDir, fovy, aspect, nearZ, farZ);

            if ( pCam ){
                ZH::Util::ENG_DBG("CameraPersp (\"%s\") created.\n", name);
                m_cameraPerspCache.insert( pCam );
            }

            return pCam;
        }


        //----------------------------------------------------------------
        //
        //  Render fragment
        //
        //----------------------------------------------------------------

        RenderFragment* ResourceManager::findRenderFragmentByName( const char* const name )
        {
            assert( name );

            RenderFragment* pObj = NULL;
            if ( name ){
                pObj = m_renderFragmentCache.findByName(name);
            }

            if ( pObj ){
                ZH::Util::ENG_DBG("RenderFragment (\"%s\") found in cache.\n", name);
            }else{
                ZH::Util::ENG_DBG("RenderFragment (\"%s\") *NOT* found in cache.\n", name);
            }

            return pObj;
        }

        RenderFragment* ResourceManager::acquireRenderFragment(
                  const char* const name,
                  Device* device,
                  Camera* camera,
                  World* world,
                  const ZH::Graphics::RenderTargetPtrArray& rts)
        {
            // Find in cache first
            RenderFragment* pObj = findRenderFragmentByName( name );
            if ( pObj ){
                return pObj;
            }

            // Create a new render fragment
            pObj = ResourceFactory::createRenderFragment(
                name, device, camera, world, rts);

            if ( pObj ){
                ZH::Util::ENG_DBG("RenderFragment (\"%s\") created.\n", name);
                m_renderFragmentCache.insert( pObj );
            }

            return pObj;
        }


    }
}



