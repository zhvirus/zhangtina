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

            virtual ~RenderNodeGrid();
            virtual bool addRenderItem( RenderItem* );
            virtual bool removeRenderItem( const char* const );

        public:
            static const char* const m_sRenderNodeGridName;


        protected:
            virtual bool prepareDefaultData();
        private:
            RenderNodeGrid();

            friend class ResourceFactory;
        };


    }
}

#endif
