#ifndef RENDERER_H
#define RENDERER_H
#include "Common/ZHSTD.h"


namespace ZH
{
    namespace Bridge{

        class ZH_BRIDGE_DLL Renderer
        {
        public:
            static Renderer& instance() { static Renderer r; return r;}

        private:
            Renderer();
            ~Renderer();



        };


    }
}

#endif
