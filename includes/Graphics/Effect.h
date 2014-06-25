#ifndef EFFECT_H
#define EFFECT_H

#include "Common/ZHSTD.h"
#include "Graphics/Resource.h"
#include "Util/Cache.h"
#include "Internal/Graphics/ShaderLibrary.h"
#include "Graphics/InputLayout.h"

namespace ZH{
    namespace Graphics{

        class ZH_GRAPHICS_DLL Effect: public Resource
        {
            CLASS_IDENTIFIER( E_CID_EFFECT );
        public:
            Effect( const char* const );
            virtual ~Effect();
            virtual bool isValid() = 0;

            const InputLayout& inputLayout() const { return m_inputLayout; }


        private:

        protected:
            virtual bool buildEffect() = 0;
            bool createShader( E_SHADER_TYPE, E_SHADER_KEY );
            bool releaseShader();

            InputLayout m_inputLayout;

            void* m_pVertexShader;
            void* m_pHullShader;
            void* m_pDomainShader;
            void* m_pGeometryShader;
            void* m_pPixelShader;
            void* m_pComputeShader;
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<Effect>;
        typedef ZH::UTIL::Cache<Effect> EffectCache;

        class ZH_GRAPHICS_DLL EffectInstance: public Resource
        {
            CLASS_IDENTIFIER( E_CID_EFFECT_INSTANCE );
        public:
            Effect* effect()const { return m_pEffect; }
            EffectInstance( const char* const );
            virtual ~EffectInstance();

            bool operator==(const EffectInstance&);

        protected:
            Effect* m_pEffect;
        };

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<Effect>;
        typedef ZH::UTIL::Cache<Effect> EffectCache;

        ZH_GRAPHICS_EXTERN template class ZH_GRAPHICS_DLL ZH::UTIL::Cache<EffectInstance>;
        typedef ZH::UTIL::Cache<EffectInstance> EffectInstanceCache;

    } // namespace Graphics
} // namespace ZH

#endif
