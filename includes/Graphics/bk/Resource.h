#ifndef RESOURCE_H
#define RESOURCE_H

#include "Common/ZHSTD.h"
#include "Graphics/Name.h"
#include "Graphics/Enums.h"

namespace ZH {
    namespace Graphics{

        class ZH_GRAPHICS_DLL BUFFER_DESC
        {
        public:
            BUFFER_DESC():
                buffSizeInByte(0),
                usage( USAGE_DEFAULT ),
                bind_flag( BIND_VERTEX_BUFFER ),
                cpu_access_flag( CPU_ACCESS_WRITE ),
                misc_flag(RESOURCE_MISC_UNUSED)
            {}
            BUFFER_DESC( const BUFFER_DESC& desc );

            unsigned int    buffSizeInByte;
            USAGE           usage;
            BIND_FLAG       bind_flag;
            CPU_ACCESS_FLAG cpu_access_flag;
            RESOURCE_MISC_FLAG misc_flag;
        };


        class ZH_GRAPHICS_DLL SUBRESOURCE_DATA
        {
        public:
            SUBRESOURCE_DATA():
                pSysMem(NULL),
                sysMemPitch(0),
                sysMemSlicePitch(0)
            {}

            void* pSysMem;
            unsigned int sysMemPitch;
            unsigned int sysMemSlicePitch;
        };

        class ZH_GRAPHICS_DLL Resource : public Name
        {
        public:
            Resource( const char* const );
            virtual ~Resource();
            virtual bool isValid() { return false; };
        protected:

        };
    }
}

#endif
