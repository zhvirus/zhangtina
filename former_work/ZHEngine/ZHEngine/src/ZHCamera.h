#pragma once
#include "ZHRenderDecl.h"

namespace ZH
{

class ZH_DLL Camera
{
public:
	enum CameraTYPE{LANDOBJECT,AIRCRAFT};
	Camera();
	Camera(CameraTYPE type);
	~Camera();
	
	void LookTrans(float units);	//forward/back
	void UpTrans(float units);	//up/down
	void RightTrans(float units);//left/right

	void LookRotate(float angle);//rotate round look vector
	void UpRotate(float angle);	//rotate round up vector
	void RightRotate(float angle);//rotate round right vector

	void initialize(const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upVector);

	const Matrix4& getViewMatrix();
	inline const Matrix4& getProjMatrix(){return _p;}
	void setProjMatrix(const Matrix4& inProjM);
	void setViewFrustumParam(float _fovy, float _aspect, float _near,float _far);
	void setCameraType(CameraTYPE cameraType);
	void getPosition(D3DXVECTOR3& pos) const;
	void setPosition(const D3DXVECTOR3& pos);

	void getRight(D3DXVECTOR3& right) const;
	void getUp(D3DXVECTOR3& up) const;
	void getLook(D3DXVECTOR3& look) const;

	bool ifVisible(const D3DXVECTOR3& _center, float _r);


	CameraTYPE _cameraType;
	D3DXVECTOR3	_right;
	D3DXVECTOR3	_up;
	D3DXVECTOR3	_look;
	D3DXVECTOR3	_pos;

	Matrix4	_v;
	Matrix4  _p;

private:
	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;

private:
	D3DXVECTOR3	Pn0;
	D3DXVECTOR3	Pf0;
	float Hhalf;
	D3DXVECTOR3	P3	;
	D3DXVECTOR3	P2;
	D3DXVECTOR3	P1;
	float Hhalf_far;
	D3DXVECTOR3	P5;	
	D3DXVECTOR3	P6	;
	D3DXVECTOR3	P4	;
	std::vector<D3DXPLANE*> m_viewFrustum;

private:	
	void _updateViewPlanes();
	bool _ifPointInViewFrustum(const D3DXVECTOR3& inPoint);
	float _PointToPlaneDistance(const D3DXPLANE& _plane, const D3DXVECTOR3& _point);
	void _makePlaneFromPoints(D3DXPLANE* _plane, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3);
};


}