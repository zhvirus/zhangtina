

#include<stdio.h>
#include<iostream>
#include<math.h>
#include"mymath.h"

using namespace std;
using namespace zhanghui;

float MyMath::randGauss()
{ 
	
  float randValue=0;  //���ز�����˹�ֲ������
  float equalRandTemp=0; //[0...1)���ȷֲ�α�����
  float step=6.0f/LengthOfGaussTable;


  equalRandTemp=(double)rand()/(double)(RAND_MAX+1);//[0...1)���ȷֲ�α���������

    for(int i=1;i<LengthOfGaussTable;i++)
  {
	  if(equalRandTemp<=FGauss[i]&&equalRandTemp>=FGauss[i-1]){
        randValue=-3.0f+(i-1)*step+(equalRandTemp-FGauss[i-1])/(FGauss[i]-FGauss[i-1])*step;  
           //���Բ�ֵ������˹�ֲ�α�����
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

    float dx=6.0f/len;    //���� 
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


//������������  ����  0  255  ���� ԭ�Ҷ�ֵ
int MyMath::randImpulse(float propHigh ,float propLow ,int orign)
{

if (propHigh<0||propHigh>1||propLow<0||propLow>1||(propHigh+propLow)>1)
   return -1;


double equalRandTemp = (double)rand()/(double)(RAND_MAX+1);//[0...1)���ȷֲ�α���������

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
FFT�㷨ģ��ӿڶ��� 
*----*----*----*----*----*----*----*----*----*----*----*----*/ 

/////////////////////////////////////////// 
// Function name : BitReverse 
// Description : �����Ƶ������ 
// Return type : int 
// Argument : int src ���������� 
// Argument : int size ������λ�� 
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
// Description : ���ݶ��������� 
// Return type : void 
// Argument : Comp x[MAX_N] ���������� 
// Argument : int N FFT���� 
// Argument : int M ������2���ݴ� 
void MyMath::Reorder(Comp *x,int line, int N, int M) 
{ 
Comp *new_x = new Comp[line]; 

for (int i=0; i<N; i++) 
{int temp = BitReverse(i, M);
new_x[i] = x[temp]; 
}
// ���´���ԭ������(�Ѿ��Ƕ�����������˵�����) 
for (int i=0; i<N; i++) 
x[i] = new_x[i]; 

delete []new_x;
} 

////////////////////////////////////////////////// 
// Function name : CalcW 
// Description : ������ת���� 
// Return type : void 
// Argument : Comp W[MAX_N] ������ӵ����� 
// Argument : int N FFT�ĵ��� 
// Argument : int flag ����任��� 
void MyMath::CalcW(Comp *W,int line, int N, int flag) 
{ 
	
	if (flag == 1)
	{
		for (int i=0; i<N; i++) 
       { 
        
          W[i] = exp(Comp(0, -_2PI_ *i/N)); // ��FFT�任 
       
        } 

	}

	
	else 

		for (int i=0; i<N; i++) 
       { 

          W[i] = exp(Comp(0, _2PI_*i/N)); // ��FFT�任 
	   }  

} 

///////////////////////////////////////////////// 
// Function name : FFT_1D_Kernel 
// Description : FFT�㷨���� 
// Return type : void 
// Argument : Comp* x ���� 
// Argument : int M �ݴ� 
// Argument : int flag ����任��� 
//���±�Ӧ���Լ���ɡ� 

void MyMath::FFT_1D(Comp* x,int line, int M, int flag) 
{ 
int N = (1 << M); 

// ���������� 
Reorder(x,line, N, M); 

// ��ת���Ӽ��� 
Comp *W =new Comp[line]; 
CalcW(W,line, N, flag); 

// ����Ⱥ�� 
int GroupNum = N/2; // ��һ����Ⱥ��ΪN/2 

// Ⱥ�ڵ��ε�Ԫ�� 
int CellNum = 1; // ��һ����Ⱥ�ڵ��ε�Ԫ��Ϊ1 

// ������� 
for (int i=0; i<M; i++) 
{ 
          // �����Ⱥ 
          for (int j=0; j<GroupNum; j++) 
          { 
                // ��������ε�Ԫ 
                for (int k=0; k<CellNum; k++) 
                 { 
                       // (1) �������ǰ���ε�Ԫ����Ԫ�������������е�λ�� 

                         // ��һԪ��λ�� 
                          int Pos1 = CellNum * j * 2 + k ; // �Ѿ�������ǰ j Ⱥ��ÿȺ�� CellNum ����Ԫ�� 
                         //ÿ��Ԫ�� 2 ��Ԫ�� 
                        // �ڶ�Ԫ��λ�� 
                          int Pos2 = Pos1 + CellNum; 

                          // (2) ������ת�����뵥Ԫ�ĵڶ�Ԫ�صĸ����˻� 
                          Comp TMP = x[Pos2] * W[k * GroupNum] ; 

                         // (3) �������ս��, �����뵽�����ԭ��λ�� 
                          x[Pos2] = x[Pos1] - TMP ; 
                          x[Pos1] = x[Pos1] + TMP ; 
                   } 

		  } 


GroupNum >>= 1; // ��������, ����Ӧ��Ⱥ������һ�� 
CellNum <<= 1; // ��������, ����Ӧ��Ⱥ�ڵ�Ԫ������һ�� 
} 

// �����IFFT����Ԫ�ػ�Ҫ�ٳ���N 
  if (flag != 1) 
  { 
     for (int i=0; i<N; i++) 
       x[i]/=N; 
  } 

delete []W;
} 

////////////////////////////////////////////////////// 
// Function name : FFT_2D_Kernel 
// Description : 2D FFT�����㷨 
// Return type : void 
// Argument : Comp x[MAX_N][MAX_N] ��ά���� 
// Argument : int M �ݴ� 
// Argument : int flag ����任��� 




void MyMath::FFT_2D(Comp *x, int line,int M, int flag) 
{ 
MAX_N = line;

int N = (1 << M); 


// �����н��� 1D-FFT 
for (int i=0; i<N; i++) 
FFT_1D(x+i*line,line, M, flag); // <--- �������ٴ������x�� 

// �����н��� 1D-FFT 
Comp *col = new Comp[line];
for (int j=0; j<N; j++) 
{ 
// ȡ�õ�j�е����� 
for (int i=0; i<N; i++) 
col[i] = x[i*line+j]; 

// �Ե�j�����ݽ��� 1D-FFT 
FFT_1D(col,line, M, flag); // <--- ������������col�� 

// ������Żؾ����j���� 
for (int i=0; i<N; i++) 
*(x+i*line+j) = col[i]; 
} 


delete []col;
} 
// <--- End of  FFT


// ������� �߳�Ϊ line��2^M �ķ��������ɢ���� ����任
  int  MyMath::DCT_2D (float * data, int  line , int M , int flag )
  {
//������������
			if (line ==static_cast<int>(pow(2.0f,M)))
			{
						//������ʱ���鱣�洫������
						float *temp = new float[line*line];	
						for (int t=0;t<line*line;t++)
						{
							
							temp[t] = data[t];
							
						}
                        //ϵ��׼��
                         float *a = new float[line];
						 float tt= sqrt(2.0f/(float)line);
						 for (int i=0;i<line;i++)
							 a[i] = tt;
						 a[0]=sqrt(1.0f/(float)line);

						//2ά��ɢ���ұ任
						if (flag ==1)
						{   
							//����ÿһ����
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
						//2ά ��ɢ������任
						else
						{
                                //����ÿһ����
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
           //�����������
			else

						return false;

  }


Vector3f operator*(float v,Vector3f in){return Vector3f(v*in.x,v*in.y,v*in.z);}
Vector3f operator-(Vector3f in){return Vector3f(-in.x,-in.y,-in.z);}
//��������
Vector3f operator-(Vector3f& inVec2,Vector3f& inVec){return Vector3f(inVec2.x-inVec.x,inVec2.y-inVec.y,inVec2.z-inVec.z);}














