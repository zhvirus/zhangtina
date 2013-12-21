#pragma once

namespace Raytracing
{
	enum LIGHTTYPE{POINTLIGHT};
class Light
{
protected:
	D3DXVECTOR3 position;		//	光源位置
	D3DXCOLOR color;			//	光源颜色
	D3DXVECTOR3 att;			//	二次倒数的衰减系数 x ==>a0  y==>a1  z==>a2
	float ka;							//	环境光系数
	float kd;							//	漫反射光系数
	float ks;							//	镜面反射光系数
	LIGHTTYPE type;
public:
	D3DXVECTOR3 getPosition(){return position;}
	D3DXCOLOR		getColor(){return color;}
	D3DXVECTOR3 getAtt(){return att;}
	float getKa(){return ka;}
	float getKd(){return kd;}
	float getKs(){return ks;}

	LIGHTTYPE getLightType(){return type;}

	void setPosition(const D3DXVECTOR3& position)	{this->position=position;}
	void setColor(const D3DXCOLOR& color)			{this->color=color;}
	void setAtt(const D3DXVECTOR3& att)				{this->att=att;}
	void setKa(float ka){this->ka=ka;}
	void setKd(float kd){this->kd=kd;}
	void setKs(float ks){this->ks=ks;}


public:
	Light(void);
	Light(const D3DXVECTOR3 &pos,const D3DXCOLOR &color,const D3DXVECTOR3&att,float ka,float kd,float ks);
	virtual D3DXCOLOR	getDiffuseColor(const D3DXVECTOR3& colorPoint,const D3DXVECTOR3& pointNormal,const D3DXVECTOR3& material)=0;
	virtual D3DXCOLOR	getSpecularColor(const D3DXVECTOR3& colorPoint,const D3DXVECTOR3& pointNormal,const D3DXVECTOR3& material,const D3DXVECTOR3& vDirection,float cosPower=8)=0;
	virtual CString getString(void)=0;
public:
	virtual ~Light(void);
};
}