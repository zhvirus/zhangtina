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

            virtual bool isValid();

            bool setColor( const ZH::Math::float4& );

        protected:
            virtual bool buildEffect();

        private:
            ZH::Math::float4 m_solidColor;

        private:

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };
    }
}

#endif
