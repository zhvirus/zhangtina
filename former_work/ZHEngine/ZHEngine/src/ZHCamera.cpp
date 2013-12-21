#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHCamera.h"

namespace ZH
{

Camera::Camera()
{
	_cameraType = AIRCRAFT;

	_pos   = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	_look  = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	
	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_up,&_up);
	D3DXVec3Normalize(&_look,&_look);

	m_fovy	=	ZH::Math::PI/4.0f;
	m_aspect	=	1.333f;
	m_near	=	0.5f;
	m_far	=	20000.0f;

	//创建视景体四个侧面
	for(int i=0;i<4;i++)
	{
		D3DXPLANE* _temp	=	new D3DXPLANE;
		m_viewFrustum.push_back(_temp);
	}


}

Camera::Camera(Camera::CameraTYPE type)
{

	_cameraType	=	type;
	_pos   = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	_look  = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_up,&_up);
	D3DXVec3Normalize(&_look,&_look);

	m_fovy	=	ZH::Math::PI/4.0f;
	m_aspect	=	1.333f;
	m_near	=	0.5f;
	m_far	=	20000.0f;

	//创建视景体四个侧面
	for(int i=0;i<4;i++)
	{
		D3DXPLANE* _temp	=	new D3DXPLANE;
		m_viewFrustum.push_back(_temp);
	}


}

Camera::~Camera()
{
	std::vector<D3DXPLANE*>::iterator	ite;
	for(ite=m_viewFrustum.begin();ite<m_viewFrustum.end();ite++)
	{
		SAFE_DELETE(*ite);
	}

}



void Camera::setCameraType(CameraTYPE cameraType)
{

	_cameraType	=	cameraType;

}


void Camera::getPosition(D3DXVECTOR3& pos) const
{
pos	=	_pos;
}

void Camera::setPosition(const D3DXVECTOR3& pos)
{
	_pos	=	pos;
	_updateViewPlanes();
}

void Camera::getRight(D3DXVECTOR3& right) const
{
	right	=	_right;
}

void Camera::getUp(D3DXVECTOR3& up) const
{
	up	=	_up;

}

void Camera::getLook(D3DXVECTOR3& look) const
{
	look	=	_look;

}


void Camera::LookTrans(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _look * units;

	_updateViewPlanes();

}



void Camera::UpTrans(float units)
{
	// move only on y-axis for land object
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;

	_updateViewPlanes();
}



void Camera::RightTrans(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _right * units;

	_updateViewPlanes();
}



void Camera::LookRotate(float angle)
{
// only roll for aircraft type
	if( _cameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
	}
	
	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_up,&_up);
	_updateViewPlanes();
}


void Camera::UpRotate(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);

	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_look,&_look);

	_updateViewPlanes();
}


void Camera::RightRotate(float angle)
{

	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right,	angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);

	D3DXVec3Normalize(&_look,&_look);
	D3DXVec3Normalize(&_up,&_up);

	_updateViewPlanes();
}



const Matrix4& Camera::getViewMatrix()
{

	// Keep camera's axes orthogonal to eachother
	//(because LANDOBJECT sometimes make camera's axes not orthogonal)
	//D3DXVec3Normalize(&_look, &_look);

	//D3DXVec3Cross(&_up, &_look, &_right);
	//D3DXVec3Normalize(&_up, &_up);

	//D3DXVec3Cross(&_right, &_up, &_look);
	//D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	_v[0][0]	=_right.x;	_v[0][1]	=_up.x;	_v[0][2]	=_look.x;	_v[0][3]=0.0f;
	_v[1][0]	=_right.y;	_v[1][1]	=_up.y;	_v[1][2]	=_look.y;	_v[1][3]=0.0f;
	_v[2][0]	=_right.z;	_v[2][1]	=_up.z;	_v[2][2]=_look.z;	_v[2][3]=0.0f;
	_v[3][0]=			x;	_v[3][1]=		y;	_v[3][2]=		z;		_v[3][3]=1.0f;


	//V(0,0) = _right.x; V(0, 1) = _up.x; V(0, 2) = _look.x; V(0, 3) = 0.0f;
	//V(1,0) = _right.y; V(1, 1) = _up.y; V(1, 2) = _look.y; V(1, 3) = 0.0f;
	//V(2,0) = _right.z; V(2, 1) = _up.z; V(2, 2) = _look.z; V(2, 3) = 0.0f;
	//V(3,0) = x;        V(3, 1) = y;     V(3, 2) = z;       V(3, 3) = 1.0f;

	return _v;

}

void Camera::setViewFrustumParam(float _fovy, float _aspect, float _near , float _far)
{
	m_fovy		=	_fovy;
	m_aspect	=	_aspect;
	m_near		=	_near;
	m_far		=	_far;
	_updateViewPlanes();
}

void Camera::setProjMatrix(const Matrix4& inProjM)
{
	_p	=	inProjM;

}

void Camera::initialize(const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upVector)
{
	_pos	=	eyePos;
	_look	=	lookAtPos	-	eyePos;
	D3DXVec3Normalize(&_look,&_look);
	//...
	//没写完


}

//更新视景体的四个侧面
void Camera::_updateViewPlanes()
{	
	Pn0	=	_pos + _look * m_near;
	Pf0	=	_pos + _look * m_far;
	
	Hhalf	=	m_near * tan(m_fovy);
	P3		=	Pn0	+	_up * Hhalf + _right * Hhalf * m_aspect;
	P2		=	P3		-	_right * Hhalf * m_aspect * 2;
	P1		=	P2		-	_up * Hhalf * 2;
	
	Hhalf_far	=	Hhalf * m_far/m_near;
	P5		=	Pf0	+	_up * Hhalf_far + _right * Hhalf_far * m_aspect;
	P6		=	P5		-	_up * Hhalf_far * 2;
	P4		=	P6		-	_right * Hhalf_far * 2 * m_aspect ;

	_makePlaneFromPoints(m_viewFrustum[0], P6, P5, P3);
	_makePlaneFromPoints(m_viewFrustum[1], P2, P3, P5);
	_makePlaneFromPoints(m_viewFrustum[2], P4, P1, P2);
	_makePlaneFromPoints(m_viewFrustum[3], P1, P4, P6);

}

void Camera::_makePlaneFromPoints(D3DXPLANE* _plane, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3)
{

	D3DXVECTOR3 v1,v2;
	v1	=	p3	-	p2;
	v2	=	p1-	p2;
	D3DXVec3Cross(&v1, &v1, &v2);
	D3DXVec3Normalize(&v1,&v1);
	_plane->a	=	v1.x;
	_plane->b	=	v1.y;
	_plane->c	=	v1.z;
	_plane->d =	-(_plane->a * p2.x + _plane->b * p2.y + _plane->c * p2.z);
}




bool Camera::ifVisible(const D3DXVECTOR3& _center, float _r)
{
	float t_distance;
	int count	=	0;

	if(_ifPointInViewFrustum(_center))
		return true;

	for(int i=0;i<4;i++)
	{
		t_distance	=	_PointToPlaneDistance(*(m_viewFrustum[i]), _center);
		if(t_distance>= -_r)
		{
			count++;
		}
	}

	if(count<4)
		return false;

	return true;
}

float Camera::_PointToPlaneDistance(const D3DXPLANE& _plane, const D3DXVECTOR3& _point)
{
	return (_plane.a * _point.x +_plane.b * _point.y +_plane.c * _point.z +_plane.d)
				/(sqrt(_plane.a * _plane.a +_plane.b * _plane.b + _plane.c * _plane.c));

}

bool Camera::_ifPointInViewFrustum(const D3DXVECTOR3& inPoint)
{	

	D3DXVECTOR3 t_vec;
	D3DXVECTOR3 t_plane_normal;
	

	t_plane_normal.x	=	(m_viewFrustum[0])->a;
	t_plane_normal.y	=	(m_viewFrustum[0])->b;
	t_plane_normal.z	=	(m_viewFrustum[0])->c;
	t_vec		=	inPoint	-	P3;
	if(D3DXVec3Dot(&t_vec, &t_plane_normal)<0)
		return false;


	t_plane_normal.x	=	(m_viewFrustum[1])->a;
	t_plane_normal.y	=	(m_viewFrustum[1])->b;
	t_plane_normal.z	=	(m_viewFrustum[1])->c;
	t_vec		=	inPoint	-	P5;
	if(D3DXVec3Dot(&t_vec, &t_plane_normal)<0)
		return false;

	t_plane_normal.x	=	(m_viewFrustum[2])->a;
	t_plane_normal.y	=	(m_viewFrustum[2])->b;
	t_plane_normal.z	=	(m_viewFrustum[2])->c;
	t_vec		=	inPoint	-	P2;
	if(D3DXVec3Dot(&t_vec, &t_plane_normal)<0)
		return false;

	t_plane_normal.x	=	(m_viewFrustum[3])->a;
	t_plane_normal.y	=	(m_viewFrustum[3])->b;
	t_plane_normal.z	=	(m_viewFrustum[3])->c;
	t_vec		=	inPoint	-	P4;
	if(D3DXVec3Dot(&t_vec, &t_plane_normal)<0)
		return false;


	return true;
}

}