#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Graphics/ResourceManager.h"
#include "Graphics/DeviceDX11.h"
#include "Internal/Graphics/DeviceDX11Imp.h"

namespace ZH{
    namespace Graphics{

        ResourceManager::ResourceManager()
        {
        }

        ResourceManager::~ResourceManager()
        {
        }

        RenderTarget* ResourceManager::acquireRenderTarget(
            Device* pDevice,
            Texture2D* tex2d,
            const char* const name)
        {
            // Find in cache first
            RenderTarget* rt = m_renderTargetCache.findByName(name);
            if ( rt ){
                std::cout<<"Render target '"<<name<<"' already existed in the cache!"
                    <<" Ignore create it again!!"<<std::endl;
                return rt;
            }

            // Create render target through device interface
            if ( pDevice->createRenderTarget( tex2d, rt, name ) ){
                if ( rt && m_renderTargetCache.insert( rt ) ){
                    std::cout<<"Render target(\""<<rt->name()<<"\") created successfully!"<<std::endl;
                    return rt;
                }
            }

            std::cerr<<"ERROR: Render target(\""<<name<<"\") creating failed!"<<std::endl;
            return NULL;
        }

        Texture2D* ResourceManager::acquireBackBuffer( Device* pDevice )
        {
            // First find in cache, if found just return it
            Texture2D* bf = m_texture2DCache.findByName(Texture2D::m_sBackBufferName);
            if ( bf ){
                std::cout<<"Back buffer "<<bf<<" returned in cache"<<std::endl;
                return bf;
            }

            if ( !pDevice ){
                std::cerr<<"ERROR: Device is NULL"<<std::endl;
                return NULL;
            }

            Texture2D* backBuffer = NULL;
            bool ret = pDevice->getBackBuffer( backBuffer );

            if ( !ret || !backBuffer ){
                return NULL;
            }

            m_texture2DCache.insert( backBuffer );

            return backBuffer;
        }


    }
}



