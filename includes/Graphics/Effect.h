#ifndef EFFECT_H
#define EFFECT_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL Effect: public Resource
        {
        public:
            Effect();
            virtual ~Effect();
            virtual bool isValid();
        private:

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        class ZH_GRAPHICS_DLL EffectInstance: public Resource
        {
        public:
            EffectInstance();
            virtual ~EffectInstance();

            bool operator==(const EffectInstance&);

        private:
            Effect* m_pEffect;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };


    } // namespace Graphics
} // namespace ZH

#endif
