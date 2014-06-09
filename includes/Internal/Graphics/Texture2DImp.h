#ifndef TEXTURE2D_IMP_H
#define TEXTURE2D_IMP_H

#include "Common/ZHSTD.h"
#include "Internal/Graphics/Internal_common_graphics.h"

namespace ZH{
    namespace Graphics{
        class Texture2DImp{
        public:
            Texture2DImp( ID3D11Texture2D* );
            ~Texture2DImp();

            bool isValid();

            ID3D11Texture2D* getTex2D_d3d() { return m_pTexture2D_d3d; }

        private:
            ID3D11Texture2D* m_pTexture2D_d3d;

        };
    } // Graphics
} // ZH



#endif
