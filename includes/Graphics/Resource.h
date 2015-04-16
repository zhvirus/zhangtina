#ifndef RESOURCE_H
#define RESOURCE_H
#include "Common/ZHSTD.h"
#include "Graphics/ClassIdentifier.h"

namespace ZH{
    namespace Graphics{

        class Context;

        class ZH_GRAPHICS_DLL Resource
        {
        public:
            unsigned int name() const { return m_name; }
            Context* context() const { return m_pCtx; }


        protected:
            Resource( Context* );
            Resource(){}

        protected:
            Context* m_pCtx;
            unsigned int m_name;


            CLASS_COMMON_PROTECTED_DECLEARATION(Resource);
        };

        class ZH_GRAPHICS_DLL Buffer : public Resource
        {
        public:
            Buffer(Context* ctx);
            Buffer(){}

            CLASS_COMMON_PROTECTED_DECLEARATION(Buffer);
        };
    }
}


#endif
