
//=================================================================
//Remark:include basic header file and class declear
//
//
//=================================================================

#pragma once

#include "ZHRenderSTD.h"
#include "ZHResult.h"
#include "ZHSingleton.h"

//=============================
//class pre-decleration
//=============================
namespace ZH
{

class RenderSystem;
class Light;
class Color;
class Camera;
class ZHCALLBACKS;
class Font;
class SceneManager;
class VideoMode;
class VideoModeList;
class ZHDisplayAdapter;
class FPS;
class Renderable;
class RenderData;
class SubMesh;
class VertexBuffer;
class IndexBuffer;
class Window;
class ZHRect;
class Mesh;
class BufferManager;
class Resource;
class Image;
class Terrain;
class TextureManager;
class Pass;
class Camera;
template<class T>
class RES_UNIT;

//--------------------------------------------------------------------------------------
//Type redefine
//--------------------------------------------------------------------------------------
typedef std::string String;
typedef std::vector<String> StringVector;

typedef char ZHchar;
typedef short ZHshort;
typedef int ZHint;
typedef float ZHfloat;
typedef long ZHlong;
typedef bool ZHbool;


const float ZHFLOATMAX=FLT_MAX;
const float ZHFLOATMIN=0.001f;

class ZHRect
{
public:
	ZHRect(){}
	ZHRect(ZHfloat _x,ZHfloat _y,ZHfloat _w,ZHfloat _h):
	x(_x),
	y(_y),
	width(_w),
	height(_h)
	{}
	ZHRect(ZHRect& inRect){memcpy(this,&inRect,sizeof(ZHRect));}
	void operator=(const ZHRect& inRect){memcpy(this,&inRect,sizeof(ZHRect));}
	ZHfloat x,y,width,height;

};






};


//delete 堆内存
template<typename T>
inline void SAFE_DELETE(T p)		{if(p){delete p;p=0;}}

//delete 堆内存数组
template<typename T>
inline void SAFE_DELETE_ARRAY(T p){if(p){delete []p;p=0;}}

//释放对象资源
template<typename T>
inline void SAFE_RELEASE(T p){if(p){p->Release();}}

inline void ZH_OUT(LPCTSTR message){std::cout<<message<<std::endl;}
//出错提示
inline void ZH_ERR(LPCTSTR message)
{
	std::cout<<"Error:   ";
	ZH_OUT(message);
	//MessageBox(0,TEXT("Error"),message,1);
	//PostQuitMessage(0);
}

//--------------------------------------------------------------------------------------
//特殊键的定义
//--------------------------------------------------------------------------------------

const int ZH_KEY_F1=0x0001;
const int ZH_KEY_F2 =0x0002;
const int ZH_KEY_F3=0x0003;
const int ZH_KEY_F4=0x0004;
const int ZH_KEY_F5 =0x0005;
const int ZH_KEY_F6=0x0006;
const int  ZH_KEY_F7=0x0007;
const int ZH_KEY_F8=0x0008;
const int ZH_KEY_F9=0x0009;
const int ZH_KEY_F10=0x000A;
const int ZH_KEY_F11=0x000B;
const int ZH_KEY_F12=0x000C;
const int ZH_KEY_LEFT=0x0064;
const int ZH_KEY_UP=0x0065;
const int ZH_KEY_RIGHT=0x0066;
const int ZH_KEY_DOWN=0x0067;
const int ZH_KEY_PAGE_UP=0x0068;
const int ZH_KEY_PAGE_DOWN=0x0069;
const int ZH_KEY_HOME=0x006A;
const int ZH_KEY_END=0x006B;
const int ZH_KEY_INSERT=0x006C;

//--------------------------------------------------------------------------------------
//鼠标事件状态标记
//--------------------------------------------------------------------------------------
const int  ZH_LEFT_BUTTON=0x0000;
const int  ZH_MIDDLE_BUTTON=0x0001;
const int  ZH_RIGHT_BUTTON=0x0002;

const int  ZH_DOWN=0x0000;	//鼠标键按下
const int  ZH_UP=0x0001;		//鼠标键未按下
const int  ZH_LEFT=0x0000;
const int  ZH_ENTERED=0x0001;