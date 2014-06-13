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
        //  Render target
        //
        //----------------------------------------------------------------
        World* ResourceManager::findWorldByName( const char* const name )
        {
            return findResourceByName<World, WorldCache>( name, m_worldCache );
        }

        World* ResourceManager::acquireWorld( const char* const name )
        {
            // Find in cache first
            World* pWorld = findWorldByName(name);
            if ( pWorld ){
                return pWorld;
            }

            World* newWorld = new World( name );

            if ( newWorld ){
                m_worldCache.insert( newWorld );
                ZH::Util::ENG_DBG("World (\"%s\") created successfully.\n", name );
            }else{
                ZH::Util::ENG_ERR("World (\"%s\") created failed!\n", name );
            }

            return newWorld;
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
            return findResourceByName<CameraPersp, CameraPerspCache>( name, m_cameraPerspCache );
        }

        CameraPersp* ResourceManager::acquireDefaultCameraPersp()
        {
            // Find in cache first
            CameraPersp* pRes = findCameraPerspByName( CameraPersp::m_sDefaultName );
            if ( pRes ){
                return pRes;
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



