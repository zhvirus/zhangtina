#include "StdAfx.h"
#include "Model.h"
#include "Sphere.h"

using namespace Raytracing;

Sphere::Sphere(void)
{
	modelType=SPHERE;
}

Sphere::Sphere(const D3DXVECTOR3& center, const float& R, const D3DXVECTOR3& inMaterial, const float& inRefractRate,bool isTrans,bool isReflactAble)
{
	modelType=SPHERE;
	sphereCenter = center;
	r = R;
	material = inMaterial;
	refractRate = inRefractRate;
	isTransparency	=	isTrans;
	isReflectAble		=	isReflactAble;
}


Sphere::~Sphere(void)
{
}

	//*************************************************************
	//��������		getReflectPoint
	//���ܣ�		��ռ����ߺͼ������  ���佻��
	//����˵���� startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					
	//����ֵ��	IntersectPoint����
	//*************************************************************

	D3DXVECTOR3 Sphere::getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction)
	{
		
		//������ʼ��
		float x0	=	startPoint.x;
		float y0	=	startPoint.y;
		float z0	=	startPoint.z;
		//���߷���ʸ��
		float a	=	direction.x;
		float b	=	direction.y;
		float c	=	direction.z;
		//��������
		float Rx	=	sphereCenter.x;
		float Ry	=	sphereCenter.y;
		float Rz	=	sphereCenter.z;
		//��뾶
		float R	=	r;
		
		//����������������ȷ ֱ�ӷ���
		if((abs(a)<(1e-4))&&(abs(b)<(1e-4))&&(abs(c)<(1e-4)))
		{
			
			return D3DXVECTOR3(50000,50000,50000);
		}

		//ֱ�߲������̴����򷽳̺󻯽�õ��� ���ڲ���t��һԪ���η��̵�����ϵ��
		float C2	=	a*a+b*b+c*c;
		float C1	=	2*(a*(x0-Rx)+b*(y0-Ry)+c*(z0-Rz));
		float C0	=	pow(x0-Rx,2)+pow(y0-Ry,2)+pow(z0-Rz,2)-R*R;

		float delta	=	C1*C1-4*C2*C0;

		if(delta<0)
		{
			return D3DXVECTOR3(50000,50000,50000);
		}

		float t1	=	(-C1+sqrt(delta))/(2*C2);
		float t2	=	(-C1-sqrt(delta))/(2*C2);
		
		if(t1>(1e-4)&&t2>(1e-4))
		{
			//������������⣬�����ཻ��ȡ�����ǵ㣨ֻ��һ��Ļ��ͷ����Ǹ��㣩
			float tt	=	t1<t2?t1:t2;
			return D3DXVECTOR3(x0+tt*a,y0+tt*b,z0+tt*c);
		}
		else if((abs(t1)<(1e-4)&&t2>0)||(t1>0&&abs(t2)<(1e-4)))
		{	
			//���������������,�����ڲ���
			float tt	=	t1>t2?t1:t2;
			return D3DXVECTOR3(x0+tt*a,y0+tt*b,z0+tt*c);
			//return D3DXVECTOR3(50000,50000,50000);

		}
		else
		{
			
			return D3DXVECTOR3(50000,50000,50000);

		}
		
		

		
	}
	
	//*************************************************************
	//��������		getReflectDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����	startPoint�ռ�������ʼ��
	//					intersectPoint�ռ��������򽻵�
	//					
	//����ֵ��		reflectDir����ʸ��
	//*************************************************************

	D3DXVECTOR3 Sphere::getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
	{
		D3DXVECTOR3 direction =	startPoint-intersectPoint;
		
		//û���ཻ ֱ�ӷ���
		if(intersectPoint==D3DXVECTOR3(50000,50000,50000))
			return D3DXVECTOR3(50000,50000,50000);
		D3DXVECTOR3 N	=	getNormal(intersectPoint);
		D3DXVECTOR3 L		=	*D3DXVec3Normalize(&L,&direction);
		D3DXVECTOR3 R	=	2*(D3DXVec3Dot(&N,&L))*N-L;
		D3DXVec3Normalize(&R,&R);

		return R;
		
		

	}
	
	//*************************************************************
	//��������		getRefractPoint
	//���ܣ�		��ռ����ߺͼ������  ��������
	//����˵����	startPoint�ռ�������ʼ��
	//					intersectPoint�ռ��������򽻵�
	//					
	//����ֵ��		��������
	//*************************************************************

	D3DXVECTOR3 Sphere::getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
	{	
			//û���ཻ ֱ�ӷ���
		if(intersectPoint==D3DXVECTOR3(50000,50000,50000))
			return D3DXVECTOR3(50000,50000,50000);

		//�淶������㷨����
		D3DXVECTOR3 N = getNormal(intersectPoint);
		//�淶���������
		D3DXVECTOR3 L = startPoint - intersectPoint;
		D3DXVec3Normalize(&L,&L);
		
		float cosOut	=	D3DXVec3Dot(&N,&L);		//�����cos
		float relativeR	=	static_cast<float>(1.0003/refractRate);		//�����������ʵ� ���������
		float cosIn		=	static_cast<float>(sqrt(1-pow(relativeR,2)*(1-pow(cosOut,2))));	//�����cos
		D3DXVECTOR3 InDir = (relativeR*cosOut-cosIn)*N-relativeR*L;
		D3DXVec3Normalize(&InDir,&InDir);			//�淶�������������

		D3DXVECTOR3 outPoint = getReflectPoint(intersectPoint,InDir);
		
		
		
		
		return outPoint;

	}




	//*************************************************************
	//��������		getRefractDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����	intersectPoint�ռ����������
	//					outPoint�ռ��������
	//					
	//����ֵ��		refractDir����ʸ��
	//*************************************************************


	D3DXVECTOR3 Sphere::getRefractDir(const D3DXVECTOR3 &intersectPoint,const D3DXVECTOR3 &outPoint)
	{
		
	
		//û�г���� ֱ�ӷ���
		if(outPoint==D3DXVECTOR3(50000,50000,50000)||intersectPoint==D3DXVECTOR3(50000,50000,50000))
			return D3DXVECTOR3(50000,50000,50000);

		//�淶������㷨����
		D3DXVECTOR3 N = getNormal(outPoint);
		//�淶�������ڲ��������
		D3DXVECTOR3 T = intersectPoint - outPoint;
		D3DXVec3Normalize(&T,&T);
		
		float cosIn		=	-D3DXVec3Dot(&T,&N);		//�����ڲ������cos
		float relativeR	=	static_cast<float>(1.0003/refractRate);			//�����������ʵ� ���������
		float cosOut		= 	static_cast<float>(sqrt(1-pow(1.0/relativeR,2)*(1-pow(cosIn,2))));//���������cos
		D3DXVECTOR3 outDir = (T-(relativeR*cosOut-cosIn)*N)/(-relativeR);
		D3DXVec3Normalize(&outDir,&outDir);		//�淶����������������


		
		
		
		
		return outDir;
			
	}






	D3DXVECTOR3 Raytracing::Sphere::getCenter()
	{
		
		
		return sphereCenter;
	}

	int Raytracing::Sphere::setCenter(const D3DXVECTOR3& center)
	{	
		sphereCenter = center;
		return 0;
	}

	float Raytracing::Sphere::getR(void)
	{
		return r;
	}

	int Raytracing::Sphere::setR(const float& inR)
	{	
		r = inR;
		return 0;
	}

	int Raytracing::Sphere::setMaterial(const D3DXVECTOR3& inMaterial)
	{	
		material = inMaterial;
		return 0;
	}

	D3DXVECTOR3 Raytracing::Sphere::getMaterial()
	{
		return material;
	}

	float Raytracing::Sphere::getRefractRate(void)
	{
		return refractRate;
	}

	int Raytracing::Sphere::setRefractRate(const float& inRate)
	{
		refractRate = inRate;
		return 0;
	}

	

	void Raytracing::Sphere::setTrans(bool inTrans)
	{

		isTransparency	=	inTrans;

	}

	D3DXVECTOR3 Raytracing::Sphere::getNormal(const D3DXVECTOR3& surfacePoint)
	{	
		D3DXVECTOR3 normal(surfacePoint.x-sphereCenter.x,surfacePoint.y-sphereCenter.y,surfacePoint.z-sphereCenter.z);

		D3DXVec3Normalize(&normal,&normal);
		return normal;
	}

	//���ض�����Ϣ�ַ���
	CString Raytracing::Sphere::getString(void)
	{
		CString temp;
		temp.Format(L"SPHERE: Center(%.2f,%.2f,%.2f) Radius(%.2f) Material(%.2f,%.2f,%.2f) RefractRate(%.2f) Transparency(%d) ReflectAble(%d)",sphereCenter.x,sphereCenter.y,sphereCenter.z,r,material.x,material.y,material.z,refractRate,isTransparency,isReflectAble);
		
		return temp;
		
	}
