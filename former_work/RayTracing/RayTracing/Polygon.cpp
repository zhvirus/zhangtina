#include "StdAfx.h"
#include	"Model.h"
#include "Polygon.h"

using namespace Raytracing;

Polygon::Polygon(void)
{
}

//**************************************************
//函数名：		构造函数
//参数说明：	vSet输入的多边形顶点集
//					inMaterial输入多边形材质
//					inRafractRate输入多边形折射率
//					isTrans多边形是否透明
//**************************************************

Polygon::Polygon(const vector<D3DXVECTOR3>& vSet,const D3DXVECTOR3& inMaterial,float inRefractRate,bool isTrans,bool isReflect)
{
	vertexSet	=	vSet;					//顶点集
	material	=	inMaterial;			//材质
	refractRate	=	inRefractRate;	//折射率
	isTransparency	=	isTrans;		//是否透明
	isReflectAble	=	isReflect;			//是否可反射
	modelType	=	POLYGON;		//物体类型
	ifHasTexture		=	false;			//没有纹理
	texture		=	NULL;					//纹理图片资源初始化为空
	
	D3DXVECTOR3 vec10	=	vSet[0]-vSet[1];
	D3DXVECTOR3 vec12	=	vSet[2]-vSet[1];
	D3DXVECTOR3 nor	=	*D3DXVec3Cross(&nor,&vec12,&vec10);
	D3DXVec3Normalize(&nor,&nor);
	normal	=	nor;							//法向量

	float xN	=	nor.x;
	float yN	=	nor.y;
	float zN	=	nor.z;
	
	float x0	=	vSet[0].x;
	float y0	=	vSet[0].y;
	float z0	=	vSet[0].z;
  
	//planeEquPara   平面方程A B C D
	planeEquPara.x	=	xN;
	planeEquPara.y	=	yN;
	planeEquPara.z	=	zN;
	planeEquPara.w	=	-(xN*x0+yN*y0+zN*z0);

	//纹理图像为空
	setTexFileName(L"NULL");
}


Polygon::~Polygon(void)
{
	//清理图片资源
	if(texture!=NULL)
	{
		delete texture;
		texture=NULL;	
	}
}
//返回多边形顶点数目
int Polygon::getVertexNum()
{
	
	return static_cast<int>(vertexSet.size());

}


//传入图片资源
bool Polygon::setTexture(Bitmap *in)
{

//传入图片有效
if(in!=NULL)
{
	//清理图片资源
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
	//函数名：		getReflectPoint
	//功能：		求空间射线和几何体的  反射交点
	//参数说明：	startPoint空间射线起始点
	//					direction空间射线方向矢量
	//					
	//返回值：		返回射线和多边形交点，若不相交返回(50000,50000,50000)
	//*************************************************************

D3DXVECTOR3 Polygon::getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction)
{		
		
		//射线起始点
		float x0	=	startPoint.x;
		float y0	=	startPoint.y;
		float z0	=	startPoint.z;
		//射线方向矢量
		float a	=	direction.x;
		float b	=	direction.y;
		float c	=	direction.z;
		//平面方程参数
		float A	=	planeEquPara.x;
		float B	=	planeEquPara.y;
		float C	=	planeEquPara.z;
		float D	=	planeEquPara.w;

		//射线和面平行或者在面内
		float value	=	A*a+B*b+C*c;
		if(abs(value)<1e-4)
			return D3DXVECTOR3(50000,50000,50000);
		
		//直线平面交点的参数t
		float t	=	-(D+A*x0+B*y0+C*z0)/value;
		
		//没有和平面相交 直接返回
		if(t<=1e-4)
			return D3DXVECTOR3(50000,50000,50000);
		
		//直线平面交点
		float pointX	=	x0 +	a*t;
		float pointY	=	y0	+	b*t;
		float pointZ	=	z0	+	c*t;
		
		//检测交点是否在多边形内部
		int intersectNum	=	0;
		int vNum	=	getVertexNum();
		
		//找出法向量最大分量 进行投影
		float MMAX	=	abs(A)>abs(B)?abs(A):abs(B);
		MMAX	=	MMAX>abs(C)?MMAX:abs(C);
		
		//投影到YOZ平面
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

		//投影到XOZ平面
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

		//投影到XOY平面
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

	//交点在多边形内部
	if(intersectNum%2==1)
		return	D3DXVECTOR3(pointX,pointY,pointZ);
	//交点在多边形外部
	else
		return D3DXVECTOR3(50000,50000,50000);

		



}
	
	//*************************************************************
	//函数名：		getReflectDir
	//功能：		求空间射线和几何体的  反射方向矢量
	//参数说明：	startPoint空间射线起始点
	//					intersectPoint空间射线与球交点
	//					
	//返回值：		reflectDir反射矢量
	//*************************************************************

D3DXVECTOR3 Polygon::getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
{
		
		
		//没有相交 直接返回
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
	//函数名：		getRefractPoint
	//功能：		求空间射线和几何体的  折射出射点
	//参数说明：	startPoint空间射线起始点
	//					intersectPoint空间射线和多边形的交点
	//					
	//返回值：		refractPoint折射出射点
	//*************************************************************

D3DXVECTOR3 Polygon::getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
{

return intersectPoint;
}

	//*************************************************************
	//函数名：		getRefractDir
	//功能：		求空间射线和几何体的  折射方向矢量
	//参数说明：	startPoint空间射线起始点
	//					intersectPoint空间射线和多边形的交点
	//					
	//返回值：	refractDir折射矢量
	//*************************************************************

D3DXVECTOR3 Polygon::getRefractDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)
{


		//没有相交 直接返回
		if(intersectPoint==D3DXVECTOR3(50000,50000,50000))
			return D3DXVECTOR3(50000,50000,50000);

		//规范化法向量
		D3DXVECTOR3 N = normal;
		//规范化入射光线
		D3DXVECTOR3 L = startPoint-intersectPoint;
		D3DXVec3Normalize(&L,&L);

		if(D3DXVec3Dot(&N,&L)<0)
			N = -N;
		
		float cosOut	=	D3DXVec3Dot(&N,&L);		//入射角cos
		float relativeR	=	static_cast<float>(1.0003/refractRate);		//空气相对球材质的 相对折射率
		float cosIn		=	static_cast<float>(sqrt(1-pow(relativeR,2)*(1-pow(cosOut,2))));	//折射角cos
		D3DXVECTOR3 InDir = (relativeR*cosOut-cosIn)*N-relativeR*L;
		D3DXVec3Normalize(&InDir,&InDir);
				//规范化折射光线向量
		
		return InDir;
}

//判断线段和射线是否有交点
bool Polygon::isIntersect(float x0,float y0,float x1,float y1,float px,float py,float u,float v)
{	
	//直线方程参数
	float a,b,c;
	
	//线段不存在 返回false
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
	float k	=	(y1-y0)/(x1-x0);//斜率
	a	=	k;
	b	=	-1;
	c	=	y0-x0*k;

	}
	
	//射线方程参数
	float t;
	
	float tempL	=	a*u+b*v;
	float tempR	=	-c-a*px-b*py;
	
	//射线起点在线段所在直线上
	if(abs(tempL)<1e-4)
		return false;
	t = tempR/tempL;
	
	//射线与线段所在直线不相交
	if(t<0)
		return false;
	
	//接下来判断交点是否在线段里面
	
	//将射线平移、旋转成 起点在坐标原点 指向x正方向
	//平移
	float x0_A	=	x0-px;
	float y0_A	=	y0-py;
	float x1_A	=	x1-px;
	float y1_A	=	y1-py;
	
	float Rlen	=	static_cast<float>(sqrt(u*u+v*v));
	float cosR	=	u/Rlen;
	float sinR	=	v/Rlen;
	//旋转
	y0_A	=	y0_A*cosR	-	x0_A*sinR;
	y1_A	=	y1_A*cosR	-	x1_A*sinR;



	//射线和线段右段点相交
	if(y1_A==0||y1_A*y0_A<0)
		return true;

	
	
	//射线于线段所在直线交点不在线段里面
	
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


//返回多边形参数字符串
CString Polygon::getString(void)
{

CString temp;
temp.Format(L"POLYGON: VerticesNum(%d) Material(%.2f,%.2f,%.2f) RefractRate(%.2f) Trans(%d)  Reflect(%d) Tex(%d : %s) ",getVertexNum(),material.x,material.y,material.z,refractRate,isTransparency,isReflectAble,ifHasTexture,textureName);
		
return temp;
}


//返回纹理坐标
bool Raytracing::Polygon::getUV(int& U, int& V,const D3DXVECTOR3& inPoint)
{
	//不是矩形直接返回
	if(getVertexNum()!=4)
		return false;

	//进行坐标投影
	//找出法向量最大分量 进行投影
	//平面方程参数
		float A	=	planeEquPara.x;
		float B	=	planeEquPara.y;
		float C	=	planeEquPara.z;
		float D	=	planeEquPara.w;	
	
	float MMAX	=	abs(A)>abs(B)?abs(A):abs(B);
		MMAX	=	MMAX>abs(C)?MMAX:abs(C);
		
		float x0,y0,x1,y1,x2,y2,x3,y3;
		float xp,yp;
		//投影到YOZ平面
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

		//投影到XOZ平面
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

		//投影到XOY平面
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


