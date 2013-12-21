#pragma once

#include<stdlib.h>
#include<time.h>
#include<complex>                              // complex<float> 
using namespace std; 
typedef complex<float> Comp;                   // 复数类型定义 

const float _2PI_ = 2.0f * 3.14159265f;        // 常数2PI定义 


/*----*----*----*----*----*----*----*----*----*----*----*----* 
FFT算法模块接口定义 
*----*----*----*----*----*----*----*----*----*----*----*----*/ 

/////////////////////////////////////////// 
// Function name : BitReverse 
// Description : 二进制倒序操作 
// Return type : int 
// Argument : int src 待倒续的数 
// Argument : int size 二进制位数 

namespace zhanghui
{
class MyMath{

public:
	MyMath()
	{  
	srand((unsigned)time(NULL));
	LengthOfGaussTable=0;	
	FFTPmax=MAX_N;

	};
	
	float randGauss();
    void  initRandGauss(int len=500,float to=1);

    int randImpulse(float propHigh = 0.3f,float propLow = 0.3f,int orign=100);
    
    int GetFFTPMax(); 

    int  DCT_2D (float * data, int  line , int M , int flag ); 





  ~MyMath()
  {if(LengthOfGaussTable!=0)
     delete []FGauss,
	 delete []fGauss;



  }


/*----*----*----*----*----*----*----*----*----*----*----*----* 
FFT算法模块接口定义 
*----*----*----*----*----*----*----*----*----*----*----*----*/ 

/////////////////////////////////////////// 
// Function name : BitReverse 
// Description : 二进制倒序操作 
// Return type : int 
// Argument : int src 待倒续的数 
// Argument : int size 二进制位数 


private:
	int BitReverse(int src, int size); 


////////////////////////////////////////////////// 
// Function name : Reorder 
// Description : 数据二进制整序 
// Return type : void 
// Argument : Comp x[MAX_N] 待整序数组 
// Argument : int N FFT点数 
// Argument : int M 点数的2的幂次 


void Reorder(Comp *x,int line, int N, int M); 


////////////////////////////////////////////////// 
// Function name : CalcW 
// Description : 计算旋转因子 
// Return type : void 
// Argument : Comp W[MAX_N] 存放因子的数组 
// Argument : int N FFT的点数 
// Argument : int flag 正逆变换标记 


void CalcW(Comp *W,int line, int N, int flag); 


///////////////////////////////////////////////// 
// Function name : FFT_1D_Kernel 
// Description : FFT算法核心 
// Return type : void 
// Argument : Comp* x 数据 
// Argument : int M 幂次 
// Argument : int flag 正逆变换标记 

public:
void FFT_1D(Comp* x,int line, int M, int flag); 


////////////////////////////////////////////////////// 
// Function name : FFT_2D_Kernel 
// Description : 2D FFT核心算法 
// Return type : void 
// Argument : Comp x[MAX_N][MAX_N] 二维数据 
// Argument : int M 幂次 
// Argument : int flag 正逆变换标记 

void FFT_2D(Comp *x,int line, int M, int flag); 


private:
	float *FGauss;
	float *fGauss;
    int   LengthOfGaussTable;
	int   FFTPmax;
	int   MAX_N ;  
	                      // 最大DFT点数 

};


//向量类
class Vector3f
{
public:
Vector3f();
Vector3f(float inX,float inY,float inZ)	{x=inX;	y=inY;	z=inZ;}
Vector3f(Vector3f& inVec)				{x=inVec.x;	y=inVec.y;	z=inVec.z;}

~Vector3f(){}


void operator=(Vector3f& inVec)		{x=inVec.x;	y=inVec.y;	z=inVec.z;}
bool operator==(Vector3f& inVec)	{if(abs(x-inVec.x)>1e-6||abs(y-inVec.y)>1e-6||abs(z-inVec.z)>1e-6) 
																return false;
															else
																return true;
															}

bool operator!=(Vector3f& inVec)	{if(abs(x-inVec.x)>1e-6||abs(y-inVec.y)>1e-6||abs(z-inVec.z)>1e-6) 
																return true;
															else
																return false;
															}
//向量加法
Vector3f operator+(Vector3f& inVec){return Vector3f(x+inVec.x,y+inVec.y,z+inVec.z);}

//向量点积
float operator*(Vector3f& inVec){return x*inVec.x+y*inVec.y+z*inVec.z;}	
//向量叉积
Vector3f operator&(Vector3f& inVec){return Vector3f(y*inVec.z-z*inVec.y,z*inVec.x-x*inVec.z,x*inVec.y-y*inVec.x);}
//归一化
Vector3f normalize(){float length = sqrt(x*x+y*y+z*z);return Vector3f(x/length,y/length,z/length);}
Vector3f operator* (float v){return Vector3f(v*x,v*y,v*z);}
Vector3f operator/(float v){return Vector3f(x/v,y/v,z/v);}	


public:
	float x;
	float y;
	float z;


};

class Vector4f
{
public:
	Vector4f();
	Vector4f(float inX,float inY,float inZ,float inW){x=inX;y=inY;z=inZ;w=inW;}
	void operator=(Vector4f& inVec){x=inVec.x;y=inVec.y;z=inVec.z;w=inVec.w;}
	~Vector4f(){}

float x;
float y;
float z;
float w;

};


}