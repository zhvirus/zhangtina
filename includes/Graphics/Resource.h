#ifndef RESOURCE_H
#define RESOURCE_H

#include "Common/ZHSTD.h"
#include "Graphics/Name.h"

namespace ZH {
    namespace Graphics{
        class ZH_GRAPHICS_DLL Resource : public Name
        {
        public:
            Resource();
            Resource( const Resource& );
            virtual ~Resource();
            virtual bool isValid() { return false; };
        protected:

        };
    }
}

#endif
