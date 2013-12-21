#pragma once

#include<stdlib.h>
#include<time.h>
#include<complex>                              // complex<float> 
using namespace std; 
typedef complex<float> Comp;                   // �������Ͷ��� 

const float _2PI_ = 2.0f * 3.14159265f;        // ����2PI���� 


/*----*----*----*----*----*----*----*----*----*----*----*----* 
FFT�㷨ģ��ӿڶ��� 
*----*----*----*----*----*----*----*----*----*----*----*----*/ 

/////////////////////////////////////////// 
// Function name : BitReverse 
// Description : �����Ƶ������ 
// Return type : int 
// Argument : int src ���������� 
// Argument : int size ������λ�� 

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
FFT�㷨ģ��ӿڶ��� 
*----*----*----*----*----*----*----*----*----*----*----*----*/ 

/////////////////////////////////////////// 
// Function name : BitReverse 
// Description : �����Ƶ������ 
// Return type : int 
// Argument : int src ���������� 
// Argument : int size ������λ�� 


private:
	int BitReverse(int src, int size); 


////////////////////////////////////////////////// 
// Function name : Reorder 
// Description : ���ݶ��������� 
// Return type : void 
// Argument : Comp x[MAX_N] ���������� 
// Argument : int N FFT���� 
// Argument : int M ������2���ݴ� 


void Reorder(Comp *x,int line, int N, int M); 


////////////////////////////////////////////////// 
// Function name : CalcW 
// Description : ������ת���� 
// Return type : void 
// Argument : Comp W[MAX_N] ������ӵ����� 
// Argument : int N FFT�ĵ��� 
// Argument : int flag ����任��� 


void CalcW(Comp *W,int line, int N, int flag); 


///////////////////////////////////////////////// 
// Function name : FFT_1D_Kernel 
// Description : FFT�㷨���� 
// Return type : void 
// Argument : Comp* x ���� 
// Argument : int M �ݴ� 
// Argument : int flag ����任��� 

public:
void FFT_1D(Comp* x,int line, int M, int flag); 


////////////////////////////////////////////////////// 
// Function name : FFT_2D_Kernel 
// Description : 2D FFT�����㷨 
// Return type : void 
// Argument : Comp x[MAX_N][MAX_N] ��ά���� 
// Argument : int M �ݴ� 
// Argument : int flag ����任��� 

void FFT_2D(Comp *x,int line, int M, int flag); 


private:
	float *FGauss;
	float *fGauss;
    int   LengthOfGaussTable;
	int   FFTPmax;
	int   MAX_N ;  
	                      // ���DFT���� 

};


//������
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
//�����ӷ�
Vector3f operator+(Vector3f& inVec){return Vector3f(x+inVec.x,y+inVec.y,z+inVec.z);}

//�������
float operator*(Vector3f& inVec){return x*inVec.x+y*inVec.y+z*inVec.z;}	
//�������
Vector3f operator&(Vector3f& inVec){return Vector3f(y*inVec.z-z*inVec.y,z*inVec.x-x*inVec.z,x*inVec.y-y*inVec.x);}
//��һ��
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