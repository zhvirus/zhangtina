#pragma once

#include	"Light.h"

namespace Raytracing
{
class PointLight :
	public Light
{

public:
	D3DXCOLOR		getDiffuseColor(const D3DXVECTOR3& colorPoint,const D3DXVECTOR3& pointNormal,const D3DXVECTOR3& material);
	D3DXCOLOR		getSpecularColor(const D3DXVECTOR3& colorPoint,const D3DXVECTOR3& pointNormal,const D3DXVECTOR3& material,const D3DXVECTOR3& vDirection,float cosPower=4);
	CString getString(void);

public:
	PointLight(const D3DXVECTOR3& pos,const D3DXCOLOR& color,const D3DXVECTOR3& att,float ka,float kd,float ks);
	PointLight(void);
public:
	virtual ~PointLight(void);
};
}
