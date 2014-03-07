#ifndef CACHES_H
#define CACHES_H

#include "Graphics/Texture2D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Effect.h"
#include "Graphics/World.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderFragment.h"
#include <vector>

namespace ZH{
    namespace Graphics{

        template <class T>
        class Cache{
        public:
            Cache();
            ~Cache();
            void destroyCache();
            T* acquire(T*);
            T* findByName(const std::string&);
        private:
            std::vector<T*>* m_pCache;
        };


        template<class T>
        Cache<T>::Cache():m_pCache(NULL){
            m_pCache = new std::vector<T*>();
        }

        template<class T>
        Cache<T>::~Cache(){
            destroyCache();
        }

        template<class T>
        void Cache<T>::destroyCache()
        {
            if( !m_pCache )
                return;

            std::vector<T*>::iterator it = m_pCache->begin();
            for( ;it != m_pCache->end();++it ){
                delete *it;
            }

            m_pCache->clear();
            delete m_pCache;
            m_pCache = NULL;
        }

        template<class T>
        T* Cache<T>::acquire( T* v )
        {
            std::vector<T*>::iterator it = m_pCache->begin();
            for( ;it != m_pCache->end();++it ){
                if ( (*it) == v ){
                    return *it;
                }
            }

            m_pCache->push_back( v );

            return v;
        }

        template<class T>
        T* Cache<T>::findByName( const std::string& name )
        {
            std::vector<T*>::const_iterator cIt = m_pCache->begin();
            for( ;cIt != m_pCache->end();++cIt ){
                if ( (*cIt)->name() == name ){
                    return *cIt;
                }
            }

            return NULL;
        }

        //******************************************
        // ResourceCaches
        //******************************************
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<Texture2D>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<VertexBuffer>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<IndexBuffer>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<EffectInstance>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<RenderTarget>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<RenderFragment>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<Camera>;


        class ZH_GRAPHICS_DLL ResourceCaches
        {
        public:
            static ResourceCaches& Instance(){ static ResourceCaches r; return r; }

            Cache<Texture2D>&      Texture2Ds()      { return m_texture2DCache; }
            Cache<VertexBuffer>&   VertexBuffers()   { return m_vertexBufferCache; }
            Cache<IndexBuffer>&    IndexBuffers()    { return m_indexBufferCache; }
            Cache<EffectInstance>& EffectInstances() { return m_effectInstanceCache; }
            Cache<RenderTarget>&   RenderTargets()   { return m_renderTargetCache; }
            Cache<RenderFragment>& RenderFragments() { return m_renderFragmentCache; }
            Cache<Camera>&         Cameras()         { return m_cameraCache; }

        private:
            Cache<Texture2D>        m_texture2DCache;
            Cache<VertexBuffer>     m_vertexBufferCache;
            Cache<IndexBuffer>      m_indexBufferCache;
            Cache<EffectInstance>   m_effectInstanceCache;
            Cache<RenderTarget>     m_renderTargetCache;
            Cache<RenderFragment>   m_renderFragmentCache;
            Cache<Camera>           m_cameraCache;

            // Put at last line
            CLASS_COMMON_PROTECTED_DECLEARATION(ResourceCaches)
        };




    }



}


#endif
