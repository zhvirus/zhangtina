#ifndef _ZH_WIGET_DLL_
#define _ZH_WIGET_DLL_
#endif

#include "Widget/FPSHelper.h"
#include "Internal/Common/Internal_common.h"

namespace ZH {
    namespace Widgets {

        void FPSHelper::updateByCurTime(unsigned int milliSec)
        {
            if (m_accFrameCount == sFrameCountForOneFPSCal) {
                if (m_accTimeInMilliSecond == 0) {
                    m_currentFPS = 0.0f;
                }
                else {
                    m_currentFPS = sFrameCountForOneFPSCal*1000 / (float)m_accTimeInMilliSecond;
                }

                m_accFrameCount = 0;
                m_accTimeInMilliSecond = 0;
            }

            if (m_prevTimeStamp == 0) {
                m_prevTimeStamp = milliSec;
            }
            else {
                m_accFrameCount++;
                m_accTimeInMilliSecond += (milliSec - m_prevTimeStamp);
                m_prevTimeStamp = milliSec;
            }
        }

        void FPSHelper::printFPS(unsigned int milliSec)
        {
            if (milliSec - m_lastPrintMilliSecond > 1000) {
                m_lastPrintMilliSecond = milliSec;
                printf("FPS = %.1f\n", m_currentFPS);
            }
        }

    } // end namespace 'Widget'
} // end namespace 'ZH'


