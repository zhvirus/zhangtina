#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHCallBacks.h"
#include "ZHWindow.h"

namespace ZH
{

void ZHSetDisplayFunc(LPRENDER	render)
{
	(Window::m_callbacks).render		=	render;

}

void ZHSetKeyboardFunc(LPKEYBOARD keyboard)
{
	(Window::m_callbacks).keyboard		=	keyboard;
}

void ZHSetSpecialFunc(LPSPECIALKEYBOARD special)
{
	(Window::m_callbacks).specialkeyboard = special;
}

void ZHSetReshapeFunc(LPRESHAPE reshape)
{
	(Window::m_callbacks).reshape = reshape;
}

void ZHSetMouseFunc(LPMOUSE mouse)
{
	(Window::m_callbacks).mouse = mouse;
}

void ZHSetMotionFunc(LPMOTION motion)
{
	(Window::m_callbacks).motion	=	motion;

}

void ZHSetPassiveMotionFunc(LPPASSIVEMOTION passiveMotion)
{
	(Window::m_callbacks).passiveMotion	=	passiveMotion;

}



















}