#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>

namespace ZH{
    namespace UTIL{

        template<class T>
        class Array{
        public:
            Array();
            ~Array();
            Array( const Array& );
            Array& operator = ( const Array& );

            void clear();
            const unsigned int size() const { return m_logicalSize; }
            void push_back(T t);
            T remove(T t);

            T& operator [] ( unsigned int idx );
            void changeLogicalSizeTo( unsigned int size );
            void changePhysicalSizeTo( unsigned int size );
            unsigned int logicalSize() const { return m_logicalSize; }
            unsigned int physicalSize() const { return m_physicalSize; }


        private:
            T* m_pArr;
            unsigned int m_logicalSize;
            unsigned int m_physicalSize;

        static const unsigned int m_sIncreaseSize = 8;
        };


        template<class T>
        Array<T>::Array():
            m_pArr(NULL),
            m_logicalSize(0),
            m_physicalSize(m_sIncreaseSize)
        {
            m_pArr = new T[m_sIncreaseSize];
        }

        template<class T>
        Array<T>::~Array()
        {
            if ( m_pArr ){
                delete []m_pArr;
            }
        }

        template<class T>
        void Array<T>::clear()
        {
            if ( m_pArr ){
                memset( (void*)m_pArr, 0, sizeof(T)*m_physicalSize );
            }

            m_logicalSize = 0;
        }

        template<class T>
        void Array<T>::push_back( T t )
        {
            m_logicalSize += 1;
            if ( m_logicalSize > m_physicalSize ){
                changePhysicalSizeTo( m_physicalSize + m_sIncreaseSize );
            }

            m_pArr[m_logicalSize-1] = t;
        }

        template<class T>
        T Array<T>::remove(T t)
        {
            if ( m_logicalSize == 0 ){
                return (T)NULL;
            }

            unsigned int i=0;
            for ( ; i<m_logicalSize; ++i ){
                if ( m_pArr[i] == t){
                    break;
                }
            }

            // Not removed as no found
            if ( i == m_logicalSize ){
                return (T)NULL;
            }

            for ( unsigned int j=i+1; j<m_logicalSize; ++j ){
                m_pArr[j-1] = m_pArr[j];
            }

            return t;
        }

        template<class T>
        T& Array<T>::operator [] ( unsigned int idx )
        {
            assert( idx < m_logicalSize );
            if ( idx < m_logicalSize ){
                return m_pArr[idx];
            }else{
                static T t;
                return t;
            }
        }

        template<class T>
        void Array<T>::changeLogicalSizeTo( unsigned int size )
        {
            if ( size > m_physicalSize ){
                changePhysicalSizeTo( size );
            }
            m_logicalSize = size;
        }

        template<class T>
        void Array<T>::changePhysicalSizeTo( unsigned int size )
        {
            if ( size == 0 ){
                if ( m_pArr ){
                    delete []m_pArr;
                    m_pArr = NULL;
                }
                m_logicalSize = 0;
                m_physicalSize = 0;
            }else if ( size != m_physicalSize ){
                T* newArr = new T[size];
                m_physicalSize = size;
                if ( size < m_logicalSize ){
                    m_logicalSize = size;
                }
                memset( (void*)newArr, 0, sizeof(T)*size );
                for ( unsigned int i=0;i<m_logicalSize;++i ){
                    newArr[i] = m_pArr[i];
                }

                if ( m_pArr ){
                    delete []m_pArr;
                }

                m_pArr = newArr;
            }
        }

        template<class T>
        Array<T>::Array( const Array& arr ):
            m_pArr(NULL),
            m_physicalSize(0),
            m_logicalSize(0)
        {
            *this = arr;
        }

        template<class T>
        Array<T>&  Array<T>::operator = ( const Array& arr )
        {
            const unsigned int srcLogicalSize  = arr.logicalSize();

            if ( srcLogicalSize == 0 ){
                m_logicalSize = 0;
            }else{
                // If need change physical size
                if ( srcLogicalSize > m_physicalSize ){
                    if ( m_pArr ){
                        delete []m_pArr;
                        m_pArr = NULL;
                    }
                    m_physicalSize = srcLogicalSize + m_sIncreaseSize;
                    m_pArr = new T[ m_physicalSize ];
                }

                m_logicalSize = srcLogicalSize;
                for ( unsigned int i=0; i<srcLogicalSize; i++ ){
                    m_pArr[i] = arr.m_pArr[i];
                }
            }

            return *this;
        }

    }
}


#endif
