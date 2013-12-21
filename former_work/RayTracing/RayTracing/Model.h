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
		Type modelType;														//ģ������
		bool isTransparency;												//�����Ƿ�͸��
		bool isReflectAble;													//�Ƿ���
	public:
		Type getModelType(void);
	public:
		bool getTrans(){return isTransparency;}
		bool getIsReflectAble(){return isReflectAble;}
		bool hasTexture(){return false;}
		Bitmap* getTex(){return NULL;}
		bool getUV(int& U, int& V,const D3DXVECTOR3& inPoint){return false;}
	//*************************************************************
	//��������	getReflectPoint
	//���ܣ�		��ռ����ߺͼ������  ���佻��
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					IntersectPoint����
	//����ֵ��	
	//*************************************************************

	virtual D3DXVECTOR3 getReflectPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &direction)=0;
	
	//*************************************************************
	//��������	getReflectDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����startPoint�ռ�������ʼ��
	//					intersectPoint�ռ��������򽻵�
	//					reflectDir����ʸ��
	//����ֵ��	
	//*************************************************************

	virtual D3DXVECTOR3 getReflectDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)=0;
	
	//*************************************************************
	//��������	getRefractPoint
	//���ܣ�		��ռ����ߺͼ������  ��������
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					refractPoint��������
	//����ֵ��	
	//*************************************************************

	virtual D3DXVECTOR3 getRefractPoint(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)=0;

	//*************************************************************
	//��������	getRefractDir
	//���ܣ�		��ռ����ߺͼ������  ���䷽��ʸ��
	//����˵����startPoint�ռ�������ʼ��
	//					direction�ռ����߷���ʸ��
	//					
	//����ֵ��	refractDir����ʸ��
	//*************************************************************

	virtual D3DXVECTOR3 getRefractDir(const D3DXVECTOR3 &startPoint,const D3DXVECTOR3 &intersectPoint)=0;

	virtual CString getString(void)=0;
	virtual D3DXVECTOR3 getNormal(const D3DXVECTOR3& surfacePoint)=0;
	virtual D3DXVECTOR3 getMaterial()=0;
	bool isTrans(){return isTransparency;}

	};

}