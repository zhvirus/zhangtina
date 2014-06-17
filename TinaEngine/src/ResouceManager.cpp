#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Graphics/ResourceManager.h"
#include "Internal/Graphics/Internal_common_graphics.h"

#include "Graphics/DeviceDX11.h"
#include "Graphics/EffectSolid.h"
#include "Internal/Graphics/DeviceDX11Imp.h"
#include "Internal/Graphics/ResourceFactory.h"
#include "Internal/Graphics/ShaderLibrary.h"
#include "Util/Print.h"
#include "Widget/Window.h"
#include "Math/MathCommon.h"

namespace ZH{
    namespace Graphics{

        ResourceManager::ResourceManager():
            m_bInitialized(false)
        {
        }

        ResourceManager::~ResourceManager()
        {
        }

        void ResourceManager::initialize()
        {
            assert(!m_bInitialized);
            if( m_bInitialized ){
                return;
            }

            bool result = true;

            // Build shaders
            result = buildShaders();

            // Create default world
            result = result && createDefaultWorld();

            // Create default Cameras
            result = result && createDefaultCameras();

            // Create default render targets
            result = result && createDefaultRenderTargets();

            // Create default render fragment
            result = result && createDefaultRenderFragment();

            m_bInitialized = true;
            if ( result ){
                ZH::Util::ENG_DBG("ResourceManager::initialzie() succeeded.\n");
            }else{
                ZH::Util::ENG_ERR("ResourceManager::initialzie() not succeeded!\n");
            }
        }

        void ResourceManager::deinitialize()
        {
            assert(m_bInitialized);
            if( !m_bInitialized ){
                return;
            }

            // Release shaders
            clearShaders();


            m_bInitialized = false;
        }

        //----------------------------------------------------------------
        //
        //  Shaders
        //
        //----------------------------------------------------------------
        bool ResourceManager::buildShaders()
        {
            // Build shader blobs
            bool result = ShaderLibrary::instance().initialize();
            assert(result);
            if ( !result ){
                ZH::Util::ENG_ERR("ResourceManager::buildShaders() - build shaders failed!\n");
                return false;
            }

            // Create default build-in effects
            Effect* pEffect = NULL;

            // EffectSolid
            {
                pEffect = new EffectSolid();
                if ( !pEffect->isValid() ){
                    result = false;
                    delete pEffect;
                    pEffect = NULL;
                    ZH::Util::ENG_ERR("Create EffectSolid failed!\n");
                }else{
                    m_effectCache.insert( pEffect );
                    pEffect = NULL;
                    ZH::Util::ENG_DBG("Create EffectSolid succeeded.\n");
                }
            }

            return true;
        }

        void ResourceManager::clearShaders()
        {
            // Free effect
            m_effectCache.destroy();


            // Free shader blobs
            ShaderLibrary::instance().clear();
        }

        template<class S, class T>
        S* findResourceByName( const char* name, const T& cache )
        {
            assert( name );
            S* pRes = NULL;
            if ( name ){
                pRes = cache.findByName(name);
            }
            return pRes;
        }

        //----------------------------------------------------------------
        //
        //  World
        //
        //----------------------------------------------------------------
        SimpleWorld* ResourceManager::findSimpleWorldByName( const char* const name )
        {
            World* pWorld = findResourceByName<World, WorldCache>( name, m_worldCache );
            if (!pWorld){
                return NULL;
            }

            SimpleWorld* pSimpleWorld = dynamic_cast<SimpleWorld*>(pWorld);
            if ( !pSimpleWorld ){
                return NULL;
            }

            return pSimpleWorld;
        }

        SimpleWorld* ResourceManager::acquireSimpleWorld( const char* const name )
        {
            // Find in cache first
            SimpleWorld* pSimpleWorld = findSimpleWorldByName(name);
            if ( pSimpleWorld ){
                return pSimpleWorld;
            }

            pSimpleWorld = new SimpleWorld( name );

            if ( pSimpleWorld ){
                m_worldCache.insert( pSimpleWorld );
                ZH::Util::ENG_DBG("SimpleWorld (\"%s\") created successfully.\n", name );
            }else{
                ZH::Util::ENG_ERR("SimpleWorld (\"%s\") created failed!\n", name );
            }

            return pSimpleWorld;
        }

        bool ResourceManager::createDefaultWorld()
        {
            SimpleWorld* pNewSimpleWorld = acquireSimpleWorld( SimpleWorld::m_sDefaultSimpleWorldName );
            if ( !pNewSimpleWorld ){
                return false;
            }


            return true;
        }


        //----------------------------------------------------------------
        //
        //  Render target
        //
        //----------------------------------------------------------------

        // Find render target by name
        RenderTarget* ResourceManager::findRenderTargetByName( const char* const name )
        {
            return findResourceByName<RenderTarget, RenderTargetCache>( name, m_renderTargetCache );
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
            rt = pDevice->createRenderTarget( name, tex2d );
            if ( rt ){
                m_renderTargetCache.insert( rt );
                return rt;
            }

            ZH::Util::ENG_ERR("Render target (\"%s\") created failed!\n", rt->name() );
            return NULL;
        }

        bool ResourceManager::createDefaultRenderTargets()
        {
            DeviceDX11* m_pDevice = DeviceDX11::instance();
            if ( !m_pDevice || !m_pDevice->isRunning() ){
                assert(false);
                return false;
            }

            Texture2D* backBuffer = acquireBackBuffer( m_pDevice );

            RenderTarget* renderTarget = acquireRenderTarget(
                RenderTarget::m_sDefaultRenderTargetName, m_pDevice, backBuffer );

            if ( !renderTarget ){
                return false;
            }

            return true;
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
        //  Camera
        //
        //----------------------------------------------------------------

        CameraPersp* ResourceManager::findCameraPerspByName( const char* const name )
        {
            Camera* pCamera = findResourceByName<Camera, CameraCache>( name, m_cameraCache );

            if (!pCamera){
                return NULL;
            }

            CameraPersp* pCameraPersp = dynamic_cast<CameraPersp*>(pCamera);
            if ( !pCameraPersp ){
                return NULL;
            }

            return pCameraPersp;
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
            CameraPersp* pCamPersp = findCameraPerspByName( name );
            if ( pCamPersp ){
                return pCamPersp;
            }

            // Create a new persp camera
            pCamPersp = ResourceFactory::createCameraPersp(
                name, pos, lookDir, upDir, fovy, aspect, nearZ, farZ);

            if ( pCamPersp ){
                ZH::Util::ENG_DBG("CameraPersp (\"%s\") created.\n", name);
                m_cameraCache.insert( pCamPersp );
            }

            return pCamPersp;
        }

        bool ResourceManager::createDefaultCameras()
        {
            // Default perspective camera
            ZH::Math::float3 pos(0.0f,0.0f, -10.0f);
            ZH::Math::float3 lookDir(0.0f,0.0f,1.0f);
            ZH::Math::float3 upDir(0.0f,1.0f,0.0f);
            float fovy = ZH::Math::PI/3.0f;
            float aspect = 1.0f;

            CameraPersp* pCamPersp = acquireCameraPersp( CameraPersp::m_sDefaultName, pos, lookDir, upDir, fovy, aspect, 0.1f, 5000.0f);
            if ( !pCamPersp ){
                return false;
            }

            return true;
        }


        //----------------------------------------------------------------
        //
        //  Render fragment
        //
        //----------------------------------------------------------------

        RenderFragment* ResourceManager::findRenderFragmentByName( const char* const name )
        {
            return findResourceByName<RenderFragment, RenderFragmentCache>( name, m_renderFragmentCache );
        }

        RenderFragment* ResourceManager::acquireRenderFragment(
                  const char* const name,
                  Device* device,
                  Camera* camera,
                  World* world,
                  const ZH::Graphics::RenderTargetPtrArray& rts)
        {
            // Find in cache first
            RenderFragment* pRes = findRenderFragmentByName( name );
            if ( pRes ){
                return pRes;
            }

            // Create a new render fragment
            pRes = ResourceFactory::createRenderFragment(
                name, device, camera, world, rts);

            if ( pRes ){
                ZH::Util::ENG_DBG("RenderFragment (\"%s\") created.\n", name);
                m_renderFragmentCache.insert( pRes );
            }

            return pRes;
        }

        bool ResourceManager::createDefaultRenderFragment()
        {
            RenderFragment* pDefaultRenderFrag = findRenderFragmentByName(
                RenderFragment::m_sDefaultRenderFragmentName);
            if( pDefaultRenderFrag ){
                return true;
            }

            // Device
            DeviceDX11* m_pDevice = DeviceDX11::instance();
            if ( !m_pDevice || !m_pDevice->isRunning() ){
                assert(false);
                return false;
            }

            // Camera
            CameraPersp* pCamera = findCameraPerspByName( CameraPersp::m_sDefaultName );
            assert( pCamera );
            if( !pCamera ){
                return false;
            }

            // World
            SimpleWorld* pSimpleWorld = findSimpleWorldByName( SimpleWorld::m_sDefaultSimpleWorldName );
            assert( pSimpleWorld );
            if( !pSimpleWorld ){
                return false;
            }

            // Render targets
            RenderTargetPtrArray renderTargets;
            RenderTarget* renderTarget = findRenderTargetByName( RenderTarget::m_sDefaultRenderTargetName );
            assert( renderTarget );
            if( !renderTarget ){
                return false;
            }
            renderTargets.push_back(renderTarget);

            // Render fragment
            RenderFragment* pRenderFrag = acquireRenderFragment(
                RenderFragment::m_sDefaultRenderFragmentName, m_pDevice, pCamera, pSimpleWorld, renderTargets);
            if ( !pRenderFrag ){
                return false;
            }

            return true;
        }

        //----------------------------------------------------------------
        //
        //  Vertex buffer
        //
        //----------------------------------------------------------------
        VertexBuffer* ResourceManager::findVertexBufferByName( const char* const name )
        {
            return findResourceByName<VertexBuffer, VertexBufferCache>( name, m_vertexBufferCache );
        }

        VertexBuffer* ResourceManager::acquireVertexBuffer(
                const char* const name,
                Device* device,
                const BUFFER_DESC& desc,
                const SUBRESOURCE_DATA& data )
        {
            // Find in cache first
            VertexBuffer* pRes = findVertexBufferByName( name );
            if ( pRes ){
                return pRes;
            }

            // Create a new vertex buffer
            if ( !device ){
                assert( device );
                ZH::Util::ENG_ERR("ResourceManager::acquireVertexBuffer(), device is NULL!\n");
                return NULL;
            }

            VertexBuffer* vb = device->createVertexBuffer( name, desc, data );
            if ( vb ){
                m_vertexBufferCache.insert( vb );
            }

            return vb;
        }


        //----------------------------------------------------------------
        //
        //  Index buffer
        //
        //----------------------------------------------------------------
        IndexBuffer* ResourceManager::findIndexBufferByName( const char* const name )
        {
            return findResourceByName<IndexBuffer, IndexBufferCache>( name, m_indexBufferCache );
        }

        IndexBuffer* ResourceManager::acquireIndexBuffer(
                const char* const name,
                Device* device,
                const BUFFER_DESC& desc,
                const SUBRESOURCE_DATA& data )
        {
            // Find in cache first
            IndexBuffer* pRes = findIndexBufferByName( name );
            if ( pRes ){
                return pRes;
            }

            // Create a new vertex buffer
            if ( !device ){
                assert( device );
                ZH::Util::ENG_ERR("ResourceManager::acquireIndexBuffer(), device is NULL!\n");
                return NULL;
            }

            IndexBuffer* ib = device->createIndexBuffer( name, desc, data );
            if ( ib ){
                m_indexBufferCache.insert( ib );
            }

            return ib;
        }

    }
}



