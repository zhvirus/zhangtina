#pragma once

#include	"Model.h"
using namespace std;



namespace Raytracing
{
class Polygon :
	public Model
{
private:
		vector<D3DXVECTOR3> vertexSet;			//����ζ�������
		D3DXVECTOR3 normal;							//����η�����
		D3DXVECTOR3 material;							//����β���
		float	refractRate;										//�����������
		D3DXVECTOR4 planeEquPara;					//ƽ�淽�̲��� Ax+By+Cz+D=0
		Bitmap *texture;										//����
		bool	ifHasTexture;									//�Ƿ�������
		CString textureName;								//����ͼƬ�ļ���
public:
		int width,height;										//����ߴ�
		
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
	//���ض���ζ�����Ŀ
	int getVertexNum();
	//����ͼƬ��Դ
	bool setTexture(Bitmap *in);
private:
	//�ж϶�ά�ռ��߶κ������Ƿ��ཻ
	bool isIntersect(float x0,float y0,float x1,float y1,float px,float py,float u,float v);

public:
	virtual ~Polygon(void);

public:

	//*************************************************************
	//��������	getReflectPoint
	//���ܣ�		��ռ����ߺͼ������  ���佻��
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					IntersectPoint����
	//����ֵ��	
	//*************************************************************

	D3DXVECTOR3 getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction);
	
	//*************************************************************
	//��������	getReflectDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����startPoint�ռ�������ʼ��
	//					intersectPoint�ռ��������򽻵�
	//					reflectDir����ʸ��
	//����ֵ��	
	//*************************************************************

	D3DXVECTOR3 getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);
	
	//*************************************************************
	//��������	getRefractPoint
	//���ܣ�		��ռ����ߺͼ������  ��������
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					refractPoint��������
	//����ֵ��	
	//*************************************************************

	D3DXVECTOR3 getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);

	//*************************************************************
	//��������	getRefractDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					
	//����ֵ��	refractDir����ʸ��
	//*************************************************************

	D3DXVECTOR3 getRefractDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint);

public:
	bool getUV(int& U, int& V,const D3DXVECTOR3& inPoint);
	};
}