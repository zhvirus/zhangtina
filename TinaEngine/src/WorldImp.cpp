#include "Internal/Graphics/WorldImp.h"


namespace ZH{
    namespace Graphics{

        WorldImp::WorldImp()
        {
        }

        WorldImp::~WorldImp()
        {
        }

        unsigned int WorldImp::numOfNodes()const
        { 
            return static_cast<unsigned int>(m_nodes.size());
        }


    }
}
