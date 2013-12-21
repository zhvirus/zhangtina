#include "StdAfx.h"
#include	"Light.h"
#include "PointLight.h"

using namespace Raytracing;

PointLight::PointLight(void)
{
	type	=	POINTLIGHT;
}

PointLight::PointLight(const D3DXVECTOR3& pos,const D3DXCOLOR& color,const D3DXVECTOR3& att,float ka,float kd,float ks)
{

	position	=	pos;
	this->color	=	color;
	this->att	=	att;
	this->ka=ka;
	this->kd=kd;
	this->ks=ks;
	type	=	POINTLIGHT;
}


PointLight::~PointLight(void)
{
}




CString PointLight::getString(void)
{
CString temp;
temp.Format(L"POINTLIGHT position(%.2f,%.2f,%.2f) color(R:%.1f G:%.1f B:%.1f A:%.1f) att(%.2f %.2f %.2f) Ka(%.2f) Kd(%.2f) Ks(%.2f)",
			position.x,position.y,position.z,color.r,color.g,color.b,color.a,att.x,att.y,att.z,ka,kd,ks);

return temp;

}


//������������ɫ
D3DXCOLOR		PointLight::getDiffuseColor(const D3DXVECTOR3& colorPoint,const D3DXVECTOR3& pointNormal,const D3DXVECTOR3& material)
{

//N*L
D3DXVECTOR3 L	=	position-colorPoint;
float distance	=	D3DXVec3Length(&L);
D3DXVec3Normalize(&L,&L);
D3DXVECTOR3 N	=	pointNormal;
D3DXVec3Normalize(&N,&N);
float cosFactor	=	abs(D3DXVec3Dot(&N,&L));

//˥������

float attFactor	=	1.0f/(att.x+att.y*distance+att.z*distance*distance);

//������ϵ����N*L��˥��ϵ��
float totalFactor	=	kd*cosFactor*attFactor;
D3DXCOLOR col(color.r*totalFactor,color.g*totalFactor,color.b*totalFactor,255);

//���ز��ʷ�������ɫ
return D3DXCOLOR(col.r*material.x,col.g*material.y,col.b*material.z,255);

}

//���㾵�淴����ɫ
D3DXCOLOR		PointLight::getSpecularColor(const D3DXVECTOR3& colorPoint,const D3DXVECTOR3& pointNormal,const D3DXVECTOR3& material,const D3DXVECTOR3& vDirection,float cosPower)
{	
	//N
	D3DXVECTOR3 N	=	pointNormal;
	D3DXVec3Normalize(&N,&N);
	
	//L
	D3DXVECTOR3 L	=	position-colorPoint;
	float distance	=	D3DXVec3Length(&L);
	D3DXVec3Normalize(&L,&L);
	
	//R
	D3DXVECTOR3 R	=	2.0f*D3DXVec3Dot(&N,&L)*N-L;
	D3DXVec3Normalize(&R,&R);
	//���۾�����������һ��
	D3DXVECTOR3 V1	=	vDirection;
	D3DXVec3Normalize(&V1,&V1);
	float cosFactor	=	static_cast<float>(abs(pow(D3DXVec3Dot(&V1,&R),cosPower)));



	////V
	//	//���۾�����������һ��
	//	D3DXVECTOR3 V1	=	vDirection;
	//	D3DXVec3Normalize(&V1,&V1);

	//	//���۾���������ͶӰ��L&Nƽ�� �����V 
	//	D3DXVECTOR3 nTemp;
	//	D3DXVec3Cross(&nTemp,&L,&N);
	//	D3DXVec3Normalize(&nTemp,&nTemp);
	//	float haha	=	D3DXVec3Dot(&nTemp,&V1);
	//	if(haha<0)
	//	{
	//		nTemp=-nTemp;
	//		haha=-haha;
	//	}
	//	float cosP	=	sqrt(1-pow(haha,2));
	//	D3DXVECTOR3 V	=	V1-nTemp*haha;
	//	D3DXVec3Normalize(&V,&V);


	////H
	//D3DXVECTOR3 H	=	(L+V)/2;
	//D3DXVec3Normalize(&H,&H);

	////(N*H)^n
	//float cosFactor	=	static_cast<float>(abs(pow(D3DXVec3Dot(&N,&H),cosPower)));

	//˥������
	
	float attFactor	=	1.0f/(att.x+att.y*distance+att.z*distance*distance);

	//���淴��ϵ����N*L��˥��ϵ��
	float totalFactor	=	ks*cosFactor*attFactor;
	D3DXCOLOR col(color.r*totalFactor,color.g*totalFactor,color.b*totalFactor,255);

	//���ز��ʷ�������ɫ
	return D3DXCOLOR(col.r*material.x,col.g*material.y,col.b*material.z,255);


}