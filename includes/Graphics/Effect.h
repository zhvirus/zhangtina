#ifndef EFFECT_H
#define EFFECT_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Cache.h"
#include "Internal/Graphics/ShaderLibrary.h"

namespace ZH{
    namespace Graphics{

        enum E_EFFECT_KEY{
            E_EFFECT_NONE,
            E_EFFECT_SOLID_COLOR

        };

        class ZH_GRAPHICS_DLL Effect: public Resource
        {
        public:
            Effect( const char* const, E_EFFECT_KEY );
            virtual ~Effect();
            virtual bool isValid() = 0;

            E_EFFECT_KEY key()const { return m_eEffectKey; }

        protected:
            virtual bool buildEffect() = 0;
            bool createShader( E_SHADER_TYPE, E_SHADER_KEY );
            bool releaseShader();

            void* m_pVertexShader;
            void* m_pHullShader;
            void* m_pDomainShader;
            void* m_pGeometryShader;
            void* m_pPixelShader;
            void* m_pComputeShader;

            E_EFFECT_KEY m_eEffectKey;
        private:

            // Put at last line
            CLASS_TYPE_NAME_DECLEARATION
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<Effect>;
        typedef ZH::UTIL::Cache<Effect> EffectCache;

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
