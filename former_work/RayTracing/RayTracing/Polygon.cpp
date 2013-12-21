#include "StdAfx.h"
#include	"Model.h"
#include "Polygon.h"

using namespace Raytracing;

Polygon::Polygon(void)
{
}

//**************************************************
//��������		���캯��
//����˵����	vSet����Ķ���ζ��㼯
//					inMaterial�������β���
//					inRafractRate��������������
//					isTrans������Ƿ�͸��
//**************************************************

Polygon::Polygon(const vector<D3DXVECTOR3>& vSet,const D3DXVECTOR3& inMaterial,float inRefractRate,bool isTrans,bool isReflect)
{
	vertexSet	=	vSet;					//���㼯
	material	=	inMaterial;			//����
	refractRate	=	inRefractRate;	//������
	isTransparency	=	isTrans;		//�Ƿ�͸��
	isReflectAble	=	isReflect;			//�Ƿ�ɷ���
	modelType	=	POLYGON;		//��������
	ifHasTexture		=	false;			//û������
	texture		=	NULL;					//����ͼƬ��Դ��ʼ��Ϊ��
	
	D3DXVECTOR3 vec10	=	vSet[0]-vSet[1];
	D3DXVECTOR3 vec12	=	vSet[2]-vSet[1];
	D3DXVECTOR3 nor	=	*D3DXVec3Cross(&nor,&vec12,&vec10);
	D3DXVec3Normalize(&nor,&nor);
	normal	=	nor;							//������

	float xN	=	nor.x;
	float yN	=	nor.y;
	float zN	=	nor.z;
	
	float x0	=	vSet[0].x;
	float y0	=	vSet[0].y;
	float z0	=	vSet[0].z;
  
	//planeEquPara   ƽ�淽��A B C D
	planeEquPara.x	=	xN;
	planeEquPara.y	=	yN;
	planeEquPara.z	=	zN;
	planeEquPara.w	=	-(xN*x0+yN*y0+zN*z0);

	//����ͼ��Ϊ��
	setTexFileName(L"NULL");
}


Polygon::~Polygon(void)
{
	//����ͼƬ��Դ
	if(texture!=NULL)
	{
		delete texture;
		texture=NULL;	
	}
}
//���ض���ζ�����Ŀ
int Polygon::getVertexNum()
{
	
	return static_cast<int>(vertexSet.size());

}


//����ͼƬ��Դ
bool Polygon::setTexture(Bitmap *in)
{

//����ͼƬ��Ч
if(in!=NULL)
{
	//����ͼƬ��Դ
	if(texture!=NULL)
	{
		delete texture;
		texture=NULL;	
		setHasTexture(false);
	}

texture	=	in;
setHasTexture(true);
width	=	in->GetWidth();
height	=	in->GetHeight();
return true;
}

return false;


}




//*************************************************************
	//��������		getReflectPoint
	//���ܣ�		��ռ����ߺͼ������  ���佻��
	//����˵����	startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					
	//����ֵ��		�������ߺͶ���ν��㣬�����ཻ����(50000,50000,50000)
	//*************************************************************

D3DXVECTOR3 Polygon::getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction)
{		
		
		//������ʼ��
		float x0	=	startPoint.x;
		float y0	=	startPoint.y;
		float z0	=	startPoint.z;
		//���߷���ʸ��
		float a	=	direction.x;
		float b	=	direction.y;
		float c	=	direction.z;
		//ƽ�淽�̲���
		float A	=	planeEquPara.x;
		float B	=	planeEquPara.y;
		float C	=	planeEquPara.z;
		float D	=	planeEquPara.w;

		//���ߺ���ƽ�л���������
		float value	=	A*a+B*b+C*c;
		if(abs(value)<1e-4)
			return D3DXVECTOR3(50000,50000,50000);
		
		//ֱ��ƽ�潻��Ĳ���t
		float t	=	-(D+A*x0+B*y0+C*z0)/value;
		
		//û�к�ƽ���ཻ ֱ�ӷ���
		if(t<=1e-4)
			return D3DXVECTOR3(50000,50000,50000);
		
		//ֱ��ƽ�潻��
		float pointX	=	x0 +	a*t;
		float pointY	=	y0	+	b*t;
		float pointZ	=	z0	+	c*t;
		
		//��⽻���Ƿ��ڶ�����ڲ�
		int intersectNum	=	0;
		int vNum	=	getVertexNum();
		
		//�ҳ������������� ����ͶӰ
		float MMAX	=	abs(A)>abs(B)?abs(A):abs(B);
		MMAX	=	MMAX>abs(C)?MMAX:abs(C);
		
		//ͶӰ��YOZƽ��
		if(MMAX==abs(A))
		{
			for(int i=0;i<vNum;i++)
			{
				if (isIntersect(	vertexSet[i%vNum].y,vertexSet[i%vNum].z,
									vertexSet[(i+1)%vNum].y,vertexSet[(i+1)%vNum].z,
									pointY,pointZ,0.3346f,0.6433f))
					intersectNum++;


			}
		}

		//ͶӰ��XOZƽ��
		else if(MMAX==abs(B))
		{
			for(int i=0;i<vNum;i++)
			{
				if (isIntersect(	vertexSet[i%vNum].x,vertexSet[i%vNum].z,
									vertexSet[(i+1)%vNum].x,vertexSet[(i+1)%vNum].z,
									pointX,pointZ,0.3346f,0.6433f))
					intersectNum++;


			}



		}

		//ͶӰ��XOYƽ��
		else
		{
			for(int i=0;i<vNum;i++)
			{
				if (isIntersect(	vertexSet[i%vNum].x,vertexSet[i%vNum].y,
									vertexSet[(i+1)%vNum].x,vertexSet[(i+1)%vNum].y,
									pointX,pointY,0.3346f,0.6433f))
					intersectNum++;


			}
		}

	//�����ڶ�����ڲ�
	if(intersectNum%2==1)
		return	D3DXVECTOR3(pointX,pointY,pointZ);
	//�����ڶ�����ⲿ
	else
		return D3DXVECTOR3(50000,50000,50000);

		



}
	
	//*************************************************************
	//��������		getReflectDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����	startPoint�ռ�������ʼ��
	//					intersectPoint�ռ��������򽻵�
	//					
	//����ֵ��		reflectDir����ʸ��
	//*************************************************************

D3DXVECTOR3 Polygon::getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
{
		
		
		//û���ཻ ֱ�ӷ���
		if(intersectPoint==D3DXVECTOR3(50000,50000,50000))
			return D3DXVECTOR3(50000,50000,50000);


		D3DXVECTOR3 direction =	startPoint-intersectPoint;
		D3DXVECTOR3 L	=	*D3DXVec3Normalize(&L,&direction);
		D3DXVECTOR3 N	=	normal;

		if(D3DXVec3Dot(&N,&L)<0)
			N	=	-N;

		
		D3DXVECTOR3 R	=	2*(D3DXVec3Dot(&N,&L))*N-L;
		D3DXVec3Normalize(&R,&R);

		return R;
}
	
	//*************************************************************
	//��������		getRefractPoint
	//���ܣ�		��ռ����ߺͼ������  ��������
	//����˵����	startPoint�ռ�������ʼ��
	//					intersectPoint�ռ����ߺͶ���εĽ���
	//					
	//����ֵ��		refractPoint��������
	//*************************************************************

D3DXVECTOR3 Polygon::getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
{

return intersectPoint;
}

	//*************************************************************
	//��������		getRefractDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����	startPoint�ռ�������ʼ��
	//					intersectPoint�ռ����ߺͶ���εĽ���
	//					
	//����ֵ��	refractDir����ʸ��
	//*************************************************************

D3DXVECTOR3 Polygon::getRefractDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
{


		//û���ཻ ֱ�ӷ���
		if(intersectPoint==D3DXVECTOR3(50000,50000,50000))
			return D3DXVECTOR3(50000,50000,50000);

		//�淶��������
		D3DXVECTOR3 N = normal;
		//�淶���������
		D3DXVECTOR3 L = startPoint-intersectPoint;
		D3DXVec3Normalize(&L,&L);

		if(D3DXVec3Dot(&N,&L)<0)
			N = -N;
		
		float cosOut	=	D3DXVec3Dot(&N,&L);		//�����cos
		float relativeR	=	static_cast<float>(1.0003/refractRate);		//�����������ʵ� ���������
		float cosIn		=	static_cast<float>(sqrt(1-pow(relativeR,2)*(1-pow(cosOut,2))));	//�����cos
		D3DXVECTOR3 InDir = (relativeR*cosOut-cosIn)*N-relativeR*L;
		D3DXVec3Normalize(&InDir,&InDir);
				//�淶�������������
		
		return InDir;
}

//�ж��߶κ������Ƿ��н���
bool Polygon::isIntersect(float x0,float y0,float x1,float y1,float px,float py,float u,float v)
{	
	//ֱ�߷��̲���
	float a,b,c;
	
	//�߶β����� ����false
	if((abs(x0-x1)<1e-4)&&(abs(y0-y1)<1e-4))
		return false;
	//
	else if(abs(x0-x1)<1e-4)
	{	
		a	=	1;
		b	=	0;
		c	=	-x0;

	}
	else if(abs(y0-y1)<1e-4)
	{
		a	=	0;
		b	=	1;
		c	=	-y0;
	}
	else
	{
	float k	=	(y1-y0)/(x1-x0);//б��
	a	=	k;
	b	=	-1;
	c	=	y0-x0*k;

	}
	
	//���߷��̲���
	float t;
	
	float tempL	=	a*u+b*v;
	float tempR	=	-c-a*px-b*py;
	
	//����������߶�����ֱ����
	if(abs(tempL)<1e-4)
		return false;
	t = tempR/tempL;
	
	//�������߶�����ֱ�߲��ཻ
	if(t<0)
		return false;
	
	//�������жϽ����Ƿ����߶�����
	
	//������ƽ�ơ���ת�� ���������ԭ�� ָ��x������
	//ƽ��
	float x0_A	=	x0-px;
	float y0_A	=	y0-py;
	float x1_A	=	x1-px;
	float y1_A	=	y1-py;
	
	float Rlen	=	static_cast<float>(sqrt(u*u+v*v));
	float cosR	=	u/Rlen;
	float sinR	=	v/Rlen;
	//��ת
	y0_A	=	y0_A*cosR	-	x0_A*sinR;
	y1_A	=	y1_A*cosR	-	x1_A*sinR;



	//���ߺ��߶��Ҷε��ཻ
	if(y1_A==0||y1_A*y0_A<0)
		return true;

	
	
	//�������߶�����ֱ�߽��㲻���߶�����
	
		return false;
			
}


vector<D3DXVECTOR3> Polygon::getVertexSet()
{

return vertexSet;

}
	
	
void 	Polygon::setVertexSet(const vector<D3DXVECTOR3>& inSet)
{

vertexSet	=	inSet;

}
	
D3DXVECTOR3 Polygon::getNormal(const D3DXVECTOR3& surfacePoint)
{

return normal;


}

void Polygon::setNormal(const D3DXVECTOR3& inNormal)
{
normal	=	inNormal;
}


D3DXVECTOR3 Polygon::getMaterial()
{
return material;
}


void Polygon::setMaterial(const D3DXVECTOR3& inMaterial)
{
material	=	inMaterial;
}


float Polygon::getRefractRate()
{
return refractRate;
}


void Polygon::setRefractRate(float inRate)
{
refractRate	=	inRate;
}


D3DXVECTOR4 Polygon::getPlaneEquParam()
{
return planeEquPara;
}


void Polygon::setPlaneEquParam(const D3DXVECTOR4& inEqu)
{
planeEquPara =	inEqu;
}





void Polygon::setTrans(bool inTrans)
{
isTransparency	=	inTrans;
}


//���ض���β����ַ���
CString Polygon::getString(void)
{

CString temp;
temp.Format(L"POLYGON: VerticesNum(%d) Material(%.2f,%.2f,%.2f) RefractRate(%.2f) Trans(%d)  Reflect(%d) Tex(%d : %s) ",getVertexNum(),material.x,material.y,material.z,refractRate,isTransparency,isReflectAble,ifHasTexture,textureName);
		
return temp;
}


//������������
bool Raytracing::Polygon::getUV(int& U, int& V,const D3DXVECTOR3& inPoint)
{
	//���Ǿ���ֱ�ӷ���
	if(getVertexNum()!=4)
		return false;

	//��������ͶӰ
	//�ҳ������������� ����ͶӰ
	//ƽ�淽�̲���
		float A	=	planeEquPara.x;
		float B	=	planeEquPara.y;
		float C	=	planeEquPara.z;
		float D	=	planeEquPara.w;	
	
	float MMAX	=	abs(A)>abs(B)?abs(A):abs(B);
		MMAX	=	MMAX>abs(C)?MMAX:abs(C);
		
		float x0,y0,x1,y1,x2,y2,x3,y3;
		float xp,yp;
		//ͶӰ��YOZƽ��
		if(MMAX==abs(A))
		{
			x0=vertexSet[0].y;
			y0=vertexSet[0].z;
			x1=vertexSet[1].y;
			y1=vertexSet[1].z;
			x2=vertexSet[2].y;
			y2=vertexSet[2].z;
			x3=vertexSet[3].y;
			y3=vertexSet[3].z;
			xp	=	inPoint.y;
			yp	=	inPoint.z;

			
		}

		//ͶӰ��XOZƽ��
		else if(MMAX==abs(B))
		{
			x0=vertexSet[0].x;
			y0=vertexSet[0].z;
			x1=vertexSet[1].x;
			y1=vertexSet[1].z;
			x2=vertexSet[2].x;
			y2=vertexSet[2].z;
			x3=vertexSet[3].x;
			y3=vertexSet[3].z;
			xp	=	inPoint.x;
			yp	=	inPoint.z;


		}

		//ͶӰ��XOYƽ��
		else
		{
			x0=vertexSet[0].x;
			y0=vertexSet[0].y;
			x1=vertexSet[1].x;
			y1=vertexSet[1].y;
			x2=vertexSet[2].x;
			y2=vertexSet[2].y;
			x3=vertexSet[3].x;
			y3=vertexSet[3].y;
			xp	=	inPoint.x;
			yp	=	inPoint.y;
		}
		
		if(x3!=x0)
		{
			U	=	static_cast<int>(abs(((xp-x0)/(x3-x0))*width));
			V	=	static_cast<int>(abs(((yp-y0)/(y2-y0))*height));
		}
		else
		{
			U	=	static_cast<int>(abs(((yp-y0)/(y3-y0))*width));
			V	=	static_cast<int>(abs(((xp-x0)/(x2-x0))*height));
		}

	return true;
}


