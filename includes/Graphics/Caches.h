#ifndef CACHES_H
#define CACHES_H

#include "Graphics/Texture2D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Effect.h"
#include <vector>

namespace ZH{
    namespace Graphics{

        template <class T>
        class Cache{
        public:
            Cache();
            ~Cache();
            void destroyCache();
            T* acquire(const T&);
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
        T* Cache<T>::acquire( const T& )
        {
            return NULL;
        }

        template<class T>
        T* Cache<T>::findByName( const std::string& name )
        {
            return name.empty()?(T*)NULL:(T*)NULL;
        }

        //******************************************
        // ResourceCaches
        //******************************************
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<Texture2D>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<VertexBuffer>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<IndexBuffer>;
        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL Cache<EffectInstance>;

        class ZH_GRAPHICS_DLL ResourceCaches
        {
        public:
            static ResourceCaches& Instance(){ static ResourceCaches r; return r; }

            Cache<Texture2D>&      Textures()        { return m_textureCaches; }
            Cache<VertexBuffer>&   VertexBuffers()   { return m_vertexBufferCaches; }
            Cache<IndexBuffer>&    IndexBuffers()    { return m_indexBufferCaches; }
            Cache<EffectInstance>& EffectInstances() { return m_EffectInstanceCaches; }

        private:
            Cache<Texture2D>        m_textureCaches;
            Cache<VertexBuffer>     m_vertexBufferCaches;
            Cache<IndexBuffer>      m_indexBufferCaches;
            Cache<EffectInstance>   m_EffectInstanceCaches;

            // Put at last line
            CLASS_COMMON_PROTECTED_DECLEARATION(ResourceCaches)
        };




    }



}


#endif
