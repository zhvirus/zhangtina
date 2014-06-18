#ifndef EFFECT_SOLID_H
#define EFFECT_SOLID_H

#include "Effect.h"
#include "Math/float4.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL EffectSolid: public Effect
        {
            CLASS_IDENTIFIER( E_CID_EFFECT_SOLID );
        public:

            virtual ~EffectSolid();

            virtual bool isValid();

            bool setColor( const ZH::Math::float4& );

        protected:
            virtual bool buildEffect();

        private:
            ZH::Math::float4 m_solidColor;

        private:
            friend class ResourceManager;
            EffectSolid();
        };
    }
}

#endif
