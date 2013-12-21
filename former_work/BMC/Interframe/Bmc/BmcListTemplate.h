#pragma once

#include <vector>
#include <fstream>

template<typename T>
class BmcList
{
    public:
        BmcList();
        ~BmcList();
    public:
        int    getListSize();
        T*     getAt(int index);
        T* operator[](int index);
        bool   add(T* _inData);
        bool   add(T& _inData);
        bool   delAt(int index);
        bool   clearAll();
        bool   saveDataToFile(char* fileName);
        bool   loadFromFile(char* fileName);
    private:
        std::vector<T*> m_list;
        typename std::vector<T*>::iterator   m_ite;
        int m_size;
};

template<typename T>
T* BmcList<T>::operator [](int index)
{
   if(index<0||index>m_size-1)
       return NULL;
   return m_list[index];
}


template<typename T>
BmcList<T>::BmcList()
{
    m_size  =  0;
}

template<typename T>
BmcList<T>::~BmcList()
{

    clearAll();
}

template<typename T>
int BmcList<T>::getListSize()
{
    return static_cast<int>(m_list.size());
}

template<typename T>
T* BmcList<T>::getAt(int index)
{
    if(index<0||index>=m_size)
        return NULL;

    return m_list[index];
}

template<typename   T>
bool BmcList<T>::add(T *_inData)
{
    if(_inData==NULL)
        return false;
    m_list.push_back(_inData);

    m_size  =  static_cast<int>(m_list.size());
    return   true;
}

template<typename   T>
bool BmcList<T>::add(T& _inData)
{
    T* t_data  =  new T;
    if(t_data==NULL)
        return false;

    memcpy((void*)t_data,(void*)(&_inData),sizeof(T));

    m_list.push_back(t_data);

    m_size  =  static_cast<int>(m_list.size());
    return true;
}

template<typename T>
bool BmcList<T>::delAt(int index)
{
    if(index<0||index>=m_size)
        return false;   

    m_ite    = m_list.begin();
    for(int i=0;i<index;i++,m_ite++);
    
    MY_DELETE(*m_ite);
    m_list.erase(m_ite);
    m_size  =  static_cast<int>(m_list.size());  
    return true;

}


template<typename T>
bool BmcList<T>::clearAll()
{

    m_ite    = m_list.begin();
    for(;m_ite!=m_list.end();m_ite++)
    {
        MY_DELETE(*m_ite);
    }

    m_list.clear();
    m_size  =  static_cast<int>(m_list.size());
    return true;

}

template<typename T>
bool   BmcList<T>::saveDataToFile(char* fileName)
{
    if(fileName==NULL)
        return false;
    
    std::ofstream ooFile(fileName,std::ios::binary);
    if(ooFile.fail())
        return false;

    m_size  =  static_cast<int>(m_list.size());
    if(m_size<=0)
        return false;

    ooFile.write((char*)(&m_size),sizeof(int));

    for(m_ite=m_list.begin(); m_ite!=m_list.end(); m_ite++)
    {
        T* _temp  =  *m_ite;
        ooFile.write((char*)(_temp),sizeof(T));
    }

    return true;
}

template<typename T>
bool   BmcList<T>::loadFromFile(char* fileName)
{
    if(fileName==NULL)
        return false;

    std::ifstream iiFile(fileName,std::ios::binary);
    if(iiFile.fail())
        return false;
    
    int size  = 0;
    iiFile.read((char*)(&size),sizeof(int));
    if(size<=0)
        return false;

    clearAll();

    T* buffer=NULL;
    for(int i=0;i<size;i++)
    {
        buffer    = new T;

        if(buffer==NULL)
        {  
            clearAll();
            return false;
        }

        iiFile.read((char*)(buffer),sizeof(T));
        m_list.push_back((T*)(buffer));

    }

    m_size  =  static_cast<int>(m_list.size());

    return true;

}