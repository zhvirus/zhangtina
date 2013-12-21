

#include<stdio.h>
#include<iostream>
#include<math.h>
#include"mymath.h"

using namespace std;
using namespace zhanghui;

float MyMath::randGauss()
{ 
	
  float randValue=0;  //返回产生高斯分布随机数
  float equalRandTemp=0; //[0...1)均匀分布伪随机数
  float step=6.0f/LengthOfGaussTable;


  equalRandTemp=(double)rand()/(double)(RAND_MAX+1);//[0...1)均匀分布伪随机数产生

    for(int i=1;i<LengthOfGaussTable;i++)
  {
	  if(equalRandTemp<=FGauss[i]&&equalRandTemp>=FGauss[i-1]){
        randValue=-3.0f+(i-1)*step+(equalRandTemp-FGauss[i-1])/(FGauss[i]-FGauss[i-1])*step;  
           //线性插值产生高斯分布伪随机数
      break;
        
	  }




  }









return randValue;

}

void MyMath::initRandGauss(int len,float to)
{  
	LengthOfGaussTable=len;
	FGauss=new float[len];
    fGauss=new float[len];

    float dx=6.0f/len;    //步长 
	float c=1.0f/sqrt(2.0f*3.1415927f)/to; 
    float x=-3.0f;
	
	fGauss[0]=c*exp(-0.5*x*x/to/to);
	FGauss[0]=0;

	for(int i=1;i<LengthOfGaussTable;i++)
	{  x+=dx;
		
		fGauss[i]=c*exp(-0.5*x*x/to/to);
		FGauss[i]=FGauss[i-1]+(fGauss[i-1]+fGauss[i])*dx/2;


	}


}


//脉冲噪声产生  返回  0  255  或者 原灰度值
int MyMath::randImpulse(float propHigh ,float propLow ,int orign)
{

if (propHigh<0||propHigh>1||propLow<0||propLow>1||(propHigh+propLow)>1)
   return -1;


double equalRandTemp = (double)rand()/(double)(RAND_MAX+1);//[0...1)均匀分布伪随机数产生

if (equalRandTemp>=0&&equalRandTemp<propHigh)
   return 255;
else if (equalRandTemp>=propHigh&&equalRandTemp<propHigh+propLow)
   return 0;
else
   return orign;


}

int MyMath::GetFFTPMax()
{

return FFTPmax;
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
int MyMath::BitReverse(int src, int size) 
{ 
int tmp = src; 
int des = 0; 
for (int i=size-1; i>=0; i--) 
{ 
des = ((tmp & 0x1) << i) | des; 
tmp = tmp >> 1; 
} 
return des; 
} 

////////////////////////////////////////////////// 
// Function name : Reorder 
// Description : 数据二进制整序 
// Return type : void 
// Argument : Comp x[MAX_N] 待整序数组 
// Argument : int N FFT点数 
// Argument : int M 点数的2的幂次 
void MyMath::Reorder(Comp *x,int line, int N, int M) 
{ 
Comp *new_x = new Comp[line]; 

for (int i=0; i<N; i++) 
{int temp = BitReverse(i, M);
new_x[i] = x[temp]; 
}
// 重新存入原数据中(已经是二进制整序过了的数据) 
for (int i=0; i<N; i++) 
x[i] = new_x[i]; 

delete []new_x;
} 

////////////////////////////////////////////////// 
// Function name : CalcW 
// Description : 计算旋转因子 
// Return type : void 
// Argument : Comp W[MAX_N] 存放因子的数组 
// Argument : int N FFT的点数 
// Argument : int flag 正逆变换标记 
void MyMath::CalcW(Comp *W,int line, int N, int flag) 
{ 
	
	if (flag == 1)
	{
		for (int i=0; i<N; i++) 
       { 
        
          W[i] = exp(Comp(0, -_2PI_ *i/N)); // 正FFT变换 
       
        } 

	}

	
	else 

		for (int i=0; i<N; i++) 
       { 

          W[i] = exp(Comp(0, _2PI_*i/N)); // 逆FFT变换 
	   }  

} 

///////////////////////////////////////////////// 
// Function name : FFT_1D_Kernel 
// Description : FFT算法核心 
// Return type : void 
// Argument : Comp* x 数据 
// Argument : int M 幂次 
// Argument : int flag 正逆变换标记 
//以下本应由自己完成。 

void MyMath::FFT_1D(Comp* x,int line, int M, int flag) 
{ 
int N = (1 << M); 

// 二进制整序 
Reorder(x,line, N, M); 

// 旋转因子计算 
Comp *W =new Comp[line]; 
CalcW(W,line, N, flag); 

// 级内群数 
int GroupNum = N/2; // 第一级的群数为N/2 

// 群内蝶形单元数 
int CellNum = 1; // 第一级的群内蝶形单元数为1 

// 处理各级 
for (int i=0; i<M; i++) 
{ 
          // 处理各群 
          for (int j=0; j<GroupNum; j++) 
          { 
                // 处理各蝶形单元 
                for (int k=0; k<CellNum; k++) 
                 { 
                       // (1) 计算出当前蝶形单元所含元素在数据数组中的位置 

                         // 第一元素位置 
                          int Pos1 = CellNum * j * 2 + k ; // 已经处理了前 j 群，每群有 CellNum 个单元， 
                         //每单元有 2 个元素 
                        // 第二元素位置 
                          int Pos2 = Pos1 + CellNum; 

                          // (2) 计算旋转因子与单元的第二元素的复数乘积 
                          Comp TMP = x[Pos2] * W[k * GroupNum] ; 

                         // (3) 计算最终结果, 并存入到数组的原先位置 
                          x[Pos2] = x[Pos1] - TMP ; 
                          x[Pos1] = x[Pos1] + TMP ; 
                   } 

		  } 


GroupNum >>= 1; // 级别增加, 则相应的群数减少一半 
CellNum <<= 1; // 级别增加, 则相应的群内单元数增加一倍 
} 

// 如果是IFFT，各元素还要再除以N 
  if (flag != 1) 
  { 
     for (int i=0; i<N; i++) 
       x[i]/=N; 
  } 

delete []W;
} 

////////////////////////////////////////////////////// 
// Function name : FFT_2D_Kernel 
// Description : 2D FFT核心算法 
// Return type : void 
// Argument : Comp x[MAX_N][MAX_N] 二维数据 
// Argument : int M 幂次 
// Argument : int flag 正逆变换标记 




void MyMath::FFT_2D(Comp *x, int line,int M, int flag) 
{ 
MAX_N = line;

int N = (1 << M); 


// 先逐行进行 1D-FFT 
for (int i=0; i<N; i++) 
FFT_1D(x+i*line,line, M, flag); // <--- 计算结果再存入矩阵x中 

// 再逐列进行 1D-FFT 
Comp *col = new Comp[line];
for (int j=0; j<N; j++) 
{ 
// 取得第j列的数据 
for (int i=0; i<N; i++) 
col[i] = x[i*line+j]; 

// 对第j列数据进行 1D-FFT 
FFT_1D(col,line, M, flag); // <--- 计算结果在数组col中 

// 将结果放回矩阵第j列中 
for (int i=0; i<N; i++) 
*(x+i*line+j) = col[i]; 
} 


delete []col;
} 
// <--- End of  FFT


// 对输入的 边长为 line＝2^M 的方阵进行离散余弦 正逆变换
  int  MyMath::DCT_2D (float * data, int  line , int M , int flag )
  {
//参数输入无误
			if (line ==static_cast<int>(pow(2.0f,M)))
			{
						//创建临时数组保存传进数据
						float *temp = new float[line*line];	
						for (int t=0;t<line*line;t++)
						{
							
							temp[t] = data[t];
							
						}
                        //系数准备
                         float *a = new float[line];
						 float tt= sqrt(2.0f/(float)line);
						 for (int i=0;i<line;i++)
							 a[i] = tt;
						 a[0]=sqrt(1.0f/(float)line);

						//2维离散余弦变换
						if (flag ==1)
						{   
							//遍历每一个点
						      for(int m=0;m<line ;m++)
								  for(int n=0;n<line ;n++)
								  {     
									    int inTemp=0;
                                        
										 for(int i=0;i<line;i++)
											 for(int k=0;k<line;k++)
											 {  
												 float pp = 3.1415927f*(2*i+1)*m/2.0f/line;
										         float qq = 3.1415927f*(2*k+1)*n/2.0f/line;
												 inTemp += *(temp+i*line+k)*cos(pp)*cos(qq);
    
                                                  
											 }
								  inTemp*=a[m]*a[n];
								  *(data+m*line+n) = inTemp;
								  
								  
								  }

						}
						//2维 离散余弦逆变换
						else
						{
                                //遍历每一个点
						      for(int i=0;i<line ;i++)
								  for(int k=0;k<line ;k++)
								  {     
									    int inTemp=0;
                                        
										 for(int m=0;m<line;m++)
											 for(int n=0;n<line;n++)
											 {  
												 float pp = 3.1415927f*(2*i+1)*m/2.0f/line;
										         float qq = 3.1415927f*(2*k+1)*n/2.0f/line;
												 inTemp += *(temp+m*line+n)*cos(pp)*cos(qq)*a[m]*a[n];
    
                                                  
											 }
								  
								  *(data+i*line+k) = inTemp;
								  
								  
								  } 


						}
                        delete  []a;
						delete  []temp;
						return true;
			}
           //输入参数有误
			else

						return false;

  }


Vector3f operator*(float v,Vector3f in){return Vector3f(v*in.x,v*in.y,v*in.z);}
Vector3f operator-(Vector3f in){return Vector3f(-in.x,-in.y,-in.z);}
//向量减法
Vector3f operator-(Vector3f& inVec2,Vector3f& inVec){return Vector3f(inVec2.x-inVec.x,inVec2.y-inVec.y,inVec2.z-inVec.z);}














