#pragma once

#ifdef _ZH_DLL
#define ZH_DLL _declspec(dllexport)
#else
#define ZH_DLL _declspec(dllimport)
#endif

namespace ZH
{
    class BilateralFilter;
    class PixelInfo;
    class ImageUtility;
    class FilterVideo;
    class Frame;
    class ImageUtility;
    class FrameForResample;
    class ZHLog;

}