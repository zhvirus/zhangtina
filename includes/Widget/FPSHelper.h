#ifndef FPSHELPER_H
#define FPSHELPER_H

#include "Common/ZHSTD.h"

namespace ZH {
    namespace Widgets {

        class ZH_WIDGET_DLL FPSHelper
        {
        public:
            FPSHelper() = default;
            ~FPSHelper() = default;

            void updateByCurTime(unsigned int milliSec);
            float getCurFPS() const { return m_currentFPS; }
            void printFPS(unsigned int milliSec);

        private:
            static const unsigned int sFrameCountForOneFPSCal = 10;
            unsigned int m_accFrameCount{ 0 };
            unsigned int m_accTimeInMilliSecond{ 0 };
            unsigned int m_prevTimeStamp{ 0 };
            float m_currentFPS{ 0.0f };

            unsigned int m_lastPrintMilliSecond{ 0 };
        };

    }
}


#endif

