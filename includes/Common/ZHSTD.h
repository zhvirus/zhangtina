#ifndef ZH_STD_H
#define ZH_STD_H

#ifdef _ZH_DLL_
#define ZH_DLL _declspec(dllexport)
#define ZH_TEMPLATE_DLL
#else
#define ZH_DLL _declspec(dllimport)
#define ZH_TEMPLATE_DLL extern
#endif

#ifndef NULL
#define NULL 0
#endif

// Declaration Macro
#define CLASS_COMMON_PROTECTED_DECLEARATION(CLASS) \
    protected: \
        CLASS(); \
        virtual ~CLASS(); \
        CLASS( const CLASS & ){} \
        CLASS& operator=( const CLASS & ){ return *this; }

#define CLASS_TYPE_NAME_DECLEARATION \
    public: \
        static char  m_cClassName[100]; \
        static char* CLASS_NAME(){ return m_cClassName; }

// Definition Macro

#define CLASS_TYPE_NAME_DEFINITION(CLASS,NAME) \
    char CLASS::m_cClassName[] = #NAME;

#define CLASS_COMMON_PROTECTED_DEFINITION(CLASS) \
    CLASS::CLASS(){} \
    CLASS::~CLASS(){} \







#endif
