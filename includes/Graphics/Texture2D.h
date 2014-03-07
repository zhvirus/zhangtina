#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Graphics/Enums.h"
#include "Internal/Common/internal_common.h"

namespace ZH{
    namespace Graphics{

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
            Texture2D();
            virtual ~Texture2D();

            virtual bool isValid();

            ID3D11Texture2D* getTex() { return m_tex2D; }

        private:




        private:
            TEX2D_DESC m_sDesc;
            ID3D11Texture2D* m_tex2D;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };
    }

}

#endif
