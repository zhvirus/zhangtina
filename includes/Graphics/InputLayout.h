#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include "Common/ZHSTD.h"
#include <vector>

namespace ZH{
    namespace Graphics{

        class Semantic;
        class ShaderCodes;

        class ZH_GRAPHICS_DLL InputLayout{
        public:
            InputLayout();
            ~InputLayout();

            std::vector<Semantic*>&       getSlotInputVec()       { return *m_pSlotInputs;}
            const std::vector<Semantic*>& getSlotInputVec() const { return *m_pSlotInputs;}

        private:
            void reset();
            std::vector<Semantic*>* m_pSlotInputs;
            void* pLayoutImp;

            friend class DeviceDX11;

        };




    }
}




#endif
