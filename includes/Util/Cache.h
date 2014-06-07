#ifndef  CACHE_H
#define CACHE_H

#include <set>
#include <assert.h>

namespace ZH{
    namespace UTIL{

        template<class T>
        class Cache{
        public:
            Cache();
            ~Cache();
            void destroy();

            T* findByName( const char* const );
            T* insert( T* );
            unsigned int size() const;
        private:
            std::set<T*>* m_pCacheImp;
        };

        template<class T>
        Cache<T>::Cache():m_pCacheImp(NULL)
        {
            m_pCacheImp = new std::set<T*>();
        }

        template<class T>
        Cache<T>::~Cache(){
            destroy();
        }

        template<class T>
        void Cache<T>::destroy()
        {
            if( !m_pCacheImp )
                return;

            std::set<T*>::iterator it = m_pCacheImp->begin();
            for( ;it != m_pCacheImp->end();++it ){
                delete *it;
            }

            m_pCacheImp->clear();
            delete m_pCacheImp;
            m_pCacheImp = NULL;
        }

        template<class T>
        unsigned int Cache<T>::size() const
        {
            if ( !m_pCacheImp ){
                return 0;
            }
            return (unsigned int)m_pCacheImp->size();
        }

        template<class T>
        T* Cache<T>::findByName( const char* const name )
        {
            if ( !name ){
                return NULL;
            }

            std::set<T*>::const_iterator cIt = m_pCacheImp->begin();
            for( ;cIt != m_pCacheImp->end();++cIt ){
                const char* const temp_name = (*cIt)->name();
                assert( temp_name );
                if ( strcmp( temp_name, name) == 0 ){
                    return *cIt;
                }
            }

            return NULL;
        }

        template<class T>
        T* Cache<T>::insert( T* v )
        {
            if (!v){
                return NULL;
            }

            T* item = findByName( v->name() );

            if ( !item ){
                m_pCacheImp->insert( v );
                return v;
            }
            return NULL;
        }
    }
}




#endif
