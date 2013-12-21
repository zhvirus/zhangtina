#pragma once

#ifdef _ZH_MATH_DLL_
#define ZH_MATH_DLL _declspec(dllexport)
#else
#define ZH_MATH_DLL _declspec(dllimport)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <limits>
#include <assert.h>

typedef float Real; 