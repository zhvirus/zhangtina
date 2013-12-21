#include "StdAfx.h"
#include	<fstream>
#include "RayTrace.h"
#include	"Model.h"
#include	"Sphere.h"
#include	"Polygon.h"
#include	"Light.h"
#include	"PointLight.h"

using namespace Raytracing;

RayTrace::RayTrace(void)
{
	RayTreeDepth=0;
}

RayTrace::~RayTrace(void)
{
	deleteAllObject();
	deleteAllLights();
}

//增加物体对象
bool RayTrace::addObject(Raytracing::Model*	newObject)
{
	object.push_back(newObject);
	

	return true;


}

//删除一个物体
bool RayTrace::deleteObject(const int& index)
{

	if(index<0||index>=static_cast<int>(object.size()))
		return false;

	delete object[index];
	vector<Model*>::iterator iter	=	object.begin();
	object.erase(iter	+	index);
	return true;
	

}

//删除所有物体
bool RayTrace::deleteAllObject()
{
	vector<Model*>::iterator	iter;
	for(iter	=	object.begin();iter!=object.end();iter++)
	{
		delete *iter;
		
	}
	
	object.clear();

	return true;

}

	//增加一个光源
	bool RayTrace::addLight(Light* newLight)
	{

		light.push_back(newLight);
		return true;
	}
		
	//删除一个光源
	bool RayTrace::deleteLight(const int& index)
	{
		if(index<0||index>=static_cast<int>(light.size()))
		return false;

	delete light[index];
	vector<Light*>::iterator iter	=	light.begin();
	light.erase(iter	+	index);
	return true;

	}

	//删除所有光源
	bool RayTrace::deleteAllLights()
	{
	vector<Light*>::iterator	iter;
	for(iter	=	light.begin();iter!=light.end();iter++)
	{
		delete *iter;
		
	}
	
	light.clear();

	return true;






	}






//从文件初始化 物体列表 光源列表
int RayTrace::loadObjectFromFile(const string& fileName)
{	
	int objectCount=0;
	deleteAllObject();
	deleteAllLights();

	ifstream fromFile(fileName.c_str());

	if(fromFile.fail())
		return -1;
	
	while(!fromFile.eof())
	{
	string type;
	fromFile>>type;
	if(type=="[SPHERE]")
	{	
		objectCount++;
		string name;
		float x,y,z,R,r,g,b,rate;
		bool Trans,isReflect;
		fromFile>>name>>x>>y>>z;
		fromFile>>name>>R;
		fromFile>>name>>r>>g>>b;
		fromFile>>name>>rate;
		fromFile>>name>>Trans;
		fromFile>>name>>isReflect;

		Sphere *here =new Sphere(D3DXVECTOR3(x,y,z),R,D3DXVECTOR3(r,g,b),rate,Trans,isReflect);
		addObject(here);
	}
	else if(type=="[POLYGON]")
	{	
		objectCount++;
		string name;
		float x,y,z,r,g,b,rate;
		bool Trans,isReflect,hasTex;
		int Num;
		string TexName;

		fromFile>>name>>Num;

	
		vector<D3DXVECTOR3> vSet;
		for(int i=0;i<Num;i++)
		{
			fromFile>>x>>y>>z;
			vSet.push_back(D3DXVECTOR3(x,y,z));
		}

		fromFile>>name>>r>>g>>b;
		fromFile>>name>>rate;
		fromFile>>name>>Trans;
		fromFile>>name>>isReflect;
		fromFile>>name>>hasTex;
		fromFile>>name>>TexName;
		
		CString fileName(TexName.c_str());

		Raytracing::Polygon *here = new Raytracing::Polygon(vSet,D3DXVECTOR3(r,g,b),rate,Trans,isReflect);
		if(hasTex==1)
		{
			Bitmap temp(fileName.AllocSysString());
			Bitmap *mapCloe	=	temp.Clone(0,0,temp.GetWidth(),temp.GetHeight(),PixelFormatDontCare);
			here->setTexFileName(fileName);
			here->setTexture(mapCloe);

		}


		addObject(here);
	}

	else if(type=="[POINTLIGHT]")
	{
       //PointLight(const D3DXVECTOR3 &pos,const D3DXCOLOR &color,const D3DXVECTOR3&att,float ka,float kd,float ks);
		string name;
		D3DXVECTOR3 position;
		D3DXCOLOR color;
		D3DXVECTOR3 att;
		float ka;
		float kd;
		float ks;

		fromFile>>name>>position.x>>position.y>>position.z;
		fromFile>>name>>color.r>>color.g>>color.b>>color.a;
		fromFile>>name>>att.x>>att.y>>att.z;
		fromFile>>name>>ka>>kd>>ks;

		Raytracing::PointLight* here	=	new Raytracing::PointLight(position,color,att,ka,kd,ks);
		light.push_back(here);
	}


	else
		return -1;


	}

return objectCount;
}



//将物体列表保存到文件中
int RayTrace::saveObjectToFile(const std::string&	fileName)
{
	ofstream toFile(fileName.c_str());

	vector<Model*>::iterator iter;

	for(iter = object.begin();iter!=object.end();iter++)
	{
		if((*iter)->getModelType()==SPHERE)
		{
			Sphere*	ob	=	static_cast<Sphere*>(*iter);

			toFile<<"[SPHERE]"<<endl	
				<<"Center: "<<ob->getCenter().x<<" "<<ob->getCenter().y<<" "<<ob->getCenter().z<<endl
				<<"Radius: "<<ob->getR()<<endl
				<<"Material: "<<ob->getMaterial().x<<" "<<ob->getMaterial().y<<" "<<ob->getMaterial().z<<endl
				<<"RefractRate: "<<ob->getRefractRate()<<endl
				<<"Transparency: "<<ob->isTrans()<<endl
				<<"Reflectable: "<<ob->getIsReflectAble()<<endl;
			
		}
		else
		{
		//Polygon(const vector<D3DXVECTOR3>& vSet,const D3DXVECTOR3& inMaterial,float inRefractRate,bool isTrans);
			Polygon* ob	=	static_cast<Polygon*>(*iter);

			toFile	<<"[POLYGON]"<<endl
				<<"VertexSet: "<<ob->getVertexNum()<<endl;
			
			vector<D3DXVECTOR3> set	=	ob->getVertexSet();

			for(int i=0;i<ob->getVertexNum();i++)
				toFile	<<set[i].x<<" "<<set[i].y<<" "<<set[i].z<<endl;

			toFile<<"Material: "<<ob->getMaterial().x<<" "<<ob->getMaterial().y<<" "<<ob->getMaterial().z<<endl
				<<"RefractRate: "<<ob->getRefractRate()<<endl
				<<"Transparency: "<<ob->getTrans()<<endl
				<<"Reflectable: "<<ob->getIsReflectAble()<<endl
				<<"HasTexture: "<<ob->hasTexture()<<endl;
				
				string temp=_com_util::ConvertBSTRToString(BSTR(ob->getTexFileName().GetString()));
				toFile<<"TextureName: "<<temp<<endl;
		}


	}

	
	//将光源信息写入文件
	vector<Light*>::iterator lur;
	for(lur=light.begin();lur<light.end();lur++)
	{
		if((*lur)->getLightType() == POINTLIGHT)
		{
			PointLight* lt	=static_cast<PointLight*>((*lur));
			
			toFile<<"[POINTLIGHT]"<<endl	
				<<"Postion: "<<lt->getPosition().x<<" "<<lt->getPosition().y<<" "<<lt->getPosition().z<<endl
				<<"Color: "<<lt->getColor().r<<" "<<lt->getColor().g<<" "<<lt->getColor().b<<" "<<lt->getColor().a<<endl
				<<"Attenuation: "<<lt->getAtt().x<<" "<<lt->getAtt().y<<" "<<lt->getAtt().z<<endl
				<<"Coefficient(ka,kd,ks): "<<lt->getKa()<<" "<<lt->getKd()<<" " <<lt->getKs()<<endl;



		}
		
	
	
	
	
	
	}





	toFile.close();


	return static_cast<int>(object.size());
}

//光线跟踪计算一个像素
D3DXCOLOR RayTrace::trace(D3DXVECTOR3& startPoint,D3DXVECTOR3& direction)
{
	//递归深度
	RayTreeDepth++;
	//到达递归深度 则返回背景色
	if(RayTreeDepth>5)
	{
		RayTreeDepth--;
		return D3DXCOLOR(0,0,0,255);

	}

//相交坐标
D3DXVECTOR3 intersectPoint;
//反射法向量
D3DXVECTOR3 reflectDir;

//获得第一个和光线相交的物体的序号以及相交坐标、反射法向量
int index	=	getFirstIntersectObj(startPoint,direction,intersectPoint,reflectDir);

//没有相交返回背景色
if(index==-1)
{
	RayTreeDepth--;
	return D3DXCOLOR(0,0,0,255);
}

//反射颜色
D3DXCOLOR reflectColor(0,0,0,255);
reflectColor	=	RayTrace::trace(intersectPoint,reflectDir);

//如果透明 求折射颜色
D3DXCOLOR refractColor(0,0,0,255);
if(object[index]->getTrans())
{
	
	D3DXVECTOR3 refractPoint = object[index]->getRefractPoint(startPoint,intersectPoint);
	D3DXVECTOR3 refractDir(-1,-1,-1);
	if(object[index]->getModelType()==SPHERE)
		refractDir	=	object[index]->getRefractDir(intersectPoint,refractPoint);
	else
		refractDir	=	object[index]->getRefractDir(startPoint,intersectPoint);
	refractColor = RayTrace::trace(refractPoint,refractDir);

}

//环境光颜色
D3DXCOLOR ambientColor	=	D3DXCOLOR(22,22,22,255);

//漫反射颜色
D3DXCOLOR diffuseColor(0,0,0,255);
//镜面发射光
D3DXCOLOR specularColor(0,0,0,255);


//如果矩形有纹理的话
if(object[index]->getModelType()==POLYGON&&static_cast<Raytracing::Polygon*>(object[index])->hasTexture())
{
	
		int U,V;
		Raytracing::Polygon* local	=	static_cast<Raytracing::Polygon*>(object[index]);
		if(local->getUV(U,V,intersectPoint)==true)
		{
			Color col ;
			Bitmap *temp	=	local->getTex();			
			temp->GetPixel(U,V,&col);
			D3DXVECTOR3 mate(static_cast<float>(col.GetR())/255.0f,static_cast<float>(col.GetG())/255.0f,static_cast<float>(col.GetB())/255.0f);

			diffSpecColor(diffuseColor,specularColor,intersectPoint,object[index]->getNormal(intersectPoint),mate,-direction);
			diffuseColor*=0.3f;
			specularColor*=0.3f;
			
		}

	
}

else
	diffSpecColor(diffuseColor,specularColor,intersectPoint,object[index]->getNormal(intersectPoint),object[index]->getMaterial(),-direction);





//最终颜色输出
RayTreeDepth--;


return shader(reflectColor,refractColor,ambientColor,diffuseColor,specularColor,object[index])/(static_cast<float>(RayTreeDepth+1));

}

//求与光线相交的第一个物体序号 以及 交点和 反射向量
int RayTrace::getFirstIntersectObj(D3DXVECTOR3& startPoint,const D3DXVECTOR3& direction,D3DXVECTOR3& Point,D3DXVECTOR3& dir)
{	
	//物体序号
	int objectIndex = -1;
	//直线参数方程 在交点处 参数值 初始化为比较大的数
	float len2	=	10000000;
	
	//将射线与所有物体求交点	
	for(int i=0;i<static_cast<int>(object.size());i++)
	{	
		//求出交点
		D3DXVECTOR3 intersectPoint	=	object[i]->getReflectPoint(startPoint,direction);
		
		//如果交点存在，则继续求参数t 和之前的t进行比较 保留最小的
		if(intersectPoint!=D3DXVECTOR3(50000,50000,50000))
		{	
			
			float t;		
	
			if(abs(direction.x)>1e-4)
				t = (intersectPoint.x-startPoint.x)/direction.x;
			else if(abs(direction.y)>1e-4)
				t = (intersectPoint.y-startPoint.y)/direction.y;
			else
				t = (intersectPoint.z-startPoint.z)/direction.z;
			//如果所求交点比之前的点更加靠近视点 则更新最近点
			if(t<len2)
			{
				objectIndex	=	i;
				len2	=	t;
				Point	=	intersectPoint;

			}


		}
	}

	if(objectIndex!=-1)
		dir	=	object[objectIndex]->getReflectDir(startPoint,Point);

	return objectIndex;


}
// 最终颜色的计算
D3DXCOLOR Raytracing::RayTrace::shader(const D3DXCOLOR& reflect,const D3DXCOLOR& refract,const D3DXCOLOR& ambient,const D3DXCOLOR& diffuse,const D3DXCOLOR& specular,Model* obj)
{

//反射颜色	
D3DXCOLOR reflectColor	=	reflect;
float reflectK	=1.0f;
//折射颜色
D3DXCOLOR refractColor	=	refract;
float refractK	=1.0f;
//环境光颜色
D3DXCOLOR ambientColor	=	ambient;
float ambientK	=1.0f;
	
//漫反射颜色
D3DXCOLOR diffuseColor	=	diffuse;
float diffuseK	=1.3f;
//镜面发射光
D3DXCOLOR specularColor = specular;
float specularK	=0.4f;

bool reflectAble	=	obj->getIsReflectAble();
bool transparency	=	obj->getTrans();


//可反射的话
if(reflectAble)
{
	diffuseK = 0.04f;
	ambientK=0.04f;
	specularK=0.04f;
	reflectK	=	reflectK*2.0f;

}
else
{
reflectK	=	0.04f;
}


if(transparency)
{
	refractK	=	refractK*2.3f;
	diffuseK = 0.04f;
	ambientK=0.04f;
	specularK=0.04f;
}



D3DXCOLOR finalPixel	=	reflectColor*reflectK+refractColor*refractK+ambientColor*ambientK+diffuseColor*diffuseK+specularColor*specularK;




if(finalPixel.r>255)
	finalPixel.r=255;
if(finalPixel.g>255)
	finalPixel.g=255;
if(finalPixel.b>255)
	finalPixel.b=255;
if(finalPixel.a>255)
	finalPixel.a=255;

	return finalPixel;
}

// 计算点和光源之间是否没有被物体档住
//返回值	true：没有被物体档住	false:被物体档住
int Raytracing::RayTrace::isReachLight(const D3DXVECTOR3& colorPoint, const D3DXVECTOR3& lightPoint)
{
	D3DXVECTOR3 lightDir	=	lightPoint-colorPoint;
	
	//光源和物体表面点连线之间不透明物体个数
	int solidNum	=	0;
	//光源和物体表面点连线之间透明物体个数
	int transNum=0;

	vector<Model*>::iterator iter;
	for(iter=object.begin();iter!=object.end();iter++)
	{
		D3DXVECTOR3 intersectPoint	=	(*iter)->getReflectPoint(colorPoint,lightDir);
		
		//颜色点为起始点，指向光源的向量和物体有交点
		if(intersectPoint!=D3DXVECTOR3(50000,50000,50000))
		{	
			
			//通过向量长度来判断交点在 光源和颜色点之间还是之外
			D3DXVECTOR3 L1	=	intersectPoint	-	colorPoint;
			D3DXVECTOR3 L2	=	lightPoint	-	colorPoint;
			float len1	=	D3DXVec3Length(&L1);
			float len2	=	D3DXVec3Length(&L2);

				if((len1>1e-4&&len1<len2))
				{
					if((*iter)->getTrans()==true)
						transNum++;
					else
						solidNum++;
				
				}
				
			}	//if
		
		
		}//for

	
	
if(solidNum==0&&transNum==0)
	return 1;
else if(solidNum>0)
	return 0;
else
	return 2+transNum;
	
}






//计算漫反射和镜面反射
int Raytracing::RayTrace::diffSpecColor(D3DXCOLOR& diff, D3DXCOLOR& spec, D3DXVECTOR3 colorPoint, D3DXVECTOR3 normal, D3DXVECTOR3 material, D3DXVECTOR3 eyeDirection)
{
	


		vector<Light*>::iterator iter;
		for(iter=light.begin();iter!=light.end();iter++)
		{
			D3DXVECTOR3 lightPosition	=	(*iter)->getPosition();
			int re	=	isReachLight(colorPoint,lightPosition);
			if(re==0)
			{
				continue;
				
			}
			else
			{
				float haha	=	static_cast<float>(re);
				spec	=	spec+(*iter)->getSpecularColor(colorPoint,normal,material,eyeDirection,6)/haha;
				diff	=	diff+(*iter)->getDiffuseColor(colorPoint,normal,material)/haha;

			}


		}
	




	return 0;
}
