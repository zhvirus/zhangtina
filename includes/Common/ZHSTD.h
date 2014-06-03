#ifndef ZH_STD_H
#define ZH_STD_H

#ifdef _ZH_GRAPHICS_DLL_
#define ZH_GRAPHICS_DLL _declspec(dllexport)
#define ZH_GRAPHICS_EXTERN
#else
#define ZH_GRAPHICS_DLL _declspec(dllimport)
#define ZH_GRAPHICS_EXTERN extern
#endif

#ifdef _ZH_MATH_DLL_
#define ZH_MATH_DLL _declspec(dllexport)
#define ZH_MATH_EXTERN
#else
#define ZH_MATH_DLL _declspec(dllimport)
#define ZH_MATH_EXTERN extern
#endif

#ifdef _ZH_WIGET_DLL_
#define ZH_WIDGET_DLL _declspec(dllexport)
#define ZH_WIDGET_EXTERN
#else
#define ZH_WIDGET_DLL _declspec(dllimport)
#define ZH_WIDGET_EXTERN extern
#endif

#ifdef _ZH_UTIL_DLL_
#define ZH_UTIL_DLL _declspec(dllexport)
#define ZH_UTIL_EXTERN
#else
#define ZH_UTIL_DLL _declspec(dllimport)
#define ZH_UTIL_EXTERN extern
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

#define CLASS_TYPE_NAME_DEFINITION(CLASS) \
    char CLASS::m_cClassName[] = #CLASS;

#define CLASS_COMMON_PROTECTED_DEFINITION(CLASS) \
    CLASS::CLASS(){} \
    CLASS::~CLASS(){} \







#endif
