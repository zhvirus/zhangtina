#ifndef EFFECT_SOLID_H
#define EFFECT_SOLID_H

#include "Effect.h"
#include "Math/float4.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL EffectSolid: public Effect
        {
        public:
            EffectSolid();
            virtual ~EffectSolid();

            bool setColor( const ZH::Math::float4& );

        private:
            ZH::Math::float4 m_solidColor;

        private:
            static const char* const m_sEffectName;
            static const char* const m_sShaderCodes;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };
    }
}

#endif
