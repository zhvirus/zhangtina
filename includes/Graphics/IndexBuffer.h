#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL IndexBuffer: public Resource
        {
        public:
            IndexBuffer( const char* const );
            virtual ~IndexBuffer();
            virtual bool isValid();

            bool operator==(const IndexBuffer&);

        private:
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };


    } // namespace Graphics
} // namespace ZH

#endif
