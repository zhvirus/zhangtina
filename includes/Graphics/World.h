#ifndef WORLD_H
#define WORLD_H

#include "Common/ZHSTD.h"

namespace ZH {
    namespace Graphics{

        class ZH_GRAPHICS_DLL World
        {
        public:
            static World& instance(){static World w; return w;}
        public:
            unsigned int numOfNodes()const;
        private:
            void* m_pImp;
        private:
            CLASS_COMMON_PROTECTED_DECLEARATION(World)
            CLASS_TYPE_NAME_DECLEARATION
        };
    } // namespace Graphics
} // namespace ZH

#endif