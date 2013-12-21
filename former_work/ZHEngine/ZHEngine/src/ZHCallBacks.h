#pragma once

#include "ZHRenderDecl.h"


//--------------------------------------------------------------------------------------
//�ص��������Ͷ���
//--------------------------------------------------------------------------------------
namespace ZH
{
typedef void		(*	LPRESHAPE)(ZHint,ZHint);//�ı䴰�ڴ�С
typedef void		(*	LPKEYBOARD)(unsigned char,int, int);//��������
typedef void		(*	LPSPECIALKEYBOARD)(ZHint,int ,int);//12�����������
typedef void		(* LPMOUSE)(ZHint,ZHint,ZHint,ZHint);//������Ҽ����м�����»��߷ſ��������¼�
typedef void		(*	LPMOTION)(ZHint,ZHint);	//mouse move with at leaset one mouse button is pressed
typedef void		(*	LPPASSIVEMOTION)(ZHint,ZHint);//mouse move without any mouse button is pressed
typedef void		(*	LPRENDER)();


//=======================================================
//����: ZHCALLBACKS
//����: �ص�����ע��
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
	
	LPRESHAPE					reshape;				//�ı䴰�ڴ�С
	LPKEYBOARD					keyboard;				//����
	LPSPECIALKEYBOARD		specialkeyboard;	//�����
	LPMOUSE						mouse;					//���
	LPMOTION						motion;					//��������һ������������ƶ�	
	LPPASSIVEMOTION			passiveMotion;		//û�а����κ�һ������������ƶ�
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