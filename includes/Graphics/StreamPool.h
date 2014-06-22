#ifndef STREAM_POOL_H
#define STREAM_POOL_H

#include "Common/ZHSTD.h"
#include "Graphics/Enums.h"
#include <map>

namespace ZH{
    namespace Graphics{

        // Forwards
        class VertexBuffer;


        class ZH_GRAPHICS_DLL StreamPool
        {
        public:
            StreamPool& instance() { static StreamPool sp; return sp; }


        private:
            typedef std::map<const char* const, VertexBuffer*>  CHAR_VB_MAP;
            typedef std::map<STREAM_TYPE, CHAR_VB_MAP* >        STREAM_CHARVB_MAP;
            STREAM_CHARVB_MAP* m_streamPool;

            void clear();


        private:
            StreamPool();
            ~StreamPool();
            StreamPool( const StreamPool& ){}
            StreamPool& operator = ( const StreamPool& ){ return *this; }
        };



    }
}


#endif
