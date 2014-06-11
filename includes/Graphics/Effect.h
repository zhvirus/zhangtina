#ifndef EFFECT_H
#define EFFECT_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Cache.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL Effect: public Resource
        {
        public:
            Effect( const char* const );
            virtual ~Effect();
            virtual bool isValid();

        protected:
            bool buildEffect( const char* const sourceName, const char* const codes, size_t size );

        private:
            void* m_pShader;
            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        class ZH_GRAPHICS_DLL EffectInstance: public Resource
        {
        public:
            EffectInstance( const char* const );
            virtual ~EffectInstance();

            bool operator==(const EffectInstance&);

        private:
            Effect* m_pEffect;

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<Effect>;
        typedef ZH::UTIL::Cache<Effect> EffectCache;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<EffectInstance>;
        typedef ZH::UTIL::Cache<EffectInstance> EffectInstanceCache;

    } // namespace Graphics
} // namespace ZH

#endif
