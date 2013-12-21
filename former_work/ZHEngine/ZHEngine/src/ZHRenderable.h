#pragma once
#include "ZHRenderDecl.h"

namespace ZH
{

//****************************************
//class name:		Renderable
//author:				Hui Zhang
//latest modified:	2007.4.2
//description:			
//****************************************

//Renderable是mesh\变换矩阵\是否可见
//
//
class ZH_DLL Renderable
{
public:
	explicit Renderable();
	explicit Renderable(Mesh* mesh);
	~Renderable();
	inline void setWorldMatrix(const Matrix4& mat){_world	=	mat;}
	inline void setViewMatrix(const Matrix4& mat){_view	=	mat;}
	inline void setProjMatrix(const Matrix4& mat){_proj	=	mat;}
	inline const Matrix4& getWorldMatrix()const{return _world;}
	inline const Matrix4& getViewMatrix()const{return _view;}
	inline const Matrix4& getProjMatrix()const{return _proj;}
	
	inline Mesh* getMesh(){return _mesh;}
	void setMesh(Mesh* mesh);

	inline bool getVisile()const{return _visible;}
	inline void setVisible(bool flag){_visible	=	flag;}
private:
	bool _visible;
	Mesh* _mesh;
	Matrix4 _world;
	Matrix4 _view;
	Matrix4 _proj;

};

}