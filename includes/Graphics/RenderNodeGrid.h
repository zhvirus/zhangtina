#ifndef RENDER_NODE_GRID_H
#define RENDER_NODE_GRID_H

#include "Common/ZHSTD.h"
#include "Graphics/RenderNode.h"


namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL RenderNodeGrid : public RenderNode
        {
            CLASS_IDENTIFIER( E_CID_RENDER_NODE_GRID );
        public:
            RenderNodeGrid();
            virtual ~RenderNodeGrid();

        private:
        };


    }
}

#endif
