#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif

#include "Graphics/ResourceManager.h"
#include "Graphics/DeviceDX11.h"

namespace ZH{
    namespace Graphics{

        ResourceManager::ResourceManager()
        {
        }

        ResourceManager::~ResourceManager()
        {
        }

        RenderTarget* ResourceManager::createRenderTarget(
            Device* pDevice,
            Texture2D* tex2d,
            const std::string& name)
        {
            // Find in cache first
            RenderTarget* rt = m_renderTargetCache.findByName(name);
            if ( rt ){
                std::cout<<"Render target '"<<name<<"' already existed in the cache!"
                    <<" Ignore create it again!!"<<std::endl;
                return rt;
            }

            // Create render target through device interface
            if ( pDevice->createRenderTarget( tex2d, &rt, name ) ){
                if ( m_renderTargetCache.insert( rt ) ){
                    std::cout<<"Render target(\""<<rt->name()<<"\") created successfully!"<<std::endl;
                    return rt;
                }
            }

            if ( rt ){
                delete rt;
                rt = NULL;
            }

            std::cerr<<"ERROR: Render target(\""<<rt->name()<<"\") creating failed!"<<std::endl;
            return NULL;
        }


    }
}



