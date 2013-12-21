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

//�����������
bool RayTrace::addObject(Raytracing::Model*	newObject)
{
	object.push_back(newObject);
	

	return true;


}

//ɾ��һ������
bool RayTrace::deleteObject(const int& index)
{

	if(index<0||index>=static_cast<int>(object.size()))
		return false;

	delete object[index];
	vector<Model*>::iterator iter	=	object.begin();
	object.erase(iter	+	index);
	return true;
	

}

//ɾ����������
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

	//����һ����Դ
	bool RayTrace::addLight(Light* newLight)
	{

		light.push_back(newLight);
		return true;
	}
		
	//ɾ��һ����Դ
	bool RayTrace::deleteLight(const int& index)
	{
		if(index<0||index>=static_cast<int>(light.size()))
		return false;

	delete light[index];
	vector<Light*>::iterator iter	=	light.begin();
	light.erase(iter	+	index);
	return true;

	}

	//ɾ�����й�Դ
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






//���ļ���ʼ�� �����б� ��Դ�б�
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



//�������б��浽�ļ���
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

	
	//����Դ��Ϣд���ļ�
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

//���߸��ټ���һ������
D3DXCOLOR RayTrace::trace(D3DXVECTOR3& startPoint,D3DXVECTOR3& direction)
{
	//�ݹ����
	RayTreeDepth++;
	//����ݹ���� �򷵻ر���ɫ
	if(RayTreeDepth>5)
	{
		RayTreeDepth--;
		return D3DXCOLOR(0,0,0,255);

	}

//�ཻ����
D3DXVECTOR3 intersectPoint;
//���䷨����
D3DXVECTOR3 reflectDir;

//��õ�һ���͹����ཻ�����������Լ��ཻ���ꡢ���䷨����
int index	=	getFirstIntersectObj(startPoint,direction,intersectPoint,reflectDir);

//û���ཻ���ر���ɫ
if(index==-1)
{
	RayTreeDepth--;
	return D3DXCOLOR(0,0,0,255);
}

//������ɫ
D3DXCOLOR reflectColor(0,0,0,255);
reflectColor	=	RayTrace::trace(intersectPoint,reflectDir);

//���͸�� ��������ɫ
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

//��������ɫ
D3DXCOLOR ambientColor	=	D3DXCOLOR(22,22,22,255);

//��������ɫ
D3DXCOLOR diffuseColor(0,0,0,255);
//���淢���
D3DXCOLOR specularColor(0,0,0,255);


//�������������Ļ�
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





//������ɫ���
RayTreeDepth--;


return shader(reflectColor,refractColor,ambientColor,diffuseColor,specularColor,object[index])/(static_cast<float>(RayTreeDepth+1));

}

//��������ཻ�ĵ�һ��������� �Լ� ����� ��������
int RayTrace::getFirstIntersectObj(D3DXVECTOR3& startPoint,const D3DXVECTOR3& direction,D3DXVECTOR3& Point,D3DXVECTOR3& dir)
{	
	//�������
	int objectIndex = -1;
	//ֱ�߲������� �ڽ��㴦 ����ֵ ��ʼ��Ϊ�Ƚϴ����
	float len2	=	10000000;
	
	//�����������������󽻵�	
	for(int i=0;i<static_cast<int>(object.size());i++)
	{	
		//�������
		D3DXVECTOR3 intersectPoint	=	object[i]->getReflectPoint(startPoint,direction);
		
		//���������ڣ�����������t ��֮ǰ��t���бȽ� ������С��
		if(intersectPoint!=D3DXVECTOR3(50000,50000,50000))
		{	
			
			float t;		
	
			if(abs(direction.x)>1e-4)
				t = (intersectPoint.x-startPoint.x)/direction.x;
			else if(abs(direction.y)>1e-4)
				t = (intersectPoint.y-startPoint.y)/direction.y;
			else
				t = (intersectPoint.z-startPoint.z)/direction.z;
			//������󽻵��֮ǰ�ĵ���ӿ����ӵ� ����������
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
// ������ɫ�ļ���
D3DXCOLOR Raytracing::RayTrace::shader(const D3DXCOLOR& reflect,const D3DXCOLOR& refract,const D3DXCOLOR& ambient,const D3DXCOLOR& diffuse,const D3DXCOLOR& specular,Model* obj)
{

//������ɫ	
D3DXCOLOR reflectColor	=	reflect;
float reflectK	=1.0f;
//������ɫ
D3DXCOLOR refractColor	=	refract;
float refractK	=1.0f;
//��������ɫ
D3DXCOLOR ambientColor	=	ambient;
float ambientK	=1.0f;
	
//��������ɫ
D3DXCOLOR diffuseColor	=	diffuse;
float diffuseK	=1.3f;
//���淢���
D3DXCOLOR specularColor = specular;
float specularK	=0.4f;

bool reflectAble	=	obj->getIsReflectAble();
bool transparency	=	obj->getTrans();


//�ɷ���Ļ�
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

// �����͹�Դ֮���Ƿ�û�б����嵵ס
//����ֵ	true��û�б����嵵ס	false:�����嵵ס
int Raytracing::RayTrace::isReachLight(const D3DXVECTOR3& colorPoint, const D3DXVECTOR3& lightPoint)
{
	D3DXVECTOR3 lightDir	=	lightPoint-colorPoint;
	
	//��Դ��������������֮�䲻͸���������
	int solidNum	=	0;
	//��Դ��������������֮��͸���������
	int transNum=0;

	vector<Model*>::iterator iter;
	for(iter=object.begin();iter!=object.end();iter++)
	{
		D3DXVECTOR3 intersectPoint	=	(*iter)->getReflectPoint(colorPoint,lightDir);
		
		//��ɫ��Ϊ��ʼ�㣬ָ���Դ�������������н���
		if(intersectPoint!=D3DXVECTOR3(50000,50000,50000))
		{	
			
			//ͨ�������������жϽ����� ��Դ����ɫ��֮�仹��֮��
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






//����������;��淴��
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
