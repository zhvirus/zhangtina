#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/Enums.h"
#include "Util/Cache.h"

namespace ZH{
    namespace Graphics{

        // forwards
        class Texture2DImp;

        // Describes multi-sampling parameters for a resource
        class ZH_GRAPHICS_DLL TEX2D_SAMPLE_DESC {
        public:
            TEX2D_SAMPLE_DESC(unsigned int c, unsigned int q):
                Count(c),Quality(q){}
            TEX2D_SAMPLE_DESC():Count(1),Quality(0){}
            unsigned int Count; // The number of multisamples per pixel.
            unsigned int Quality; // The image quality level. The higher
            // the quality, the lower the performance.
            // The valid range is between zero and one less
            // than the level returned by
            // ID3D10Device::CheckMultisampleQualityLevels.

            bool operator == ( const TEX2D_SAMPLE_DESC& );
        };

        class ZH_GRAPHICS_DLL TEX2D_DESC
        {
        public:
            TEX2D_DESC():
                Width(0),
                Height(0),
                MipLevels(1),
                ArraySize(1),
                Format(TEX2D_FORMAT_R8G8B8A8_UNORM),
                Usage(USAGE_DEFAULT),
                BindFlags(BIND_SHADER_RESOURCE),
                CPUAccessFlags(0),
                MiscFlags(0){}

            bool operator == ( const TEX2D_DESC& );

            unsigned int        Width;
            unsigned int        Height;
            unsigned int        MipLevels;
            unsigned int        ArraySize;
            TEX2D_FORMAT        Format;
            TEX2D_SAMPLE_DESC   SampleDesc;
            USAGE               Usage;
            unsigned int        BindFlags;
            unsigned int        CPUAccessFlags;
            unsigned int        MiscFlags;
        };

        class ZH_GRAPHICS_DLL Texture2D : public Resource
        {
        public:
            virtual ~Texture2D();

            virtual bool isValid();

            bool operator == ( const Texture2D& );


        public:
            static const char m_sBackBufferName[30];

        private:
            Texture2D( const char* const, Texture2DImp* );

        private:
            TEX2D_DESC m_sDesc;
            Texture2DImp* m_pTex2DImp;

            friend class DeviceDX11;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<Texture2D>;
        typedef ZH::UTIL::Cache<Texture2D> Texture2DCache;


    }

}

#endif
