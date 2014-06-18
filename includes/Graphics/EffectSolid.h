#ifndef EFFECT_SOLID_H
#define EFFECT_SOLID_H

#include "Graphics/ClassIdentifier.h"
#include "Graphics/Effect.h"
#include "Math/float4.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL EffectSolid: public Effect
        {
            CLASS_IDENTIFIER( E_CID_EFFECT_SOLID );
            static const char* const m_sDefaultName;
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
            EffectSolid( const char* const );
        };


        class ZH_GRAPHICS_DLL EffectInstanceSolid : public EffectInstance
        {
            CLASS_IDENTIFIER( E_CID_EFFECT_INSTANCE_SOLID );
        public:
            virtual ~EffectInstanceSolid();
            virtual bool isValid() const;

        private:

            friend class ResourceFactory;
            EffectInstanceSolid( const char* const );

        };

    }
}

#endif
