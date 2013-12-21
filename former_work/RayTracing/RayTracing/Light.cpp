#include "StdAfx.h"
#include "Light.h"

using namespace Raytracing;

Light::Light(void):position(50,50,50),
						color(200,200,200,200),
						att(1,1,1),
						ka(1),
						kd(1),
						ks(1)
{
}


Light::Light(const D3DXVECTOR3 &pos,const D3DXCOLOR &color,const D3DXVECTOR3&att,float ka,float kd,float ks)
{
position	=	pos;
this->color	=	color;
this->att	=	att;
this->ka=ka;
this->kd=kd;
this->ks=ks;

}


Light::~Light(void)
{
}
