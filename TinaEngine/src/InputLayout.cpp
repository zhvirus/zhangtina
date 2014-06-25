#ifndef _ZH_GRAPHICS_DLL_
#define _ZH_GRAPHICS_DLL_
#endif
#include "Graphics/InputLayout.h"
#include "Internal/Graphics/Internal_common_graphics.h"
#include "Internal/Graphics/ShaderLibrary.h"
#include "Internal/Graphics/DataToD3D.h"
#include "Graphics/DeviceDX11.h"

namespace ZH{
    namespace Graphics{

        InputLayout::InputLayout()
        {
            m_pSlotInputs = new std::vector<Semantic*>;
        }

        InputLayout::~InputLayout()
        {
            reset();
            if ( m_pSlotInputs){
                delete m_pSlotInputs;
                m_pSlotInputs = NULL;
            }
        }

        void InputLayout::reset()
        {
            if ( m_pSlotInputs ){
                std::vector<Semantic*>::iterator it = m_pSlotInputs->begin();
                for(; it != m_pSlotInputs->end(); ++it){
                    delete *it;
                }
                m_pSlotInputs->clear();
            }
        }


    }

}
