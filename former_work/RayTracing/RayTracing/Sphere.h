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
	//函数名：	getReflectPoint
	//功能：		求空间射线和几何体的  反射交点
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					
	//返回值：	IntersectPoint交点
	//*************************************************************

	D3DXVECTOR3 getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction);
	
	//*************************************************************
	//函数名：	getReflectDir
	//功能：		求空间射线和几何体的  反射方向矢量
	//参数说明：startPoint空间射线起始点
	//					intersectPoint空间射线入射点
	//					
	//返回值：	reflectDir反射矢量
	//*************************************************************

	D3DXVECTOR3 getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);
	
	//*************************************************************
	//函数名：	getRefractPoint
	//功能：		求空间射线和几何体的  折射出射点
	//参数说明：startPoint空间射线起始点
	//					intersectPoint空间射线入射点
	//					
	//返回值：	refractPoint折射出射点
	//*************************************************************

	D3DXVECTOR3 getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);

	//*************************************************************
	//函数名：	getRefractDir
	//功能：		求空间射线和几何体的  折射方向矢量
	//参数说明：intersectPoint空间射线入射点
	//					outPoint空间折射出点
	//					
	//返回值：	refractDir折射矢量
	//*************************************************************

	D3DXVECTOR3 getRefractDir(const D3DXVECTOR3 &intersectPoint,const D3DXVECTOR3 &outPoint);

	private:
		D3DXVECTOR3 sphereCenter;						//球心坐标
		float r;													//球半径
		float refractRate;										//球体折射率
		D3DXVECTOR3 material;								//反射材质
		
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