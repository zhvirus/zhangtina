#ifndef _ZH_DLL
#define _ZH_DLL
#endif


#include "ZHRenderable.h"
#include "ZHMesh.h"

namespace ZH
{

	Renderable::Renderable():
	_mesh(NULL),
	_visible(true)
	{
		if(_mesh==NULL)
		{
			_mesh	=	new Mesh();

		}
	}

	Renderable::Renderable(ZH::Mesh *mesh):
	_mesh(NULL),
	_visible(true)
	{
		_mesh	=	mesh;
		_world	=	Matrix4::IDENTITY;
		_view		=	Matrix4::IDENTITY;
		_proj		=	Matrix4::IDENTITY;


	}

	Renderable::~Renderable()
	{
		if(_mesh!=NULL)
		{
			SAFE_DELETE(_mesh);

		}

	}

	void Renderable::setMesh(Mesh* mesh)
	{
		SAFE_DELETE(_mesh);

		_mesh	=	mesh;


	}




}