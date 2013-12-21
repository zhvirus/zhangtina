/*
* Internet Graphics Group, MSR Asia
* Microsoft Confidential.  All Rights Reserved.

* --------------------------------------------------------------------
* Permission to use, copy, or modify this software and its documentation
* for educational and research purposes only and without fee is hereby
* granted, provided that this copyright notice appear on all copies and
* supporting documentation.  For any other uses of this software, in
* original or modified form, including but not limited to distribution
* in whole or in part, specific prior permission must be obtained from
* Microsoft research and the authors.  These programs shall not be used,
* rewritten, or adapted as the basis of a commercial software or hardware
* product without first obtaining appropriate licenses from MSRCN.  MSRCN
* makes no representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
* --------------------------------------------------------------------
*
* Sept. 2001
*/


// BspTR.cpp: implementation of the CBspTR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\inc\\BspTR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CVec3 *CBspTriangle::m_vpVertices   = NULL;
CVec3 *CBspTriangle::m_vpNormals    = NULL;
CVec3 *CBspTriangle::m_vpTexCoords  = NULL;
CCol4 *CBspTriangle::m_cpBrightness = NULL;
CVec3 *CBspTriangle::m_vpTexAxisU   = NULL;


void  CBspTriangle::Set_static_values (CVec3 *pVrt, CVec3 *pNml,
									   CVec3 *pTex, CCol4 *pBri,
									   CVec3 *pAxisU)
{
	m_vpVertices   = pVrt;
	m_vpNormals    = pNml;
	m_vpTexCoords  = pTex;
	m_cpBrightness = pBri;
	m_vpTexAxisU   = pAxisU;
}

void  CBspTriangle::SetVerticesID  (int v0, int v1, int v2)
{
	m_nVrt[0] = v0;
	m_nVrt[1] = v1;
	m_nVrt[2] = v2;
}

void  CBspTriangle::SetNormalsID  (int n0, int n1, int n2)
{
	m_nNml[0] = n0;
	m_nNml[1] = n1;
	m_nNml[2] = n2;
}

void  CBspTriangle::SetTexCoordsID (int t0, int t1, int t2)
{
	m_nTex[0] = t0;
	m_nTex[1] = t1;
	m_nTex[2] = t2;
}
inline CVec3 CBspTriangle::_Vrt(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return m_vpVertices[m_nVrt[i]];
}
inline CVec3 CBspTriangle::_Nml(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return m_vpNormals[m_nNml[i]];
}
inline CVec3 CBspTriangle::_Tex(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return m_vpTexCoords[m_nTex[i]];
}
inline CCol4 CBspTriangle::_Bri(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return m_cpBrightness[m_nVrt[i]];
}
inline CVec3 CBspTriangle::_TexAxisU (int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return m_vpTexAxisU[m_nVrt[i]];
}
CVec3 *CBspTriangle::VrtPtr(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return &m_vpVertices[m_nVrt[i]];
}
CVec3 *CBspTriangle::NmlPtr(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return &m_vpNormals[m_nNml[i]];
}
CVec3 *CBspTriangle::TexPtr(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return &m_vpTexCoords[m_nTex[i]];
}
CVec3 *	CBspTriangle::TexAxisUPtr(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return &m_vpTexAxisU[m_nVrt[i]];
}
CCol4 *CBspTriangle::BriPtr(int i) const
{
	assert ( (i >= 0) && (i < 3) );
	return &m_cpBrightness[m_nVrt[i]];
}

float CBspTriangle::Calc_area()
{
	float a, b, c, s;

	a = (_Vrt(0) - _Vrt(1)).Length();
	b = (_Vrt(1) - _Vrt(2)).Length();
	c = (_Vrt(2) - _Vrt(0)).Length();
	s = ( a + b + c ) / 2;

	return (float)sqrt( s * (s - a) * (s - b) * (s - c));
}
void CBspTriangle::Calc_bounding_box()
{
	m_vMax = m_vMin = _Vrt(0);
	
	for (int i = 1; i < 3; i ++)
	{
		if (m_vMax[0] < _Vrt(i)[0]) m_vMax.x = _Vrt(i)[0];
		if (m_vMax[1] < _Vrt(i)[1]) m_vMax.y = _Vrt(i)[1];
		if (m_vMax[2] < _Vrt(i)[2]) m_vMax.z = _Vrt(i)[2];

		if (m_vMin[0] > _Vrt(i)[0]) m_vMin.x = _Vrt(i)[0];
		if (m_vMin[1] > _Vrt(i)[1]) m_vMin.y = _Vrt(i)[1];
		if (m_vMin[2] > _Vrt(i)[2]) m_vMin.z = _Vrt(i)[2];
	}
}

CVec3 CBspTriangle::Calc_normal()
{
	CVec3 v1 = _Vrt(2) - _Vrt(1);
	CVec3 v2 = _Vrt(0) - _Vrt(1);
	CVec3 vv = v1.Cross(v2);
	return vv.Normalized();
}


BOOL CBspTriangle::Ray_intersection_flat (TBspRay &ray, TBspCross &crs)
{
//	return TRUE;
	CVec3 p1p3, p2p3, ap3, ab;
	CVec3 m_vNml;
	float denometor, u, v, t;

	p1p3 = _Vrt(2) - _Vrt(0);
	p2p3 = _Vrt(2) - _Vrt(1);
	ap3  = _Vrt(2) - ray.vOrg;
	ab   = ray.vEnd  - ray.vOrg;

	denometor = BLEND_PRODUCT(p1p3, p2p3, ab);
	if( fabs(denometor)>1.0e-6f )
	{
		denometor = 1.f/denometor;
		u = BLEND_PRODUCT( ap3, p2p3, ab ) * denometor;
		v = BLEND_PRODUCT(p1p3,  ap3, ab ) * denometor;
		t = BLEND_PRODUCT(p1p3, p2p3, ap3) * denometor;

		if (UNIT_VALUE(u) && UNIT_VALUE(v) && UNIT_VALUE(u+v) && UNIT_VALUE(t))
		{
			crs.vPos  = ray.vOrg + t*ab;
			crs.fDist = t;
			crs.vNml  = p1p3.Cross(p2p3);
			crs.vNml.Normalize();
			crs.vUVW = CVec3 (u, v, 1-u-v);

			if (t > 1.0e-6f) return TRUE;
		}
	}
	return FALSE;
}

// Ray_intersection_smooth
BOOL CBspTriangle::Ray_intersection_smooth (TBspRay &ray, TBspCross &crs)
{
	CVec3	p1p3, p2p3, ap3, ab;
	CVec3	m_vNml;
	float	denometor, u, v, t;

/*	if (((ray.vDir.Dot(_Nml(0))) > 0.1f) &&
		((ray.vDir.Dot(_Nml(1))) > 0.1f) &&
		((ray.vDir.Dot(_Nml(2))) > 0.1f) )
		return FALSE;
*/
	p1p3 = _Vrt(2) - _Vrt(0);
	p2p3 = _Vrt(2) - _Vrt(1);
	ap3  = _Vrt(2) - ray.vOrg;
	ab   = ray.vEnd - ray.vOrg;

	/*********************************************************
	*
	*               p2
	*              /\
	*             /  \
	*            /    \
	*           /      \
	*          /   _b   \
	*         /    /|    \
	*       |/    *       \
	*       /-   /         \
	*     p3<---/----------p1
	*       \  /
	*        \/
	*        a
	*
	*
	*     (1-t)*a + t*b = p = u*p1 + v*p2 + (1-u-v)*p3	 *
	*   then    u*p1p3 + v*p2p3 + t*ab = ap3		 *
	*							 *
	*********************************************************/

	denometor = BLEND_PRODUCT(p1p3, p2p3, ab);
	if( fabs(denometor)>1.0e-6f )
	{
		denometor = 1.f/denometor;
		u = BLEND_PRODUCT( ap3, p2p3, ab ) * denometor;
		v = BLEND_PRODUCT(p1p3,  ap3, ab ) * denometor;
		t = BLEND_PRODUCT(p1p3, p2p3, ap3) * denometor;

		if( UNIT_VALUE(u) && UNIT_VALUE(v) && UNIT_VALUE(u+v)
			&& UNIT_VALUE(t) )
		{
			float w = 1 - u - v;
			crs.vNml = u * _Nml(0) + v * _Nml(1) + w * _Nml(2);
			crs.vNml.Normalize();
			if ( (ray.vDir.Dot(crs.vNml)) >= 0.f) return FALSE;

			crs.vPos = ray.vOrg + t * ab;
			crs.fDist    = t;
			crs.vTex = u*_Tex(0) + v*_Tex(1) + w *_Tex(2);
			crs.vUVW = CVec3 (u, v, w);

			return TRUE;
		}
	}
	return FALSE;
}

// Ray_intersection_smooth
// Consider both front and back face
BOOL CBspTriangle::Ray_intersection_FB (TBspRay &ray, TBspCross &crs, BOOL &bFront)
{
	CVec3	p1p3, p2p3, ap3, ab;
	CVec3	m_vNml;
	float	denometor, u, v, t;

	p1p3 = _Vrt(2) - _Vrt(0);
	p2p3 = _Vrt(2) - _Vrt(1);
	ap3  = _Vrt(2) - ray.vOrg;
	ab   = ray.vEnd - ray.vOrg;

	denometor = BLEND_PRODUCT(p1p3, p2p3, ab);
	if( fabs(denometor)>1.0e-6f )
	{
		denometor = 1.f/denometor;
		u = BLEND_PRODUCT( ap3, p2p3, ab ) * denometor;
		v = BLEND_PRODUCT(p1p3,  ap3, ab ) * denometor;
		t = BLEND_PRODUCT(p1p3, p2p3, ap3) * denometor;

		if( UNIT_VALUE(u) && UNIT_VALUE(v) && UNIT_VALUE(u+v)
			&& (t > 1.0e-6f) && (t < 1.f))
		{
			float w = 1 - u - v;
			crs.vNml = u * _Nml(0) + v * _Nml(1) + w * _Nml(2);
			crs.vNml.Normalize();

			bFront = ((ray.vDir.Dot(crs.vNml)) >= 0.f) ? FALSE : TRUE;

			crs.vPos = ray.vOrg + t * ab;
			crs.fDist    = t;
			crs.vTex = u*_Tex(0) + v*_Tex(1) + w *_Tex(2);
			crs.vUVW = CVec3 (u, v, w);

			return TRUE;
		}
	}
	return FALSE;
}


// Ray_intersection_smooth
BOOL CBspTriangle::Ray_intersection_back_face (TBspRay &ray, TBspCross &crs,
											   const float &fTolerance)
{
	CVec3	p1p3, p2p3, ap3, ab;
	CVec3	m_vNml;
	float	denometor, u, v, t;

	p1p3 = _Vrt(2) - _Vrt(0);
	p2p3 = _Vrt(2) - _Vrt(1);
	ap3  = _Vrt(2) - ray.vOrg;
	ab   = ray.vEnd - ray.vOrg;

	denometor = BLEND_PRODUCT(p1p3, p2p3, ab);
	if( fabs(denometor)>1.0e-6f )
	{
		denometor = 1.f/denometor;
		u = BLEND_PRODUCT( ap3, p2p3, ab ) * denometor;
		v = BLEND_PRODUCT(p1p3,  ap3, ab ) * denometor;
		t = BLEND_PRODUCT(p1p3, p2p3, ap3) * denometor;

		if( UNIT_VALUE(u) && UNIT_VALUE(v) && UNIT_VALUE(u+v)
			&& UNIT_VALUE(t) )
		{
			float w = 1 - u - v;
			crs.vNml = u * _Nml(0) + v * _Nml(1) + w * _Nml(2);
			crs.vNml.Normalize();
			if ( (ray.vDir.Dot(crs.vNml)) < - fTolerance) return FALSE;

			crs.vPos = ray.vOrg + t * ab;
			crs.fDist    = t;
			crs.vTex = u*_Tex(0) + v*_Tex(1) + w *_Tex(2);
			crs.cCol = u*_Bri(0) + v*_Bri(1) + w *_Bri(2);
			crs.vUVW = CVec3 (u, v, w);

			return TRUE;
		}
	}
	return FALSE;
}












//-----------------------------------------------------------------------------
//	_ComputerRenderingParameters
// preparing the parameters for ray-tracing
//-----------------------------------------------------------------------------
void CCamera::ComputerRenderingParameters(CVec3 &daltaX, CVec3 &daltaY,
										 CVec3 &vLeftBottom)
{
	CVec3 vBC, vBX, vBY, vBZ;
	float	fHH, fHW;

	/////////////////////////////////////////////////////////////////////
	//        vBC
	//  +------+------+     +------------|------------+  ---
	//   \     |     /      |           /|\ vBY       |   |
	//    \    |    /       |            |        vBX |   |
	//     \   |   /        |            +----------->|  2*fHH
	//      \  |  /         |            | vBC        |   |
	// vangle\ | /          |            |            |   |
	//        \|/           +-------------------------+  ---
	//         *            |<-------- 2*fHW -------->|
	//        eye
	/////////////////////////////////////////////////////////////////////

	// the back plane center
	vBC = m_vCent - m_vEye;
	vBC.Normalize();
	vBC = vBC * m_fFar + m_vEye;

	// half size of the back plane
	fHH = m_fFar * (float)tan(m_fViewAngle*PI/180/2);
	fHW = fHH * m_nImageWidth/m_nImageHeight;

	// local frame of the back plane
	vBY = m_vUpright;
	vBZ = m_vEye - vBC;
	vBY.Normalize();
	vBZ.Normalize();
	vBX = vBY.Cross(vBZ);
	vBX.Normalize();

	vLeftBottom = vBC + vBY*(-fHH) + vBX*(-fHW);

	// Resolution is doubled for anti-aliasing
	daltaX = vBX * (2*fHW) / static_cast<float>(m_nImageWidth);		
	daltaY = vBY * (2*fHH) / static_cast<float>(m_nImageHeight);
}

void CCamera::ComputerRenderingParameters3x3(CVec3 &daltaX, CVec3 &daltaY,
										 CVec3 &vLeftBottom)
{
	ComputerRenderingParameters (daltaX, daltaY, vLeftBottom);
	daltaX /= 2;
	daltaY /= 2;
}









//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBspTR::CBspTR()
{
	m_vpVertices     = NULL;
	m_vpNormals      = NULL;
	m_vpTexCoords    = NULL;
	m_cpBrightness   = NULL;
	m_vpTexAxisU     = NULL;

	m_nVertices   = 0;
	m_nTexCoords  = 0;

	m_nTriangles = 0;
	m_pTriangles = NULL;
	m_nMaterials = 0;
	m_pMaterials = NULL;
	m_ptheTree = NULL;

	m_nTracingType = NORMAL_RAY_TRACING;

	m_bModelLoaded = FALSE;
}

CBspTR::~CBspTR()
{
	_DestroyScene();
}

void CBspTR::SetBgColor(CCol4 cBgColor)
{
	m_cBgColor = cBgColor;
}


/******************************************************************************
* The efficiency of the following c-cord can be increased by defining macros
* at appropriate places. For example, the _Leaf() function can be replaced by
* a corresponding macros. Another way to increase the code efficiency is by
* passing the address of structures instead of the structures themselves.
*
* note: There are a few standard link list functions that are not listed in
* the following code: FirstOfLinkList(), NextOfLinkList(), AddToLinkList(),
* and DuplicateLinkList()
******************************************************************************/

/*****************************************************************************
*	_AvailableNormal
******************************************************************************/
int CBspTR::_AvailableNormal( CVec3 *nml, CVec3 origin, CVec3 *v, int vnum )
{
	int	revert = FALSE;
	float	dpr;
	CVec3	vect;

	if( vnum <= 1 )	return FALSE;
	nml->Normalize();

	for( vnum--; vnum >= 0; vnum -- )
	{
		vect = v[vnum] - origin;
		dpr  = (*nml).Dot(vect);

		if( dpr < -1.0e-10 )	revert = TRUE;

		if( (dpr > 1.0e-10) )
		{
			if ( revert )
				return FALSE;
			else
			{
				*nml = (*nml) * (-1.f);
				revert = TRUE;
			}
		}
	}

	return TRUE;
}

/*****************************************************************************
*	_CBoxAtTheOtherSide
******************************************************************************/
int CBspTR::_CBoxAtTheOtherSide (CVec3 nml, CVec3 origin, CVec3 min, CVec3 max)
{
	int	i;
	CVec3	vct;
	CVec3	vrt[8];

	vrt[0].x = min.x;	vrt[0].y = min.y;	vrt[0].z = min.z;
	vrt[1].x = min.x;	vrt[1].y = min.y;	vrt[1].z = max.z;
	vrt[2].x = min.x;	vrt[2].y = max.y;	vrt[2].z = min.z;
	vrt[3].x = min.x;	vrt[3].y = max.y;	vrt[3].z = max.z;
	vrt[4].x = max.x;	vrt[4].y = min.y;	vrt[4].z = min.z;
	vrt[5].x = max.x;	vrt[5].y = min.y;	vrt[5].z = max.z;
	vrt[6].x = max.x;	vrt[6].y = max.y;	vrt[6].z = min.z;
	vrt[7].x = max.x;	vrt[7].y = max.y;	vrt[7].z = max.z;

	for( i = 0; i < 8; i ++ )
	{
		vct = vrt[i] - origin;
		if( ( nml.Dot(vct) ) <= 1.0e-20 )
			return FALSE;
	}

	return TRUE;
}

/*****************************************************************************
*	_IsObjectInNode
******************************************************************************/
int CBspTR::_IsObjectInNode( int ID, CVec3 min, CVec3 max )
{
	CVec3 nv[8], ov[8], nml;
	CBspTriangle *tri;

	tri = &m_pTriangles[ID];
	ov[0] = *tri->VrtPtr(0);
	ov[1] = *tri->VrtPtr(1);
	ov[2] = *tri->VrtPtr(2);

	nml = tri->Calc_normal();
	if( ( nml.Dot(min - ov[1]) ) < 1.0e-6f )
	nml = nml * (-1.f);

	if( _CBoxAtTheOtherSide( nml, ov[1], min, max ) )
		return FALSE;
	else	return TRUE;
}

/*****************************************************************************
* Stack oprations
******************************************************************************/
void CBspTR::_InitStack(StackPtr stack)
{
	stack->stack[0].node = NULL;
	stack->stackPtr = 1;
}

void CBspTR::_push(StackPtr stack, CBspNode * node, double min, double max)
{
	stack->stack[stack->stackPtr].node = node;
	stack->stack[stack->stackPtr].min  = min;
	stack->stack[stack->stackPtr].max  = max;
	(stack->stackPtr) ++;
}

void CBspTR::_pop(StackPtr stack, CBspNode * *node, double *min, double *max)
{
	(stack->stackPtr) --;
	*node = stack->stack[stack->stackPtr].node;
	*min  = stack->stack[stack->stackPtr].min;
	*max  = stack->stack[stack->stackPtr].max;
}

/*****************************************************************************
* Returns the distance between origin and plane, measured along the input
* direction. direction is a unit vector.
*
* Entry:
* 	plane		subdivision plane of current node
* 	origin		origin of the ray
* 	direction	direction of the ray, must be a unit vector
* Exit:
* 	return the distance between the origin and plane measured along
* 	the direction
* Note:
* 	there is a function for each of the three subdivision planes
******************************************************************************/
double CBspTR::_DistanceToDivisionPlane(CVec3 plane, TBspRay ray, int 
nDivisionPlane)
{
	switch (nDivisionPlane)
	{
	case 1: return ( (plane.x - ray.vOrg.x)/ray.vDir.x );
	case 2: return ( (plane.y - ray.vOrg.y)/ray.vDir.y );
	case 3: return ( (plane.z - ray.vOrg.z)/ray.vDir.z );
	default: return 0.0;
	}
}

/*****************************************************************************
* Determines which of the half space of the two children contains origin, 
* return the child as nearChild, the other as farChild
*
* Entry:
* 	currentNode	node currently working on
* 	origin		origin of the ray
* Exit:
* 	nearChild		node whose half plane contains the origin
* 	farChild		node whose half plane does not contain the origin
* Note:
* 	there is a function for each of the three subdividsion planes
******************************************************************************/
void CBspTR::_GetChildren(CBspNode * currentNode, CVec3 origin,
			 CBspNode * *nearChild, CBspNode * *farChild,
			 int nTheChildren)
{
	switch (nTheChildren)
	{
	case 1:	// X Children
		if ( currentNode->child[0]->max.x >= origin.x )
		{
			*nearChild = currentNode->child[0];
			*farChild  = currentNode->child[1];
		}
		else
		{
			*nearChild = currentNode->child[1];
			*farChild  = currentNode->child[0];
		}
		break;
	case 2: // Y Children
		if ( currentNode->child[0]->max.y >= origin.y )
		{
			*nearChild = currentNode->child[0];
			*farChild  = currentNode->child[1];
		}
		else
		{
			*nearChild = currentNode->child[1];
			*farChild  = currentNode->child[0];
		}
		break;
	case 3: // Z Children
		if ( currentNode->child[0]->max.z >= origin.z )
		{
			*nearChild = currentNode->child[0];
			*farChild  = currentNode->child[1];
		}
		else
		{
			*nearChild = currentNode->child[1];
			*farChild  = currentNode->child[0];
		}
		break;
	}
}

/*****************************************************************************
* Some miscellaneous supporting functions
******************************************************************************/
int CBspTR::_Leaf ( CBspNode * node )
{	return ( node->child[0] == NULL);	}

int CBspTR::_PointInNode ( CBspNode * node, CVec3 pt )
{
	return (( pt.x >= node->min.x ) && (pt.y >= node->min.y ) &&
		( pt.z >= node->min.z ) && (pt.x <= node->max.x ) &&
		( pt.y <= node->max.y ) && (pt.z <= node->max.z ) );
}

int CBspTR::_GeomInNode ( CBspNode * node, int ID )
{
	CVec3	*tmin, *tmax;

	tmin = m_pTriangles[ID].Bounding_box_min();
	tmax = m_pTriangles[ID].Bounding_box_max();

	if (node->min.x > tmax->x || node->max.x < tmin->x) return FALSE;
	if (node->min.y > tmax->y || node->max.y < tmin->y) return FALSE;
	if (node->min.z > tmax->z || node->max.z < tmin->z) return FALSE;

//	return( _IsObjectInNode( ID, node->min, node->max ) );
	return TRUE;
}

void CBspTR::_PointAtDistance ( TBspRay ray, double distance, CVec3 *pt )
{
	pt->x = (float)(ray.vOrg.x + distance * ray.vDir.x);
	pt->y = (float)(ray.vOrg.y + distance * ray.vDir.y);
	pt->z = (float)(ray.vOrg.z + distance * ray.vDir.z);
}

int CBspTR::_RayBoxIntersect ( TBspRay ray, CVec3 min, CVec3 max,
							 double *mind, double *maxd )
{
	int	srcnum = 0;
	float	t, dist[2];
	CVec3	cp;

	if ( ray.vDir.x != 0 )
	{
		/* 1. intersect with the minx plane */
		cp.x = min.x;
		t = (cp.x - ray.vOrg.x) / ray.vDir.x;
		cp.y = ray.vOrg.y + ray.vDir.y*t;
		cp.z = ray.vOrg.z + ray.vDir.z*t;
		if ( (cp.y>min.y) && (cp.y<max.y) &&
			 (cp.z>min.z) && (cp.z<max.z) )
		{
			dist[srcnum] = t;
			srcnum ++;
		}
		/* 2. intersect with the maxx plane */
		cp.x = max.x;
		t = (cp.x - ray.vOrg.x) / ray.vDir.x;
		cp.y = ray.vOrg.y + ray.vDir.y*t;
		cp.z = ray.vOrg.z + ray.vDir.z*t;
		if ( (cp.y>min.y) && (cp.y<max.y) &&
			 (cp.z>min.z) && (cp.z<max.z) )
		{
			dist[srcnum] = t;
			srcnum ++;
			if ( srcnum == 2 )
			{
				*mind = (dist[0]>dist[1])? dist[1] : dist[0];
				*maxd = (dist[0]>dist[1])? dist[0] : dist[1];
				return TRUE;
			}
		}
	}

	if ( ray.vDir.y != 0 )
	{
		/* 3. intersect with the miny plane */
		cp.y = min.y;
		t = (cp.y - ray.vOrg.y) / ray.vDir.y;
		cp.x = ray.vOrg.x + ray.vDir.x*t;
		cp.z = ray.vOrg.z + ray.vDir.z*t;
		if ( (cp.x>min.x) && (cp.x<max.x) &&
			 (cp.z>min.z) && (cp.z<max.z) )
		{
			dist[srcnum] = t;
			srcnum ++;
			if ( srcnum == 2 )
			{
				*mind = (dist[0]>dist[1])? dist[1] : dist[0];
				*maxd = (dist[0]>dist[1])? dist[0] : dist[1];
				return TRUE;
			}
		}
		/* 4. intersect with the maxy plane */
		cp.y = max.y;
		t = (cp.y - ray.vOrg.y) / ray.vDir.y;
		cp.x = ray.vOrg.x + ray.vDir.x*t;
		cp.z = ray.vOrg.z + ray.vDir.z*t;
		if ( (cp.x>min.x) && (cp.x<max.x) &&
			 (cp.z>min.z) && (cp.z<max.z) )
		{
			dist[srcnum] = t;
			srcnum ++;
			if ( srcnum == 2 )
			{
				*mind = (dist[0]>dist[1])? dist[1] : dist[0];
				*maxd = (dist[0]>dist[1])? dist[0] : dist[1];
				return TRUE;
			}
		}
	}

	if ( ray.vDir.z != 0 )
	{
		/* 5. intersect with the minz plane */
		cp.z = min.z;
		t = (cp.z - ray.vOrg.z) / ray.vDir.z;
		cp.x = ray.vOrg.x + ray.vDir.x*t;
		cp.y = ray.vOrg.y + ray.vDir.y*t;
		if ( (cp.x>min.x) && (cp.x<max.x) &&
			 (cp.y>min.y) && (cp.y<max.y) )
		{
			dist[srcnum] = t;
			srcnum ++;
			if ( srcnum == 2 )
			{
				*mind = (dist[0]>dist[1])? dist[1] : dist[0];
				*maxd = (dist[0]>dist[1])? dist[0] : dist[1];
				return TRUE;
			}
		}
		/* 6. intersect with the maxz plane */
		cp.z = max.z;
		t = (cp.z - ray.vOrg.z) / ray.vDir.z;
		cp.x = ray.vOrg.x + ray.vDir.x*t;
		cp.y = ray.vOrg.y + ray.vDir.y*t;
		if ( (cp.x>min.x) && (cp.x<max.x) &&
			 (cp.y>min.y) && (cp.y<max.y) )
		{
			dist[srcnum] = t;
			srcnum ++;
			if ( srcnum == 2 )
			{
				*mind = (dist[0]>dist[1])? dist[1] : dist[0];
				*maxd = (dist[0]>dist[1])? dist[0] : dist[1];
				return TRUE;
			}
		}
	}

	return FALSE;
}



/*****************************************************************************
* Traverses ray through BSPTree and intersects ray with all of the objects
* along the way. Returns the closest intersection distance and the intersecting
* Object if there is one.
*
* Entry:
* 	ray		the ray being traced
* Exit:
* 	cross		the first object that intersects the ray
* 	telID		ID of the intersecting texel
******************************************************************************/
BOOL CBspTR::_RayTreeIntersect ( TBspRay &ray, TBspCross &crs, int &ID)
{
	int		i, found;//, tested, inserted, oppc;
	int		*npTID;
	StackPtr	stack;
	CBspNode	*currentNode, *nearChild, *farChild;
	double		dist, min, max;
	TBspCross		tmpcrs;
	CVec3		p;

	/*
	 * test if the whole BSP tree is missed by the input ray
	 */
	if (!_RayBoxIntersect (ray, m_ptheTree->min, m_ptheTree->max, &min, &max))
		return	FALSE;

	stack = (StackPtr)malloc(sizeof(Stack));
	_InitStack(stack);
	crs.fDist = 1.0e8f;
	currentNode = m_ptheTree->root;

	int nBasePatch = ID;

	while ( currentNode != NULL )
	{
		while ( !( _Leaf(currentNode) ) )
		{
			dist = _DistanceToDivisionPlane(currentNode->child[0]->max,
						ray, currentNode->nDivisionPlane );
			_GetChildren( currentNode, ray.vOrg, &nearChild, &farChild,
						currentNode->nDivisionPlane);
			if ( (dist>max) || (dist<0) )
				currentNode = nearChild;
			else if ( dist<min )
				currentNode = farChild;
			else
			{
				_push( stack, farChild, dist, max );
				currentNode = nearChild;
				max = dist;
			}
		}

		npTID = currentNode->pMembers->objlnk;
		for ( i = 0; i < currentNode->pMembers->length; i ++ )
		{
			if (npTID[i] == nBasePatch) continue;

			if (!m_pTriangles[npTID[i]].Ray_intersection_smooth(ray, tmpcrs))
				continue;
			if ( !_PointInNode(currentNode, tmpcrs.vPos) ) continue;

			if ( tmpcrs.fDist < crs.fDist )
			{
				found = TRUE;
				crs   = tmpcrs;
				ID    = npTID[i];
			}
		}

		if ( crs.fDist < 1.f )
		{
			free ( stack );
			return TRUE;
		}
		_pop( stack, &currentNode, &min, &max );
	}

	free ( stack );
	return FALSE;
}


/*********************************************************************************
 *	_RayTreeIntersect_back_face
 *********************************************************************************/
BOOL CBspTR::_RayTreeIntersect_back_face ( TBspRay &ray, TBspCross &crs,
										  int &ID, const float &fTolerance)
{
	int		i, found;//, tested, inserted, oppc;
	int		*npTID;
	StackPtr	stack;
	CBspNode	*currentNode, *nearChild, *farChild;
	double		dist, min, max;
	TBspCross		tmpcrs;
	CVec3		p;

	/*
	 * test if the whole BSP tree is missed by the input ray
	 */
	if (!_RayBoxIntersect (ray, m_ptheTree->min, m_ptheTree->max, &min, &max))
		return	FALSE;

	stack = (StackPtr)malloc(sizeof(Stack));
	_InitStack(stack);
	crs.fDist = 1.0e8f;
	currentNode = m_ptheTree->root;

	int nBasePatch = ID;

	while ( currentNode != NULL )
	{
		while ( !( _Leaf(currentNode) ) )
		{
			dist = _DistanceToDivisionPlane(currentNode->child[0]->max,
						ray, currentNode->nDivisionPlane );
			_GetChildren( currentNode, ray.vOrg, &nearChild, &farChild,
						currentNode->nDivisionPlane);
			if ( (dist>max) || (dist<0) )
				currentNode = nearChild;
			else if ( dist<min )
				currentNode = farChild;
			else
			{
				_push( stack, farChild, dist, max );
				currentNode = nearChild;
				max = dist;
			}
		}

		npTID = currentNode->pMembers->objlnk;
		for ( i = 0; i < currentNode->pMembers->length; i ++ )
		{
			if (npTID[i] == nBasePatch) continue;

			if (!m_pTriangles[npTID[i]]. Ray_intersection_back_face(ray, tmpcrs, fTolerance))
				continue;
			if ( !_PointInNode(currentNode, tmpcrs.vPos) ) continue;

			if ( tmpcrs.fDist < crs.fDist )
			{
				found = TRUE;
				crs   = tmpcrs;
				ID    = npTID[i];
			}
		}

		if ( crs.fDist < 1.f )
		{
			free ( stack );
			return TRUE;
		}
		_pop( stack, &currentNode, &min, &max );
	}

	free ( stack );
	return FALSE;
}



/*********************************************************************************
 *	RayTreeIntersectForShadowTest
 *********************************************************************************/
BOOL CBspTR::_RayTreeIntersectForShadowTest(TBspRay &ray, TBspCross &crs, int ID)
{
	int       i, found;//, tested, inserted, oppc;
	int      *npTID;
	StackPtr  stack;
	CBspNode *currentNode, *nearChild, *farChild;
	double    dist, min, max;
	TBspCross tmpcrs;
	CVec3     p;

	/*
	 * test if the whole BSP tree is missed by the input ray
	 */
	if (!_RayBoxIntersect (ray, m_ptheTree->min, m_ptheTree->max, &min, &max))
		return	FALSE;

	stack = (StackPtr)malloc(sizeof(Stack));
	_InitStack(stack);
	crs.fDist = 1.0e8f;
	currentNode = m_ptheTree->root;

	int nBasePatch = ID;

	while ( currentNode != NULL )
	{
		while ( !( _Leaf(currentNode) ) )
		{
			dist = _DistanceToDivisionPlane(currentNode->child[0]->max, ray,
										currentNode->nDivisionPlane );
			_GetChildren (currentNode, ray.vOrg, &nearChild, &farChild,
						currentNode->nDivisionPlane);
			if ( (dist>max) || (dist<0) )
				currentNode = nearChild;
			else if ( dist<min )
				currentNode = farChild;
			else
			{
				_push( stack, farChild, dist, max );
				currentNode = nearChild;
				max = dist;
			}
		}

		npTID = currentNode->pMembers->objlnk;
		for ( i = 0; i < currentNode->pMembers->length; i ++ )
		{
			if (npTID[i] == nBasePatch) continue;

			if (!m_pTriangles[npTID[i]].Ray_intersection_smooth(ray, tmpcrs))
				continue;

			if (tmpcrs.vNml.Dot(ray.vDir) > 0.f) continue; //This is for the special purpose of low resolution smooth cuver surface.
			if ( tmpcrs.fDist < 0.f ) continue;
			if ((tmpcrs.fDist < crs.fDist) && (npTID[i] != ID) )
			{
				found = TRUE;
				crs   = tmpcrs;
			}
		}

		if ( crs.fDist < 1.f )
		{
			free ( stack );
			return TRUE;
		}
		_pop( stack, &currentNode, &min, &max );
	}

	free ( stack );
	return FALSE;
}

/*****************************************************************************
* Builds the BSP tree by subdividing along the center of x, y, or z bounds, 
* one each time this function is called. This function calls itself
* recursively until either the tree is deeper than MaxDepth or all of the tree
* leaves contain less than MaxListLength of objects.
*
* Entry:
* 	node		node currently working on
* 	depth		current tree depth
* 	MaxDepth	max allowed tree depth
* 	MaxListLength	man allowed object list length of a leave node
* 	axis		subdivision axis for the children of node (1:x, 2:y, 3:z)
******************************************************************************/
void CBspTR::_Subdivide(CBspNode * node, int depth, int MaxDepth,
					   int MaxListLength, int axis )
{
	int	i, j, nextAxis,	length, *objlnk;
//	BspObjLink	*ObjPtr, *AddObjPtr;

	node->child[0] = node->child[1] = NULL;

	if ( (node->pMembers->length > MaxListLength) && (depth < MaxDepth) )
	{
		for ( i=0; i<2; i++ )
		{
			node->child[i] = new CBspNode;
			node->child[i]->min = node->min;
			node->child[i]->max = node->max;

			if ( axis == 1 )
			{
				/* current subdivision plane is x */
				node->child[i]->min.x = 
					node->min[0] + .5f * i * (node->max[0] - node->min[0]);
				node->child[i]->max.x =
					node->min[0] + .5f * (i+1) * (node->max[0] - node->min[0]);

				/* child subdivision plane will be y */
				nextAxis = 2;
				node->child[i]->nDivisionPlane = nextAxis;
			}
			else if ( axis == 2 )
			{
				/* current subdivision plane is y */
				node->child[i]->min.y =
					node->min[1] + .5f * i * (node->max[1] - node->min[1]);
				node->child[i]->max.y =
					node->min[1] + .5f * (i+1) * (node->max[1] - node->min[1]);

				/* child subdivision plane will be z */
				nextAxis = 3;
				node->child[i]->nDivisionPlane = nextAxis;
			}
			else
			{
				/* current subdivision plane is z */
				node->child[i]->min.z =
					node->min[2] + .5f * i * (node->max[2] - node->min[2]);
				node->child[i]->max.z =
					node->min[2] + .5f * (i+1) * (node->max[2] - node->min[2]);

				/* child subdivision plane will be x */
				nextAxis = 1;
				node->child[i]->nDivisionPlane = nextAxis;
			}

			objlnk = new int [node->pMembers->length];
			length = 0;
			for ( j = 0; j < node->pMembers->length; j ++)
				if ( _GeomInNode(node->child[i], node->pMembers->objlnk[j]) )
			{
				objlnk[length]=node->pMembers->objlnk[j];
				length ++;
			}

			node->child[i]->pMembers = new CBspObjList;
			node->child[i]->pMembers->length = length;
			node->child[i]->pMembers->objlnk =
				(int *)realloc( objlnk, length*sizeof(int) );
			if ( i == 1 )
			{
				delete []  node->pMembers->objlnk;
				node->pMembers->length = 0;
			}

			_Subdivide(node->child[i], depth+1, MaxDepth, MaxListLength,
					nextAxis);
		}
	}
}

CBspObjList *CBspTR::_CreateObjectsIDList ( void )
{
	int		i;
	CBspObjList *	rsl;

	rsl = new CBspObjList;
	rsl->length = m_nTriangles;
	rsl->objlnk = new int [rsl->length];
	for ( i = 0; i < rsl->length; i ++ )
		rsl->objlnk[i] = i;
	return ( rsl );
}

/*****************************************************************************
* Inititalize and start the building of BSPTree.
*
* Entry:
*	BSPTree		The BSPTree enclosing the entire scene
******************************************************************************/
void CBspTR::_BuildBSPTree()
{
	// Delete old data
	if (m_ptheTree != NULL) delete m_ptheTree;
	m_ptheTree = new CBspTree;

	/* calc. closing box */
	m_ptheTree->min = m_vMin;
	m_ptheTree->max = m_vMax;
	m_ptheTree->MaxDepth = BSP_MAX_DEPTH;
	m_ptheTree->MaxListLength= BSP_ALLOW_LIST_LENGTH;

	/* Start building the BSPTree by subdividing along the x axis first	*/
	m_ptheTree->root = new CBspNode;
	m_ptheTree->root->min = m_ptheTree->min;
	m_ptheTree->root->max = m_ptheTree->max;
	m_ptheTree->root->nDivisionPlane = 1;
	/* DuplicateLinkList ( m_ptheTree->root->member, m_ptheTree->member);	*/
	m_ptheTree->root->pMembers = _CreateObjectsIDList ();
	_Subdivide( m_ptheTree->root, 0, m_ptheTree->MaxDepth, 
		m_ptheTree->MaxListLength, 1);
}

/////////////////////////////////////////////////////////////////////////////
// Convert the OBJ data for ray tracing
void CBspTR::_ConvertData()
{
	int	i;
	CVec3 vMax, vMin;
	COBJmodel *pObjModel = m_theOBJModel.m_pModel;

	m_nTriangles = pObjModel->nTriangles;
	m_nMaterials = pObjModel->nMaterials;
	m_nVertices  = pObjModel->nVertices;
	m_nNormals   = pObjModel->nNormals;
	m_nTexCoords = pObjModel->nTexCoords;

	m_vpVertices     = new CVec3 [m_nVertices];
	m_vpNormals      = new CVec3 [m_nNormals];
	m_vpTexCoords    = new CVec3 [m_nTexCoords];
	m_cpBrightness   = new CCol4 [m_nVertices];
	m_vpTexAxisU     = new CVec3 [m_nVertices];

	memcpy (m_vpVertices, &pObjModel->vpVertices[1], m_nVertices  * sizeof(CVec3));
	memcpy (m_vpNormals,  &pObjModel->vpNormals[1],  m_nNormals   * sizeof(CVec3));
	memcpy (m_vpTexCoords,&pObjModel->vpTexCoords[1],m_nTexCoords * sizeof(CVec3));

	for (i = 0; i < m_nVertices; i ++)
		m_cpBrightness[i] = COLOR_WHITE;

	for (i = 0; i < m_nNormals; i ++)
		m_vpNormals[i].Normalize();

	m_fMeanArea = 0.f;

	// 1. Loading geometric data
	m_pTriangles = new CBspTriangle [m_nTriangles];
	m_pTriangles[0].Set_static_values ( m_vpVertices, m_vpNormals,
										m_vpTexCoords, m_cpBrightness,
										m_vpTexAxisU);

	for (i = 0; i < m_nTriangles; i ++)
	{
		m_pTriangles[i].SetMaterialID ((int)pObjModel->pTriangles[i].mindex);
		int nVID[3], nNID[3];
		nVID[0] = pObjModel->pTriangles[i].vindices[0]-1;
		nVID[1] = pObjModel->pTriangles[i].vindices[1]-1;
		nVID[2] = pObjModel->pTriangles[i].vindices[2]-1;
		nNID[0] = pObjModel->pTriangles[i].nindices[0]-1;
		nNID[1] = pObjModel->pTriangles[i].nindices[1]-1;
		nNID[2] = pObjModel->pTriangles[i].nindices[2]-1;
		m_pTriangles[i].SetVerticesID ( nVID[0], nVID[1], nVID[2]);
		m_pTriangles[i].SetNormalsID  ( nNID[0], nNID[1], nNID[2]);

		if (pObjModel->pTriangles[i].tindices[0] < 1)
			m_pTriangles[i].SetTexCoordsID (0,0,0);
		else
		{
			m_pTriangles[i].SetTexCoordsID( pObjModel->pTriangles[i].tindices[0]-1,
											pObjModel->pTriangles[i].tindices[1]-1,
											pObjModel->pTriangles[i].tindices[2]-1);
		}

		// Calc bounding box of this triangle
		m_pTriangles[i].Calc_bounding_box();
	}

	// 2. Loading material
	m_pMaterials = new CBspMaterial [m_nMaterials];

	for (i = 0; i < m_nMaterials; i ++)
	{
		CBspMaterial *p1 = &m_pMaterials[i];
		COBJmaterial *p2 = &pObjModel->pMaterials[i];

		memcpy (p1->m_fAmbient,  p2->ambient,  4*sizeof(float));
		memcpy (p1->m_fDiffuse,  p2->diffuse,  4*sizeof(float));
		memcpy (p1->m_fEmissive, p2->emissive, 4*sizeof(float));
		memcpy (p1->m_fSpecular, p2->specular, 4*sizeof(float));
		memcpy (p1->m_fShininess,p2->shininess,  sizeof(float));
		strcpy (p1->m_sTexture,  p2->sTexture);
	}

	// 3. Loading Textures
	for (i = 0; i < m_nMaterials; i ++)
	{
		if (strlen(m_pMaterials[i].m_sTexture) > 4)
			m_pMaterials[i].m_iTexture.LoadTex(m_pMaterials[i].m_sTexture);
		else m_pMaterials[i].m_sTexture[0] = '\0';
	}
}


/////////////////////////////////////////////////////////////////////////////
// _Resize_Model
void CBspTR::_Resize_Model (const float &fSize)
{
	int i;

	float fScale = m_vMax[0] - m_vMin[0];
	if (fScale < (m_vMax[1] - m_vMin[1])) fScale = m_vMax[1] - m_vMin[1];
	if (fScale < (m_vMax[2] - m_vMin[2])) fScale = m_vMax[2] - m_vMin[2];

	fScale = fSize / fScale;

	for (i = 0; i < m_nVertices; i ++)
		m_vpVertices[i] = m_vpVertices[i] * fScale;

	printf ("\nOriginal size (%f,%f,%f) - (%f,%f,%f)",
		m_vMax[0], m_vMax[1], m_vMax[2], m_vMin[0], m_vMin[1], m_vMin[2]);

	printf ("\nScale: %f  Size: %f\n", fScale, fSize);

	m_fMeanArea = 0.f;
	for (i = 0; i < m_nTriangles; i ++)
	{
		m_fMeanArea += m_pTriangles[i].Calc_area();
		m_pTriangles[i].Calc_bounding_box();
	}
	m_fMeanArea /= m_nVertices;
}


/////////////////////////////////////////////////////////////////////////////
// _Resize_Model
void CBspTR::_Calc_bounding_box ()
{
	m_vMax = m_vMin = m_vpVertices[0];

	for (int i = 0; i < m_nVertices; i ++)
	{
		if (m_vMax[0] < m_vpVertices[i][0]) m_vMax.x = m_vpVertices[i][0];
		if (m_vMax[1] < m_vpVertices[i][1]) m_vMax.y = m_vpVertices[i][1];
		if (m_vMax[2] < m_vpVertices[i][2]) m_vMax.z = m_vpVertices[i][2];

		if (m_vMin[0] > m_vpVertices[i][0]) m_vMin.x = m_vpVertices[i][0];
		if (m_vMin[1] > m_vpVertices[i][1]) m_vMin.y = m_vpVertices[i][1];
		if (m_vMin[2] > m_vpVertices[i][2]) m_vMin.z = m_vpVertices[i][2];
	}

	m_vMax += (m_vMax - m_vMin)*0.001f;
	m_vMin += (m_vMin - m_vMax)*0.001f;

	m_fSceneSize = (m_vMax - m_vMin).Length();

	printf ("\nScene size (%f,%f,%f) - (%f,%f,%f)",
		m_vMax[0], m_vMax[1], m_vMax[2], m_vMin[0], m_vMin[1], m_vMin[2]);
}


/////////////////////////////////////////////////////////////////////////////
// Load the OBJ model
// Convert it into the data structure that fit for ray tracing
// input: fn, the file name of a wavefront OBJ model
/////////////////////////////////////////////////////////////////////////////
void CBspTR::Load_OBJ ( char *fn, float fSize)
{
	printf("\nLoading %s...", fn);
	m_theOBJModel.LoadOBJ(fn);

	printf("\nConverting data ..");
	_DestroyScene();
	_ConvertData();

	m_theOBJModel.Destory();

	_Calc_bounding_box();
	if (fSize > 0.f) _Resize_Model(fSize);
	_Calc_bounding_box();

	m_fSceneSize = (m_vMax - m_vMin).Length();

	printf ("\nBuilding BSP tree ...\n");
	_BuildBSPTree();

	m_bModelLoaded = TRUE;
}

void CBspTR::_DestroyScene()
{
	if (m_nTriangles != 0)	delete [] m_pTriangles;
	m_nTriangles = 0;
	m_pTriangles = NULL;

	if (m_nMaterials != 0)	delete [] m_pMaterials;
	m_nMaterials = 0;
	m_pMaterials = NULL;

	if (m_ptheTree != NULL) delete m_ptheTree;
	m_ptheTree = NULL;

	if (m_vpVertices      != NULL) delete [] m_vpVertices;
	if (m_vpNormals       != NULL) delete [] m_vpNormals;
	if (m_vpTexCoords     != NULL) delete [] m_vpTexCoords;
	if (m_cpBrightness    != NULL) delete [] m_cpBrightness;
	if (m_vpTexAxisU      != NULL) delete [] m_vpTexAxisU;

	m_vpVertices     = NULL;
	m_vpNormals      = NULL;
	m_vpTexCoords    = NULL;
	m_cpBrightness   = NULL;
	m_vpTexAxisU     = NULL;

	m_nVertices   = 0;
	m_nTexCoords  = 0;
}







//----------------------------------------------------------------------------
//	_TraceOneRay
//----------------------------------------------------------------------------
CCol4 CBspTR::_TraceOneRay(TBspRay ray, CCol4 cRemainder, int nDepth)
{
	int	pID;
	TBspRay	ray2;
	TBspCross crs, crs2;
	CCol4	rsl, col, cRefRsl;
	CBspMaterial *pMtrl;

	// Check intersection
	pID = -1;
	if ( !_RayTreeIntersect(ray, crs, pID ) )
		return (m_cBgColor*cRemainder);

	// First step
	pMtrl = &m_pMaterials[m_pTriangles[pID].GetMaterialID()];

	ray2.vOrg = crs.vPos;
	ray2.vDir = m_vLightDir;
	ray2.vDir.Normalize();
	ray2.vEnd = ray2.vOrg + m_vLightDir * (m_fSceneSize);

	rsl.r() = rsl.g() = rsl.b() = 0.f;
	CVec3 vView = m_theCamera.Eye() - crs.vPos;
	vView.Normalize();

	if (! _RayTreeIntersectForShadowTest (ray2, crs2, pID ) )
	{
		rsl = m_iShaders.PhongShading (m_cLightColor, m_vLightDir,
			vView, crs.vPos, crs.vNml,
			crs.vTex[0], crs.vTex[1], pMtrl);

	}
	else
	{
		rsl = m_iShaders.PhongShading (COLOR_BLACK, m_vLightDir,
			vView, crs.vPos, crs.vNml,
			crs.vTex[0], crs.vTex[1], pMtrl);
	}

	// Reflection
	float fRefAtten = pMtrl->m_fShininess[0]/128.f;
	fRefAtten *= fRefAtten;
	col.r() = cRemainder.r() * fRefAtten * pMtrl->m_fSpecular[0] * pMtrl->m_fSpecular[3];
	col.g() = cRemainder.g() * fRefAtten * pMtrl->m_fSpecular[1] * pMtrl->m_fSpecular[3];
	col.b() = cRemainder.b() * fRefAtten * pMtrl->m_fSpecular[2] * pMtrl->m_fSpecular[3];
	if ((nDepth < 4) && ((col[0] > .004f)||(col[1] > .004f)||(col[2] > .004f)))
	{
		float n_len = crs.vNml.Length();
		CVec3 ptmp = ray.vDir * (-1.f);
		ptmp  = ptmp * (n_len/fabs(crs.vNml.Dot(ptmp)));
		ray2.vDir = (crs.vNml * 2.f) - ptmp;

		float fDisturb = (128 - pMtrl->m_fShininess[0])/256;
		fDisturb = fDisturb * fDisturb;
		fDisturb = fDisturb * fDisturb;

		ray2.vDir.x += fDisturb*(rand() - rand())/RAND_MAX;
		ray2.vDir.y += fDisturb*(rand() - rand())/RAND_MAX;
		ray2.vDir.z += fDisturb*(rand() - rand())/RAND_MAX;
		ray2.vDir.Normalize();

		ray2.vOrg = crs.vPos;
		ray2.vEnd = crs.vPos + (ray2.vDir * m_fSceneSize);

		cRefRsl = _TraceOneRay(ray2, col, nDepth +1);
	}
	else	cRefRsl.r() = cRefRsl.g() = cRefRsl.b() = 0.f;

	rsl += cRefRsl;
	rsl.Unify();

	return ( rsl * cRemainder );
}

//----------------------------------------------------------------------------
//	_RayTraceOnePixel
//----------------------------------------------------------------------------
CCol4 CBspTR::_RayTraceOnePixel(CVec3 m_vPos)
{
	TBspRay	ray;

	// The ray
	ray.vOrg = m_theCamera.Eye();
	ray.vEnd = m_vPos;
	ray.vDir = m_vPos - m_theCamera.Eye();
	ray.vDir.Normalize();

	return (_TraceOneRay (ray, CCol4(1.f,1.f,1.f), 0));
}

void CBspTR::Render()
{
	int   x, y, bx, rp;
	int   BufWidth, BufHeight;
	CVec3 vLeftBottom, CurrPixel, daltaX, daltaY, CurrY;
	CCol4 **ColBuf, rsl_color, m_cBgColor;

	//1. Open screen-buffer-data file & alloc memory for buffers
	BufWidth  = m_theCamera.Film_width() * 2 + 1;
	BufHeight = m_theCamera.Film_height()* 2 + 1;

	ColBuf= new CCol4 * [4];
	for (y = 0; y < 3; y ++)	ColBuf[y] = new CCol4[BufWidth];

	// 2. Ray-tracing
	m_theCamera.ComputerRenderingParameters3x3(daltaX, daltaY, vLeftBottom);

	CurrPixel = vLeftBottom;
	rp = 0;

	for ( x = 0; x < BufWidth; x ++ )
	{
		ColBuf[0][x] = _RayTraceOnePixel (CurrPixel);
		CurrPixel = vLeftBottom + daltaX * x;
	}

//	CCol4 *pResultImage = m_theCamera.ResultImagePtr();
	printf ("\nRay casting ");
	for ( y = 0; y < m_theCamera.Film_height(); y ++ )
	{
		if (y % 20 == 0) printf (".");
		CurrY = vLeftBottom + daltaY * (2*y);
		for ( x = 0; x < BufWidth; x ++ )
		{
			CurrPixel = CurrY + daltaX * x;

			CurrPixel = CurrPixel + daltaY;
			ColBuf[1][x] = _RayTraceOnePixel ( CurrPixel );

			CurrPixel = CurrPixel + daltaY;
			ColBuf[2][x] = _RayTraceOnePixel ( CurrPixel );
		}

		//    (bx+0, 0) +------+------+------+
		//              | 1/16 | 2/16 | 1/16 |	weight table
		//              +------+------+------+
		//              | 2/16 | 4/16 | 2/16 |	for 9-points anti-aliasing
		//              +------+------+------+
		//              | 1/16 | 2/16 | 1/16 |
		//              +------+------+------+ ( bx+2, 2)
		bx = 0;
		for ( x = 0; x < m_theCamera.Film_width(); x ++ )
		{
			rsl_color.r() = ( ColBuf[0][bx+0].r() + ColBuf[0][bx+2].r() +
					ColBuf[2][bx+0].r() + ColBuf[2][bx+2].r() +
				2.f * ( ColBuf[0][bx+1].r() + ColBuf[1][bx+0].r() +
					ColBuf[1][bx+2].r() + ColBuf[2][bx+1].r() ) +
				4.f * ( ColBuf[1][bx+1].r() ) ) / 16.f;
			rsl_color.g() = ( ColBuf[0][bx+0].g() + ColBuf[0][bx+2].g() +
					ColBuf[2][bx+0].g() + ColBuf[2][bx+2].g() +
				2.f * ( ColBuf[0][bx+1].g() + ColBuf[1][bx+0].g() +
					ColBuf[1][bx+2].g() + ColBuf[2][bx+1].g() ) +
				4.f * ( ColBuf[1][bx+1].g() ) ) / 16.f;
			rsl_color.b() = ( ColBuf[0][bx+0].b() + ColBuf[0][bx+2].b() +
					ColBuf[2][bx+0].b() + ColBuf[2][bx+2].b() +
				2.f * ( ColBuf[0][bx+1].b() + ColBuf[1][bx+0].b() +
					ColBuf[1][bx+2].b() + ColBuf[2][bx+1].b() ) +
				4.f * ( ColBuf[1][bx+1].b() ) ) / 16.f;
			bx += 2;

			m_theCamera.Set_pixel (x, y, rsl_color);
		}
		memcpy ( ColBuf[0],ColBuf[2],sizeof(CCol4)*BufWidth );
	}

	printf ("\n%c", 7);

	// 3. Free memory
	for ( y = 0; y < 3; y++ )
		delete [] ColBuf[y];
	delete [] ColBuf;
}


CVec3 CBspTR::_Reflect_the_ray(const CVec3 &vIn, const CVec3 & vNml, const float & fGlossy)
{
	float fLen = vNml.Length();
	CVec3 vTemp = vIn * (-1.f);
	vTemp = vTemp * (fLen/(float)fabs(vNml.Dot(vTemp)));
	CVec3 rsl = (vNml * 2.f) - vTemp;
	rsl.Normalize();
	
	float fTemp = fGlossy * fGlossy;
	rsl.x += fTemp * RANDOM_N1_P1;
	rsl.y += fTemp * RANDOM_N1_P1;
	rsl.z += fTemp * RANDOM_N1_P1;

	if (rsl.Dot (vNml) < 0.f)
		return _Reflect_the_ray (vIn, vNml, fGlossy);

	rsl.Normalize();
	return rsl;
}
