#pragma once



namespace Raytracing
{	
	
	enum Type{POLYGON,SPHERE};

	class Model
	{
	public:
		Model(void);
	public:
		virtual ~Model(void);
		
	protected:
		Type modelType;														//模型类型
		bool isTransparency;												//物体是否透明
		bool isReflectAble;													//是否反射
	public:
		Type getModelType(void);
	public:
		bool getTrans(){return isTransparency;}
		bool getIsReflectAble(){return isReflectAble;}
		bool hasTexture(){return false;}
		Bitmap* getTex(){return NULL;}
		bool getUV(int& U, int& V,const D3DXVECTOR3& inPoint){return false;}
	//*************************************************************
	//函数名：	getReflectPoint
	//功能：		求空间射线和几何体的  反射交点
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					IntersectPoint交点
	//返回值：	
	//*************************************************************

	virtual D3DXVECTOR3 getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction)=0;
	
	//*************************************************************
	//函数名：	getReflectDir
	//功能：		求空间射线和几何体的  反射方向矢量
	//参数说明：startPoint空间射线起始点
	//					intersectPoint空间射线与球交点
	//					reflectDir反射矢量
	//返回值：	
	//*************************************************************

	virtual D3DXVECTOR3 getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)=0;
	
	//*************************************************************
	//函数名：	getRefractPoint
	//功能：		求空间射线和几何体的  折射出射点
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					refractPoint折射出射点
	//返回值：	
	//*************************************************************

	virtual D3DXVECTOR3 getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)=0;

	//*************************************************************
	//函数名：	getRefractDir
	//功能：		求空间射线和几何体的  折射方向矢量
	//参数说明：startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					
	//返回值：	refractDir折射矢量
	//*************************************************************

	virtual D3DXVECTOR3 getRefractDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)=0;

	virtual CString getString(void)=0;
	virtual D3DXVECTOR3 getNormal(const D3DXVECTOR3& surfacePoint)=0;
	virtual D3DXVECTOR3 getMaterial()=0;
	bool isTrans(){return isTransparency;}

	};

}