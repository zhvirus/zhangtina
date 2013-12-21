#pragma once

#include "ZHRenderDecl.h"


//--------------------------------------------------------------------------------------
//回调函数类型定义
//--------------------------------------------------------------------------------------
namespace ZH
{
typedef void		(*	LPRESHAPE)(ZHint,ZHint);//改变窗口大小
typedef void		(*	LPKEYBOARD)(unsigned char,int, int);//键盘输入
typedef void		(*	LPSPECIALKEYBOARD)(ZHint,int ,int);//12个特殊键按下
typedef void		(* LPMOUSE)(ZHint,ZHint,ZHint,ZHint);//鼠标左右键、中间键按下或者放开，六个事件
typedef void		(*	LPMOTION)(ZHint,ZHint);	//mouse move with at leaset one mouse button is pressed
typedef void		(*	LPPASSIVEMOTION)(ZHint,ZHint);//mouse move without any mouse button is pressed
typedef void		(*	LPRENDER)();


//=======================================================
//类名: ZHCALLBACKS
//描述: 回调函数注册
//=======================================================


class ZH_DLL ZHCALLBACKS
{
public:
	ZHCALLBACKS()
	{	
		render	=	NULL;
		reshape	=	NULL;
		keyboard	=	NULL;
		specialkeyboard	=	NULL;
		mouse	=	NULL;
		motion	=	NULL;
		passiveMotion	=	NULL;
	}
	~ZHCALLBACKS()
	{
		render	=NULL;
		reshape	=	NULL;
		keyboard	=	NULL;
		specialkeyboard	=	NULL;
		mouse	=	NULL;
		motion	=	NULL;
		passiveMotion	=	NULL;
	};
public:
	
	LPRESHAPE					reshape;				//改变窗口大小
	LPKEYBOARD					keyboard;				//键盘
	LPSPECIALKEYBOARD		specialkeyboard;	//特殊键
	LPMOUSE						mouse;					//鼠标
	LPMOTION						motion;					//按下至少一个鼠标键的鼠标移动	
	LPPASSIVEMOTION			passiveMotion;		//没有按下任何一个鼠标键的鼠标移动
	LPRENDER						render;

};


//--------------------------------------------------------------------------------------
//Call backs
//--------------------------------------------------------------------------------------

void ZH_DLL ZHSetKeyboardFunc(LPKEYBOARD keyboard);
void ZH_DLL ZHSetSpecialFunc(LPSPECIALKEYBOARD special);
void ZH_DLL ZHSetReshapeFunc(LPRESHAPE reshape);
void ZH_DLL ZHSetMouseFunc(LPMOUSE mouse);
void ZH_DLL ZHSetMotionFunc(LPMOTION motion);
void ZH_DLL ZHSetPassiveMotionFunc(LPPASSIVEMOTION passiveMotion);
void ZH_DLL ZHSetDisplayFunc(LPRENDER	render);
}