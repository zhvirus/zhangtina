#pragma once

#ifdef _ZH_DLL
#define ZH_DLL _declspec(dllexport)
#else
#define ZH_DLL _declspec(dllimport)
#endif

#include "Image.h"
#include "PixelInfo.h"
#include "Filter.h"
#include "Singleton.h"
#include "FilterVideo.h"
#include "Watch.h"
#include "Log.h"

