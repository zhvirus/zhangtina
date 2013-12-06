#ifndef WORLD_IMP_H
#define WORLD_IMP_H

#include "Common/ZHSTD.h"
#include <vector>

namespace ZH{
    namespace Graphics{

        // Forwards
        class RenderNode;

        class WorldImp{
        public:
            unsigned int numOfNodes()const;

        private:
            std::vector<RenderNode*> m_nodes;
        private:
            friend class World;
            CLASS_COMMON_PROTECTED_DECLEARATION(WorldImp)
        };

    } // Graphics
} // ZH

#endif