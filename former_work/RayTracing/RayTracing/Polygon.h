#pragma once

#include	"Model.h"
using namespace std;



namespace Raytracing
{
class Polygon :
	public Model
{
private:
		vector<D3DXVECTOR3> vertexSet;			//多边形顶点序列
		D3DXVECTOR3 normal;							//多边形法向量
		D3DXVECTOR3 material;							//多边形材质
		float	refractRate;										//多边形折射率
		D3DXVECTOR4 planeEquPara;					//平面方程参数 Ax+By+Cz+D=0
		Bitmap *texture;										//纹理
		bool	ifHasTexture;									//是否有纹理
		CString textureName;								//纹理图片文件名
public:
		int width,height;										//纹理尺寸
		
public:

	CString getTexFileName(){return textureName;}
	void setTexFileName(const CString& in){textureName = in;}

	bool hasTexture(){return ifHasTexture;}
	Bitmap* getTex(){return texture;}
	void setHasTexture(bool is){ifHasTexture	=	is;}

	vector<D3DXVECTOR3> getVertexSet();
	void 	setVertexSet(const vector<D3DXVECTOR3>&);
	
	D3DXVECTOR3 getNormal(const D3DXVECTOR3& surfacePoint);
	void setNormal(const D3DXVECTOR3&);

	
	void setReflectAble(bool is){isReflectAble	=	is;}

	D3DXVECTOR3 getMaterial();
	void setMaterial(const D3DXVECTOR3&);

	float getRefractRate();
	void setRefractRate(float);

	D3DXVECTOR4 getPlaneEquParam();
	void setPlaneEquParam(const D3DXVECTOR4&);

	
	void setTrans(bool);

	CString getString(void);


public:
	Polygon(void);
	Polygon(const vector<D3DXVECTOR3>& vSet,const D3DXVECTOR3& inMaterial,float inRefractRate,bool isTrans,bool isReflect);
	//返回多边形顶点数目
	int getVertexNum();
	//传入图片资源
	bool setTexture(Bitmap *in);
private:
	//判断二维空间线段和射线是否相交
	bool isIntersect(float x0,float y0,float x1,float y1,float px,float py,float u,float v);

public:
	virtual ~Polygon(void);

public:

	//*************************************************************
	//函数名：	getReflectPoint
	//功能：		求空间射线和几何体的  反射交点
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					IntersectPoint交点
	//返回值：	
	//*************************************************************

	D3DXVECTOR3 getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction);
	
	//*************************************************************
	//函数名：	getReflectDir
	//功能：		求空间射线和几何体的  反射方向矢量
	//参数说明：startPoint空间射线起始点
	//					intersectPoint空间射线与球交点
	//					reflectDir反射矢量
	//返回值：	
	//*************************************************************

	D3DXVECTOR3 getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);
	
	//*************************************************************
	//函数名：	getRefractPoint
	//功能：		求空间射线和几何体的  折射出射点
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					refractPoint折射出射点
	//返回值：	
	//*************************************************************

	D3DXVECTOR3 getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);

	//*************************************************************
	//函数名：	getRefractDir
	//功能：		求空间射线和几何体的  折射方向矢量
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					
	//返回值：	refractDir折射矢量
	//*************************************************************

	D3DXVECTOR3 getRefractDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);

public:
	bool getUV(int& U, int& V,const D3DXVECTOR3& inPoint);
	};
}