// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <tchar.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../common_src/gl_common.h"
#include "GL/glew.h"
#include <GL/freeglut.h>
#include "Camera.h"
#define DEBUG_GL_ERRORS TEST_COM::peek_gl_errors(__LINE__);

// TODO: reference additional headers your program requires here
