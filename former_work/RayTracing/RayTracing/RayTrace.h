#pragma once

#include	"Model.h"
#include	"Light.h"
using namespace std;



namespace Raytracing
{
class RayTrace
{
public:
	RayTrace(void);
public:
	virtual ~RayTrace(void);

public:
	bool addObject(Model*);
	bool deleteObject(const int&);
	bool deleteAllObject();
	
	bool addLight(Light*);
	bool deleteLight(const int&);
	bool deleteAllLights();

	int loadObjectFromFile(const string&);
	int saveObjectToFile(const string&);
	
public:
	int RayTreeDepth;
	
	D3DXCOLOR trace(D3DXVECTOR3& startPoint,D3DXVECTOR3& direction);
	int getFirstIntersectObj(D3DXVECTOR3& startPoint,const D3DXVECTOR3& direction,D3DXVECTOR3& Point,D3DXVECTOR3& dir);

	//物体对象列表
	vector<Model*>	object;
	vector<Light*>	light;


	// 最终颜色的计算
	D3DXCOLOR shader(const D3DXCOLOR& reflect,const D3DXCOLOR& refract,const D3DXCOLOR& ambient,const D3DXCOLOR& diffuse,const D3DXCOLOR& specular,Model* obj);
public:
	// 计算点和光源之间是否有被物体档住
	int isReachLight(const D3DXVECTOR3& colorPoint, const D3DXVECTOR3& lightPoint);
public:
	int diffSpecColor(D3DXCOLOR& diff, D3DXCOLOR& spec, D3DXVECTOR3 colorPoint, D3DXVECTOR3 normal, D3DXVECTOR3 material, D3DXVECTOR3 eyeDirection);
};
}