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


// BspTR.h: interface for the CBspTR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSPTR_H__A77E503F_72AA_44AE_8507_309ABA45A60F__INCLUDED_)
#define AFX_BSPTR_H__A77E503F_72AA_44AE_8507_309ABA45A60F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vector3f.h"
#include "color.h"
#include "common.h"
#include "AccessOBJ.h"
#include "BspMaterial.h"
#include "BspTexture.h"
#include "Shaders.h"
#include <fstream>
#include "Singleton.h"
#include "Log.h"

#define	BSP_MAX_DEPTH			21
#define	BSP_ALLOW_LIST_LENGTH	8
#define	STACKSIZE				50

#define TRIANGLE_PATCH			3
#define RECTANGLE_PATCH			4

#define	UNIT_VALUE(x)		  ( (0.<=x) && (x<=1.) )
#define MID(x, y, z)		  ((x>y && x<z) || (x<y && x>z))

#define BLEND_PRODUCT(a,b,c)  ((a.Cross(b)).Dot(c))


// --------------------------------------------------------------------
// TBspRay class
struct TBspRay
{
	CVec3 vOrg;	// ray origin */
	CVec3 vEnd;	// ray destination */
	CVec3 vDir;	// unit vector, indicating ray direction */
};

// --------------------------------------------------------------------
// cross class : the intersection point
struct TBspCross
{
	CVec3 vPos;	// interection point
	CVec3 vNml;	// normal at the intersection point
	CCol4 cCol;	// color at the intersection point
	CVec3 vTex;	// texture coordinate at the intersection point
	CVec3 vUVW; // the parameters for interpolation
	float fDist;// parameteric distance
};

// --------------------------------------------------------------------
// Triangle class
class CBspTriangle
{
private:
	static CVec3 *m_vpVertices;
	static CVec3 *m_vpNormals;
	static CVec3 *m_vpTexCoords;
	static CCol4 *m_cpBrightness;
	static CVec3 *m_vpTexAxisU;

	int	  m_nMtrl;			// material index
	CVec3 m_vMin, m_vMax;	// Bounding box of this patch
	int   m_nVrt[3];		// 3 vertex indices
	int   m_nNml[3];        // 3 Normal indices
	int   m_nTex[3];		// texture coord

	CVec3 _Vrt (int i) const;
	CVec3 _Nml (int i) const;
	CVec3 _Tex (int i) const;
	CCol4 _Bri (int i) const;
	CVec3 _TexAxisU (int i) const;

public:
	float Calc_area();
	CVec3 Calc_normal();
	void  Set_static_values (CVec3 *pVrt, CVec3 *pNml, CVec3 *pTex, CCol4 *pVInt, CVec3 *pAxisU);

	void  SetMaterialID  (int ID) {m_nMtrl = ID;}
	void  SetVerticesID  (int v0, int v1, int v2);
	void  SetNormalsID   (int v0, int v1, int v2);
	void  SetTexCoordsID (int t0, int t1, int t2);

	void  Calc_bounding_box();
	int   GetMaterialID (void)   {return m_nMtrl;}
	int   GetVertexID(int nVID)
	{
		if (nVID >= 0 && nVID < 3)
			return m_nVrt[nVID];
		else return -1;
	}
	int   GetNormalID(int nVID)
	{
		if (nVID >= 0 && nVID < 3)
			return m_nNml[nVID];
		else return -1;
	}
	int   GetTextureID(int nTID)
	{
		if (nTID >= 0 && nTID < 3)
			return m_nTex[nTID];
		else return -1;
	}

	CVec3 *VrtPtr(int i) const;
	CVec3 *NmlPtr(int i) const;
	CVec3 *TexPtr(int i) const;
	CVec3 *TexAxisUPtr(int i) const;
	CCol4 *BriPtr(int i) const;

	CVec3 *Bounding_box_max()    {return &m_vMax;}
	CVec3 *Bounding_box_min()    {return &m_vMin;}

	CVec3 Random_point_on_the_triangle()
	{
		//CVec3 t = CVec3((float)rand(), (float)rand(), (float)rand());
		//t = t * ( 1.f / (t[0] + t[1] + t[2]) );
		//return (m_vpVertices[m_nVrt[0]] * t[0] + 
		//	m_vpVertices[m_nVrt[1]] * t[1] + m_vpVertices[m_nVrt[2]] * t[2]);

		while (1)
		{
			CVec3 vl, vr;
			float t0 = RANDOM_0_1;
			float t1 = RANDOM_0_1;
			if (t1 <= t0)
			{
				vl = m_vpVertices[m_nVrt[0]] + 
					(m_vpVertices[m_nVrt[1]] - m_vpVertices[m_nVrt[0]]) * t0;
				vr = m_vpVertices[m_nVrt[0]] + 
					(m_vpVertices[m_nVrt[2]] - m_vpVertices[m_nVrt[0]]) * t0;
				return vl + (vr - vl) * RANDOM_0_1;
			}
		}
	}


	BOOL Ray_intersection_flat (TBspRay &ray, TBspCross &crs);
	BOOL Ray_intersection_smooth (TBspRay &ray, TBspCross &crs);
	BOOL Ray_intersection_FB (TBspRay &ray, TBspCross &crs, BOOL &bFront);
	BOOL Ray_intersection_back_face (TBspRay &ray, TBspCross &crs, const float &fTolerance);

	CBspTriangle(){};
	virtual ~CBspTriangle() {};
};

// --------------------------------------------------------------------
// Ball class
class CBspBall
{
public:
	CVec3 m_vCent;
	float m_fRadius;

public:
	BOOL Ray_intersection (TBspRay &ray, TBspCross &crs)
	{
		int	  bHit;                        // True if ray intersects sphere
		CVec3 vTemp = m_vCent - ray.vOrg;  // Ray base to sphere center
		float bsq = vTemp.Dot(ray.vDir);

		if (bsq < 0.f) return (FALSE);

		float u = vTemp.Dot(vTemp) - m_fRadius * m_fRadius;
		float disc = bsq * bsq - u;

		bHit = (disc >= 0.f);
		if (bHit)
		{
			float root = sqrt(disc);
			float t    = bsq - root;		/*    entering distance	*/

			crs.vPos = ray.vOrg + ray.vDir * t;
			crs.vNml = crs.vPos - m_vCent;
			crs.vNml.Normalize();
			crs.fDist = (crs.vPos - ray.vOrg ).Length() /
				(ray.vEnd - ray.vOrg).Length();
		}

		return (bHit);
	}

	CBspBall () {};
	~CBspBall () {};
};

// --------------------------------------------------------------------
class CBspObjList
{
public:
	int			*objlnk;	// Link list of primitives
	int			length;		// Length of the list

	CBspObjList()
	{
		objlnk = NULL;
		length = 0;
	}
	virtual ~CBspObjList()
	{
		if (length != 0) delete [] objlnk;
	}
};

// --------------------------------------------------------------------
class CBspNode
{
public:
	CVec3		min, max;	// extent of node
	CBspObjList	*pMembers;	// list of enclosed primitives
	CBspNode	*child[2];	// pointers to children nodes, if any
	int			nDivisionPlane;

	CBspNode()
	{
		child[0] = child[1] = NULL;
		pMembers = NULL;
	}
	virtual ~CBspNode()
	{
		if (child[0] != NULL) delete child[0];
		if (child[1] != NULL) delete child[1];
		if (pMembers != NULL) delete pMembers;
	}
};

// --------------------------------------------------------------------
class CBspTree
{
public:
	CVec3		min, max;	// extent of the entire bin tree
	int			MaxDepth;	// max allowed depth of the tree
	int			MaxListLength;	// max primitive allowed in a leaf node
	CBspNode *	root;		// root of the entire bin tree

	CBspTree()
	{ root = NULL; }

	virtual ~CBspTree()
	{
		if (root != NULL) delete root;
	}
};

// --------------------------------------------------------------------

class CCamera
{
public:
	float m_fViewAngle;		// View angle
	CVec3 m_vEye;			// Eye position
	CVec3 m_vCent;			// View center
	CVec3 m_vUpright;		// Upright
	float m_fNear;			// Near culling plane
	float m_fFar;			// Far culling plane
private:
	int   m_nImageWidth;	// Result image width
	int   m_nImageHeight;	// Result image height
	CCol4 *m_pResultImage;	// Result image
	CCol4 *m_pResultImageWithFiltered; // Result image after filtered
public:
	float ViewAngle() {return m_fViewAngle;}
	CVec3 Eye ()      {return m_vEye;}
	CVec3 Cent ()     {return m_vCent;}
	CVec3 Upright ()  {return m_vUpright;}
	float Near ()     {return m_fNear;}
	float Far()       {return m_fFar;}

	bool saveResultImageDataToFile(char* fileName)
	{
		if(fileName==NULL)
			return false;

		if(m_nImageWidth<=0||m_nImageHeight<=0||m_pResultImage==NULL)
			return false;
		
		std::ofstream ooFile(fileName, std::ios::binary);
		if(ooFile.fail())
			return false;

		ooFile.write((char*)(&m_nImageWidth),sizeof(int));
		ooFile.write((char*)(&m_nImageHeight),sizeof(int));
		ooFile.write((char*)(m_pResultImage),sizeof(CColor4f) * m_nImageWidth * m_nImageHeight );
		ooFile.close();
		return true;
	}

	bool loadResultImageDataFromFile(char*fileName)
	{
		if(fileName==NULL)
			return false;

		std::ifstream iiFile(fileName,std::ios::binary);
		if(iiFile.fail())
			return false;

		//prepare memory
		int t_widthHeight[2];
		//read width and height
		iiFile.read((char*)(t_widthHeight),2*sizeof(int));
		if(t_widthHeight[0]*t_widthHeight[1]>m_nImageWidth*m_nImageHeight||m_pResultImage==NULL||m_pResultImageWithFiltered==NULL)
		{
			MY_DELETE_ARRAY(m_pResultImage);
			MY_DELETE_ARRAY(m_pResultImageWithFiltered);
			m_pResultImage	=	new CColor4f[t_widthHeight[0] * t_widthHeight[1] ];
			if(m_pResultImage==NULL)
				return false;
			m_pResultImageWithFiltered	=	new CColor4f[t_widthHeight[0] * t_widthHeight[1] ];
			if(m_pResultImageWithFiltered==NULL)
				return false;
			
			memset(m_pResultImage, 0, sizeof(CColor4f) * t_widthHeight[0] * t_widthHeight[1]);
			memset(m_pResultImageWithFiltered, 0, sizeof(CColor4f) * t_widthHeight[0] * t_widthHeight[1]);
		
		}
		else
		{
			memset(m_pResultImage, 0, sizeof(CColor4f) * m_nImageWidth * m_nImageHeight);
			memset(m_pResultImageWithFiltered, 0, sizeof(CColor4f) * m_nImageWidth * m_nImageHeight);


		}

		m_nImageWidth	=	t_widthHeight[0];
		m_nImageHeight	=	t_widthHeight[1];

		//read data
		iiFile.read((char*)(m_pResultImage), sizeof(CColor4f) * m_nImageWidth * m_nImageHeight);
		memcpy(m_pResultImageWithFiltered, m_pResultImage,  sizeof(CColor4f) * m_nImageWidth * m_nImageHeight);

		return true;
	}

	bool  InitCamera (
                    float fViewAngle,
					CVec3 vEye, 
                    CVec3 vCent, 
                    CVec3 vUpright,
					float fNear, 
                    float fFar,
					int nWidth, 
                    int nHeight)
	{
		m_fViewAngle    = fViewAngle;
		m_vEye             = vEye;
		m_vCent           = vCent;
		m_vUpright       = vUpright;
		m_fNear            = fNear;
		m_fFar              = fFar;
	
        if(m_pResultImage==NULL||m_pResultImageWithFiltered==NULL||
            m_nImageWidth!=nWidth||m_nImageHeight!=nHeight)
        {            
            MY_DELETE_ARRAY(m_pResultImage);
            MY_DELETE_ARRAY(m_pResultImageWithFiltered);

            m_nImageWidth  = nWidth;
            m_nImageHeight = nHeight;
		    m_pResultImage = new CCol4 [m_nImageWidth*m_nImageHeight];
		    m_pResultImageWithFiltered	=	new CCol4[m_nImageWidth*m_nImageHeight];
            if(m_pResultImage==NULL||m_pResultImageWithFiltered==NULL)
            {
                ZHLog* _log    = Singleton<ZHLog>::getSingletonPtr();
                if(_log)
                    _log->assertError("InitCamera()| m_pResultImage = new CCol4 [m_nImageWidth*m_nImageHeight] failed!.");
                return false;
            }
         }
       

		memset((void*)m_pResultImage, 0, sizeof(CColor4f)* m_nImageWidth*m_nImageHeight);
		memset((void*)m_pResultImageWithFiltered, 0, sizeof(CColor4f)* m_nImageWidth*m_nImageHeight);
        
        return true;
	}

	inline int   Film_width()const   {return m_nImageWidth; }
	inline int   Film_height()const  {return m_nImageHeight;}
	inline CCol4 *ResultImagePtr() {return m_pResultImage;}
	inline CCol4 *FilteredResultImagePtr(){return m_pResultImageWithFiltered;}
	void  Set_pixel (int x, int y, CCol4 col)
	{
		m_pResultImage[y*m_nImageWidth+x] = col;
	}
	void  ComputerRenderingParameters(CVec3 &daltaX, CVec3 &daltaY, CVec3 &vLB);
	void  ComputerRenderingParameters3x3(CVec3 &daltaX, CVec3 &daltaY, CVec3 &vLB);
	CCamera()
	{
		m_fViewAngle = 60.f;
		m_fNear = 1.f;
		m_fFar = 100.f;
		m_nImageWidth  = 0;
		m_nImageHeight = 0;
		m_pResultImage = NULL;
		m_pResultImageWithFiltered = NULL;
	}
	virtual ~CCamera()
	{
		MY_DELETE_ARRAY(m_pResultImage);
		MY_DELETE_ARRAY(m_pResultImageWithFiltered);

	}
};




#define NORMAL_RAY_TRACING	0
#define FAST_RAY_TRACING	1

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
class CBspTR
{
protected:
	struct BspObjLink
	{
		int	   ID;
		struct BspObjLink	*next;
	};
	struct BspStackElem
	{
		CBspNode *	node;
		double		min, max;
	};

	struct Stack
	{
		int	stackPtr;
		BspStackElem stack[STACKSIZE];
	};
	typedef Stack *StackPtr;

protected:

	// --------------------------------------------------------------------
	// variables

	CVec3 *m_vpVertices;
	CVec3 *m_vpNormals;
	CVec3 *m_vpTexCoords;
	CCol4 *m_cpBrightness;
	CVec3 *m_vpTexAxisU;

	int   m_nVertices;
	int   m_nNormals;
	int   m_nTexCoords;

	CBspTriangle *m_pTriangles;
	CBspMaterial *m_pMaterials;

	int   m_nTriangles;
	int   m_nMaterials;

	float m_fMeanArea;        // Mean area of all triangles
	float m_fSceneSize;       // The diagonal length of the bounding box
	CVec3 m_vMin, m_vMax;

	CVec3 m_vLightDir;       // Lighting direction
	CCol4 m_cLightColor;     // Light color
	CCol4 m_cBgColor;        // Background color
	int   m_nTracingType;

	CBspTree*   m_ptheTree;
	CAccessObj  m_theOBJModel; // the temporal obj model
	CCamera     m_theCamera;

	BOOL m_bModelLoaded;

	CShaders m_iShaders;

	// --------------------------------------------------------------------
	//functions
protected:
	void   _DestroyScene();
	void   _Calc_bounding_box ();
	void   _Resize_Model (const float &fSize);
	void   _ConvertData();
	void   _BuildBSPTree();
	int    _AvailableNormal( CVec3*, CVec3, CVec3*, int );
	int    _CBoxAtTheOtherSide(CVec3, CVec3, CVec3, CVec3 );
	int    _IsObjectInNode( int ID, CVec3 min, CVec3 max );
	void   _InitStack(StackPtr stack);
	void   _push(StackPtr stack, CBspNode * node, double min, double max);
	void   _pop(StackPtr stack, CBspNode * *node, double *min, double *max);
	double _DistanceToDivisionPlane(CVec3 plane, TBspRay ray,
									int nDivisionPlane);
	void   _GetChildren(CBspNode * currentNode, CVec3 origin,
					CBspNode **nearChild, CBspNode **farChild,
					int nTheChildren);
	int    _Leaf ( CBspNode * node );
	int    _PointInNode ( CBspNode * node, CVec3 pt );
	int    _GeomInNode ( CBspNode * node, int ID );
	void   _PointAtDistance ( TBspRay ray, double distance, CVec3 *pt );
	int    _RayBoxIntersect ( TBspRay ray, CVec3 min, CVec3 max,
						double *mind, double *maxd );
	void   _Subdivide(CBspNode * node, int depth, int MaxDepth,
					int MaxListLength, int axis );
	CBspObjList *_CreateObjectsIDList ( void );
	CVec3 _Reflect_the_ray(const CVec3 &vIn, const CVec3 & vNml, const float & fGlossy);

	virtual BOOL _RayTreeIntersect (TBspRay &ray, TBspCross &crs, int &ID);
	virtual BOOL _RayTreeIntersect_back_face ( TBspRay &ray, TBspCross &crs, int &ID, const float &fTolerance);
	virtual BOOL _RayTreeIntersectForShadowTest (TBspRay &ray, TBspCross &crs, int ID);
	virtual CCol4  _RayTraceOnePixel (CVec3 sp);
	virtual CCol4  _TraceOneRay(TBspRay ray, CCol4 cRemainder, int nDepth);

public:
	BOOL   Is_model_loaded()                                {return m_bModelLoaded;}
	int    TriangleNum()                                    {return m_nTriangles;}
	int    MaterialNum()                                    {return m_nMaterials;}
	void   Get_boundingbox (CVec3 &vMax, CVec3 &vMin)       {vMax = m_vMax, vMin = m_vMin;}
	CVec3  *Get_triangle_ver(int nTID, int nVID)	        {return m_pTriangles[nTID].VrtPtr(nVID);}
	CVec3  *Get_triangle_nml(int nTID, int nVID)	        {return m_pTriangles[nTID].NmlPtr(nVID);}
	CVec3  Get_triangle_nml(int nTID)                       {return m_pTriangles[nTID].Calc_normal();}
	CVec3  *Get_triangle_tex(int nTID, int nVID)	        {return m_pTriangles[nTID].TexPtr(nVID);}
	CCol4  *Get_triangle_vrt_color(int &nTID, int &nVID)	{return m_pTriangles[nTID].BriPtr(nVID);}
	CCamera *Get_camera (void)                              {return &m_theCamera;}

	CBspMaterial *Get_tri_material(int nTID)                {return &m_pMaterials[m_pTriangles[nTID].GetMaterialID()];}

	void   SetTracingType (int ntt) {	m_nTracingType = ntt;	}
	void   SetBgColor(CCol4 cBgColor);
	void   SetRayTracingFrustum(float fViewAngle,
							CVec3 vEye, CVec3 vCent, CVec3 vUpright,
							float fNear, float fFar,
							int nWidth, int nHeight)
	{
		m_theCamera.InitCamera (fViewAngle, vEye, vCent, vUpright,
							fNear, fFar, nWidth, nHeight);
	}


	void SetLight(CVec3 vLigDir, CCol4 color)
	{
		m_vLightDir = vLigDir.Normalized();
		m_cLightColor = color;
	}

	virtual void Render();

	// Result image width
	int    ResultWidth()  { return m_theCamera.Film_width(); }
	// Result image height
	int    ResultHeight() { return m_theCamera.Film_height();}
	// Result image, (unsigned byte pointer)
	CCol4   *ResultImagePtr()  { return m_theCamera.ResultImagePtr();}

	virtual void Load_OBJ( char *fn, float fMapSize = 0.f);

	CBspTR();
	virtual ~CBspTR();
};

#endif //!defined(AFX_BSPTR_H__A77E503F_72AA_44AE_8507_309ABA45A60F__INCLUDED_)

