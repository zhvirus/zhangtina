#pragma once

#include "Model.h"

namespace Raytracing

{
	class Sphere :
		public Model
	{
	public:
		Sphere(void);
		Sphere(const D3DXVECTOR3& center, const float& R, const D3DXVECTOR3& inMaterial, const float& inRefractRate,bool isTrans,bool isReflectAble);
	public:
		~Sphere(void);
	
	public:
		
	//*************************************************************
	//��������	getReflectPoint
	//���ܣ�		��ռ����ߺͼ������  ���佻��
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					
	//����ֵ��	IntersectPoint����
	//*************************************************************

	D3DXVECTOR3 getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction);
	
	//*************************************************************
	//��������	getReflectDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����startPoint�ռ�������ʼ��
	//					intersectPoint�ռ����������
	//					
	//����ֵ��	reflectDir����ʸ��
	//*************************************************************

	D3DXVECTOR3 getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);
	
	//*************************************************************
	//��������	getRefractPoint
	//���ܣ�		��ռ����ߺͼ������  ��������
	//����˵����startPoint�ռ�������ʼ��
	//					intersectPoint�ռ����������
	//					
	//����ֵ��	refractPoint��������
	//*************************************************************

	D3DXVECTOR3 getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);

	//*************************************************************
	//��������	getRefractDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����intersectPoint�ռ����������
	//					outPoint�ռ��������
	//					
	//����ֵ��	refractDir����ʸ��
	//*************************************************************

	D3DXVECTOR3 getRefractDir(const D3DXVECTOR3 &intersectPoint,const D3DXVECTOR3 &outPoint);

	private:
		D3DXVECTOR3 sphereCenter;						//��������
		float r;													//��뾶
		float refractRate;										//����������
		D3DXVECTOR3 material;								//�������
		
	public:
		D3DXVECTOR3 getCenter();
		int setCenter(const D3DXVECTOR3& center);
	
		float getR(void);
		int setR(const float& inR);
		
		int setMaterial(const D3DXVECTOR3& inMaterial);
		D3DXVECTOR3 getMaterial();

		float getRefractRate(void);
		int setRefractRate(const float& inRate);

		
		void setTrans(bool);

		


		
	public:
		D3DXVECTOR3 getNormal(const D3DXVECTOR3& surfacePoint);
	public:
		CString getString(void);
	};
}